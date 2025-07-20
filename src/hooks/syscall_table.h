#ifndef __SYSCALL_TABLE_H
#define __SYSCALL_TABLE_H

#include <linux/syscalls.h>
#include <linux/types.h>

unsigned long **get_syscall_table(void);

void disable_write_protection(void);
void enable_write_protection(void);

#endif
