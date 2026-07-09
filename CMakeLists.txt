cmake_minimum_required(VERSION 3.16)

project(tear C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_compile_options(-Wall -Wextra -Werror)

include_directories(include)

add_library(tear-platform-linux STATIC
    platform/linux/platform.c
)

target_include_directories(tear-platform-linux PUBLIC include)

add_library(tear-backend-mock STATIC
    backend/mock/backend.c
)

target_include_directories(tear-backend-mock PUBLIC include)

add_library(tear-callback-demo-linux-mock SHARED
    workload/demo/callback.c
)

target_link_libraries(tear-callback-demo-linux-mock
    tear-platform-linux
    tear-backend-mock
)

set_target_properties(tear-callback-demo-linux-mock PROPERTIES
    OUTPUT_NAME "tear-callback-demo-linux-mock"
)

add_executable(teard
    runtime/teard.c
    runtime/execution_loop.c
    runtime/callback_loader.c
)

target_link_libraries(teard
    tear-platform-linux
    tear-backend-mock
    dl
)

add_executable(tearictl
    runtime/tearictl.c
)
