# TEAR

**Trusted Edge Adaptive Runtime**

TEAR is an experimental runtime for trusted, adaptive workload execution on edge devices.

The core idea is simple: a workload should not run as an unstructured binary with hidden side effects. Instead, TEAR runs workloads through a small execution loop where platform access, backend trust decisions, callback execution, and result commitment are explicit parts of the runtime flow.

This repository starts with the smallest useful vertical slice:

```text
tearictl -> teard -> execution loop -> callback -> result commit
```

The first target is a Linux host demo using:

- a dynamic demo workload callback;
- a Linux platform abstraction;
- a mock backend;
- a runtime daemon;
- a small CLI client.

No manifests, profiles, OP-TEE, Zephyr, or policy engine are used yet. Those pieces will be added only after the basic execution model is working and easy to reason about.

## Current demo

The current demo builds:

- `teard` — runtime daemon;
- `tearictl` — client used to request execution;
- `libtear-callback-demo-linux-mock.so` — dynamic demo workload callback;
- `libtear-platform-linux.a` — Linux platform abstraction;
- `libtear-backend-mock.a` — mock trust backend.

## Repository structure

```text
backend/
  mock/
    backend.c
  optee/              # later
    backend.c

platform/
  linux/
    platform.c
  zephyr/             # later

runtime/
  callback_loader.c
  execution_loop.c
  tearictl.c
  teard.c

workload/
  demo/
    callback.c

include/
  backend/
    mock/
      backend.h
  platform/
    linux/
      platform.h
  runtime/
    callback_loader.h
    execution_loop.h
  workload/
    callback_api.h
```

## Build

```bash
cmake -S . -B build
cmake --build build
```

## Run

Start the daemon:

```bash
./build/bin/teard
```

In another terminal, request the demo workload:

```bash
./build/bin/tearictl demo
```

Expected result:

```text
exit_code=0
```

Stop the daemon with `Ctrl+C`.

## Design direction

TEAR is moving toward a runtime model where:

1. the platform provides explicit host/device services;
2. the backend verifies and commits trusted execution state;
3. workloads expose callbacks through a stable callback API;
4. the runtime execution loop controls transitions instead of giving the workload full control;
5. future profiles/manifests describe what is allowed to run and how it may adapt.

The Linux/mock/demo path is intentionally minimal. It is the baseline used to validate the runtime shape before adding OP-TEE, Zephyr, signed manifests, profiles, trusted history, revocation, and adaptive optimization.
