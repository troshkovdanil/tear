#include "platform/linux/platform.h"

#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

static volatile sig_atomic_t tear_platform_stop_requested = 0;

static void tear_platform_handle_signal(int signo)
{
    (void)signo;
    tear_platform_stop_requested = 1;
}

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

int tear_platform_setup_signal_handlers(void)
{
    struct sigaction action;

    action.sa_handler = tear_platform_handle_signal;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    if (sigaction(SIGINT, &action, NULL) != 0) {
        return -1;
    }

    if (sigaction(SIGTERM, &action, NULL) != 0) {
        return -1;
    }

    return 0;
}

int tear_platform_should_stop(void)
{
    return tear_platform_stop_requested != 0;
}
