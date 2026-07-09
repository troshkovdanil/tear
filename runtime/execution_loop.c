#include "runtime/execution_loop.h"

#include "backend/mock/backend.h"
#include "platform/linux/platform.h"
#include "runtime/callback_loader.h"
#include "workload/callback_api.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static const char *callback_path_for_workload(const char *workload_name)
{
    const char *override = getenv("TEAR_CALLBACK_PATH");
    if (override && override[0] != '\0') {
        return override;
    }

    if (workload_name && strcmp(workload_name, "demo") == 0) {
        if (access("./lib/libtear-callback-demo-linux-mock.so", R_OK) == 0) {
            return "./lib/libtear-callback-demo-linux-mock.so";
        }

        if (access("../lib/libtear-callback-demo-linux-mock.so", R_OK) == 0) {
            return "../lib/libtear-callback-demo-linux-mock.so";
        }

        return "./build/lib/libtear-callback-demo-linux-mock.so";
    }

    return NULL;
}

int tear_execution_loop_run(const char *workload_name)
{
    if (!workload_name) {
        return 1;
    }

    tear_platform_log("teard", "execution requested");

    if (tear_backend_mock_verify_workload(workload_name) != 0) {
        return 1;
    }

    char run_id[64];
    if (tear_platform_make_run_id(run_id, sizeof(run_id)) != 0) {
        tear_platform_log("teard", "failed to create run id");
        return 1;
    }

    const char *callback_path = callback_path_for_workload(workload_name);
    if (!callback_path) {
        tear_platform_log("teard", "no callback path for workload");
        return 1;
    }

    fprintf(stderr, "[teard] loading callback: %s\n", callback_path);

    struct tear_loaded_callback callback;
    if (tear_callback_load(callback_path, &callback) != 0) {
        return 1;
    }

    struct tear_callback_context ctx = {
        .workload_name = workload_name,
        .run_id = run_id,
    };

    struct tear_callback_result result = {
        .exit_code = 1,
        .message = NULL,
    };

    const int rc = callback.run(&ctx, &result);

    fprintf(stderr,
            "[teard] callback finished: rc=%d exit_code=%d message=%s\n",
            rc,
            result.exit_code,
            result.message ? result.message : "");

    tear_backend_mock_commit_result(run_id, result.exit_code);
    tear_callback_unload(&callback);

    return rc == 0 ? result.exit_code : rc;
}
