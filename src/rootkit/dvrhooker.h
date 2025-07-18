#ifndef __HOOKER_H
#define __HOOKER_H

#include <linux/fs.h>
#include <linux/uaccess.h>

int init_hooks(void);

void cleanup_hooks(void);

#endif
