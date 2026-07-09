#include "backend/mock/backend.h"
#include "platform/linux/platform.h"
#include "workload/callback_api.h"

#include <stdio.h>

int tear_workload_run(
    const struct tear_callback_context *ctx,
    struct tear_callback_result *result
)
{
    if (!ctx || !result) {
        return 1;
    }

    tear_platform_log("demo", "callback started");

    printf("Hello from TEAR demo workload\n");
    printf("workload=%s\n", ctx->workload_name);
    printf("run_id=%s\n", ctx->run_id);

    result->exit_code = 0;
    result->message = "demo workload completed";

    tear_platform_log("demo", "callback completed");

    return 0;
}
