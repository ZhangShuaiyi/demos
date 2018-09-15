#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <libaio.h>


#define MAX_EVENTS  128
#define NR          1
#define SIZE        512
#define FILE        "test.txt"

int main(int argc, char *argv[]) {
    int fd = 0;
    int ret = 1;
    io_context_t ctx;
    struct iocb cb;
    struct iocb *cbs[NR];
    struct io_event events[NR];
    char *s = "direct pwrite\n";
    void *buf;

    fd = open(FILE, O_DIRECT | O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd == -1) {
        perror(FILE);
        return ret;
    }

    ctx = 0;
    ret = io_setup(MAX_EVENTS, &ctx);
    if (ret != 0) {
        printf("io_setup error %d\n", ret);
        goto exit;
    }

    // O_DIRECT need to use posix_memalign.
    // If not events[0].res = -22
    ret = posix_memalign(&buf, 512, SIZE);
    if (ret != 0) {
        perror("posix_memalign");
        goto aio_exit;
    }
    memcpy(buf, s, strlen(s));

    io_prep_pwrite(&cb, fd, buf, SIZE, 0);
    cbs[0] = &cb;

    ret = io_submit(ctx, NR, cbs);
    if (ret != NR) {
        printf("io_submit error %d\n", ret);
        goto aio_exit;
    }

    ret = io_getevents(ctx, NR, NR, events, NULL);
    printf("io_getevents ret:%d data size:%d res:%ld\n", ret, strlen(s), events[0].res);

    free(buf);
aio_exit:
    io_destroy(ctx);
exit:
    close(fd);
    return ret;
}
