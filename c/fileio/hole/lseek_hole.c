// gcc -D_GNU_SOURCE lseek_hole.c
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    int fd = 0;
    off_t len;
    off_t start, offset;
    if (argc < 2) {
        printf("Usage: %s <image_name>\n", argv[0]);
        return 1;
    }

    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror(argv[1]);
        return 1;
    }

    len = lseek(fd, 0, SEEK_END);
    // printf("%s length:%d\n", argv[1], len);

    start = offset = 0;
    while(1) {
        offset = lseek(fd, start, SEEK_DATA);
        if (offset < 0) {
            if (errno == ENXIO) {
                printf("HOLE %lu:%lu\n", start, len);
            }
            break;
        }
        if (offset > start) {
            printf("HOLE %lu:%lu\n", start, offset);
            start = offset;
            continue;
        }
        offset = lseek(fd, start, SEEK_HOLE);
        if (offset < 0) {
            break;
        }
        if (offset > start) {
            printf("DATA %lu:%lu\n", start, offset);
            start = offset;
        }
    }
    close(fd);
    return 0;
}
