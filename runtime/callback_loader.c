#include "runtime/callback_loader.h"

#include <dlfcn.h>
#include <stdio.h>

int tear_callback_load(const char *path, struct tear_loaded_callback *out)
{
    if (!path || !out) {
        return -1;
    }

    out->handle = NULL;
    out->run = NULL;

    void *handle = dlopen(path, RTLD_NOW);
    if (!handle) {
        fprintf(stderr, "[callback_loader] dlopen failed: %s\n", dlerror());
        return -1;
    }

    dlerror();

    tear_workload_run_fn run =
        (tear_workload_run_fn)dlsym(handle, "tear_workload_run");

    const char *err = dlerror();
    if (err) {
        fprintf(stderr, "[callback_loader] dlsym failed: %s\n", err);
        dlclose(handle);
        return -1;
    }

    out->handle = handle;
    out->run = run;

    return 0;
}

void tear_callback_unload(struct tear_loaded_callback *callback)
{
    if (!callback || !callback->handle) {
        return;
    }

    dlclose(callback->handle);
    callback->handle = NULL;
    callback->run = NULL;
}
