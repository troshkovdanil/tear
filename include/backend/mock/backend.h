#ifndef TEAR_BACKEND_MOCK_H
#define TEAR_BACKEND_MOCK_H

int tear_backend_mock_verify_workload(const char *workload_name);
int tear_backend_mock_commit_result(const char *run_id, int exit_code);

#endif
