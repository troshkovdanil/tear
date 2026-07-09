#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define TEAR_SOCKET_PATH "/tmp/tear.sock"
#define TEAR_RESPONSE_MAX 128

static int usage(const char *argv0)
{
    fprintf(stderr, "usage: %s demo\n", argv0);
    return 1;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        return usage(argv[0]);
    }

    const char *workload_name = argv[1];

    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));

    addr.sun_family = AF_UNIX;
    snprintf(addr.sun_path, sizeof(addr.sun_path), "%s", TEAR_SOCKET_PATH);

    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
        perror("connect");
        close(fd);
        return 1;
    }

    dprintf(fd, "%s\n", workload_name);

    char response[TEAR_RESPONSE_MAX];
    memset(response, 0, sizeof(response));

    ssize_t n = read(fd, response, sizeof(response) - 1);
    if (n < 0) {
        perror("read");
        close(fd);
        return 1;
    }

    printf("%s", response);

    close(fd);
    return 0;
}
