#ifndef TEAR_RUNTIME_CALLBACK_LOADER_H
#define TEAR_RUNTIME_CALLBACK_LOADER_H

#include "workload/callback_api.h"

struct tear_loaded_callback {
    void *handle;
    tear_workload_run_fn run;
};

int tear_callback_load(const char *path, struct tear_loaded_callback *out);
void tear_callback_unload(struct tear_loaded_callback *callback);

#endif
