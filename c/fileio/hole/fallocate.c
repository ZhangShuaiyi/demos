#include <stdio.h>
#include <fcntl.h>

#define FILE    "test.txt"

int main(int argc, char *argv[]) {
    int fd = 0;
    int ret = 0;
    int length = 2048;

    if (argc == 2) {
        length = atoi(argv[1]);
        if (length <= 0) {
            printf("Usage: %s <length>\n", argv[0]);
            return 1;
        }
    }

    fd = open(FILE, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    ret = posix_fallocate(fd, 0, length);
    if (ret != 0) {
        perror("posix_fallocate");
    }

    close(fd);
    return 0;
}

