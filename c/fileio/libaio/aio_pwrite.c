// gcc aio_pwrite.c -laio
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <libaio.h>


#define MAX_EVENTS  128
#define FILE        "test.txt"


int main(int argc, char *argv[]) {
    int fd = 0;
    int ret = 0;
    char *s = "Hello aio_pwrite\n";
    io_context_t ctx;
    struct iocb cb;
    struct iocb *cbs[1];
    struct io_event events[1];
    struct timespec timeout;

    fd = open(FILE, O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    memset(&ctx, 0, sizeof(ctx));
    ret = io_setup(MAX_EVENTS, &ctx);
    if (ret != 0) {
        printf("io_setup ret:%d\n", ret);
        return 1;
    }

    io_prep_pwrite(&cb, fd, s, strlen(s), 0);
    cbs[0] = &cb;
    if (io_submit(ctx, 1, cbs) != 1) {
        printf("io_submit error!\n");
        io_destroy(ctx);
        return 1;
    }

    while (1) {
        timeout.tv_sec = 0;
        timeout.tv_nsec = 500000000; // 0.5s
        if (io_getevents(ctx, 0, 1, events, &timeout) == 1) {
            break;
        }
        printf("not done yet\n");
        sleep(1);
    }

    printf("Write end!\n");
    close(fd);
    io_destroy(ctx);
    return 0;
}
