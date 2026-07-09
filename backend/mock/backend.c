#include "backend/mock/backend.h"

#include <stdio.h>
#include <string.h>

int tear_backend_mock_verify_workload(const char *workload_name)
{
    if (!workload_name) {
        return -1;
    }

    if (strcmp(workload_name, "demo") != 0) {
        fprintf(stderr, "[backend:mock] rejected unknown workload: %s\n", workload_name);
        return -1;
    }

    fprintf(stderr, "[backend:mock] workload verified: %s\n", workload_name);
    return 0;
}

int tear_backend_mock_commit_result(const char *run_id, int exit_code)
{
    if (!run_id) {
        return -1;
    }

    fprintf(stderr, "[backend:mock] committed result: run_id=%s exit_code=%d\n",
            run_id,
            exit_code);

    return 0;
}
