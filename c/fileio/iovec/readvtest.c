#include <stdio.h>
#include <fcntl.h>
#include <sys/uio.h>

int main(int argc, char *argv[]) {
    int fd = 0;
    ssize_t n;
    struct iovec iov[2];
    char str0[6] = {0};
    char str1[7] = {0};
    char *fname = "test.txt";

    fd = open(fname, O_RDWR);
    if (fd == -1) {
        perror(fname);
        return 1;
    }

    iov[0].iov_base = str0;
    iov[0].iov_len = sizeof(str0);
    iov[1].iov_base = str1;
    iov[1].iov_len = sizeof(str1);
    n = readv(fd, iov, 2);
    printf("%d read %s%s\n", n, str0, str1);
    return 0;
}
