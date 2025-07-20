#ifndef __KILL_HOOK_H
#define __KILL_HOOK_H

void enable_kill_hook(unsigned long **syscall_table);
void disable_kill_hook(unsigned long **syscall_table);

#endif
