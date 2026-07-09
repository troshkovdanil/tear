#include "platform/linux/platform.h"
#include "runtime/execution_loop.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define TEAR_SOCKET_PATH "/tmp/tear.sock"
#define TEAR_REQUEST_MAX 128
#define TEAR_RESPONSE_MAX 128

static int create_server_socket(void)
{
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        return -1;
    }

    unlink(TEAR_SOCKET_PATH);

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));

    addr.sun_family = AF_UNIX;
    snprintf(addr.sun_path, sizeof(addr.sun_path), "%s", TEAR_SOCKET_PATH);

    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
        perror("bind");
        close(fd);
        return -1;
    }

    if (listen(fd, 8) != 0) {
        perror("listen");
        close(fd);
        unlink(TEAR_SOCKET_PATH);
        return -1;
    }

    return fd;
}

static void handle_client(int client_fd)
{
    char request[TEAR_REQUEST_MAX];
    memset(request, 0, sizeof(request));

    ssize_t n = read(client_fd, request, sizeof(request) - 1);
    if (n <= 0) {
        return;
    }

    request[strcspn(request, "\r\n")] = '\0';

    fprintf(stderr, "[teard] request: %s\n", request);

    int exit_code = tear_execution_loop_run(request);

    char response[TEAR_RESPONSE_MAX];
    snprintf(response, sizeof(response), "exit_code=%d\n", exit_code);

    (void)write(client_fd, response, strlen(response));
}

int main(void)
{
    if (tear_platform_setup_signal_handlers() != 0) {
        perror("signal setup");
        return 1;
    }

    int server_fd = create_server_socket();
    if (server_fd < 0) {
        return 1;
    }

    fprintf(stderr, "[teard] listening on %s\n", TEAR_SOCKET_PATH);

    while (!tear_platform_should_stop()) {
        int client_fd = accept(server_fd, NULL, NULL);
        if (client_fd < 0) {
            if (errno == EINTR && tear_platform_should_stop()) {
                break;
            }

            if (errno == EINTR) {
                continue;
            }

            perror("accept");
            break;
        }

        handle_client(client_fd);
        close(client_fd);
    }

    fprintf(stderr, "[teard] stopping\n");

    close(server_fd);
    unlink(TEAR_SOCKET_PATH);

    return 0;
}
