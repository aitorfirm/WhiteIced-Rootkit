#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/syscalls.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/paravirt.h>
#include "syscall_table.h"

unsigned long **get_syscall_table(void)
{
    unsigned long int offset;
    unsigned long **sct;

    for (offset = PAGE_OFFSET; offset < ULLONG_MAX; offset += sizeof(void *)) {
        sct = (unsigned long **)offset;

        if (sct[__NR_close] == (unsigned long *)ksys_close)
            return sct;
    }

    return NULL;
}

void disable_write_protection(void)
{
    preempt_disable();
    barrier();
    write_cr0(read_cr0() & (~0x10000));
    barrier();
}

void enable_write_protection(void)
{
    barrier();
    write_cr0(read_cr0() | 0x10000);
    barrier();
    preempt_enable();
}
