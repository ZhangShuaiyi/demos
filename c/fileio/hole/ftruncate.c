#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define FILE    "test.txt"

int main(int argc, char *argv[]) {
    int fd = 0;
    int ret = 0;
    int length = 1024;

    if (argc == 2) {
        length = atoi(argv[1]);
    }

    fd = open(FILE, O_CREAT | O_WRONLY | O_TRUNC, 0666);
    if (fd == -1) {
        perror("open");
        return errno;
    }

    ret = ftruncate(fd, length);
    if (ret != 0) {
        perror("ftruncate");
    }
    
    close(fd);
    return ret;
}
