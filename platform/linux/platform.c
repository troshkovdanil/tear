#include "platform/linux/platform.h"

#include <stdio.h>
#include <time.h>
#include <unistd.h>

void tear_platform_log(const char *component, const char *message)
{
    if (!component) {
        component = "unknown";
    }

    if (!message) {
        message = "";
    }

    fprintf(stderr, "[%s] %s\n", component, message);
}

int tear_platform_make_run_id(char *buf, unsigned long buf_len)
{
    if (!buf || buf_len == 0) {
        return -1;
    }

    const long now = (long)time(NULL);
    const long pid = (long)getpid();

    const int written = snprintf(buf, buf_len, "run-%ld-%ld", now, pid);
    if (written < 0 || (unsigned long)written >= buf_len) {
        return -1;
    }

    return 0;
}
