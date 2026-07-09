#ifndef TEAR_WORKLOAD_CALLBACK_API_H
#define TEAR_WORKLOAD_CALLBACK_API_H

#ifdef __cplusplus
extern "C" {
#endif

#define TEAR_CALLBACK_API_VERSION 1

struct tear_callback_context {
    const char *workload_name;
    const char *run_id;
};

struct tear_callback_result {
    int exit_code;
    const char *message;
};

typedef int (*tear_workload_run_fn)(
    const struct tear_callback_context *ctx,
    struct tear_callback_result *result
);

int tear_workload_run(
    const struct tear_callback_context *ctx,
    struct tear_callback_result *result
);

#ifdef __cplusplus
}
#endif

#endif
