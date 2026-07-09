#ifndef TEAR_PLATFORM_LINUX_H
#define TEAR_PLATFORM_LINUX_H

void tear_platform_log(const char *component, const char *message);
int tear_platform_make_run_id(char *buf, unsigned long buf_len);

#endif
