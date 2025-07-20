#ifndef __HIDE_FILES_H
#define __HIDE_FILES_H

void enable_hide_files_hook(unsigned long **syscall_table);

void disable_hide_files_hook(unsigned long **syscall_table);

#endif
