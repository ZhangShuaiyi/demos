#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/uio.h>

int main(int argc, char *argv[]) {
    int fd = 1;
    ssize_t n;
    struct iovec iov[2];
    char *str0 = "hello ";
    char *str1 = "world!\n";
    char *fname = "test.txt";

    fd = open(fname, O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror(fname);
        return 1;
    }

    iov[0].iov_base = str0;
    iov[0].iov_len = strlen(str0);
    iov[1].iov_base = str1;
    iov[1].iov_len = strlen(str1);

    n = writev(fd, iov, 2);
    printf("%d written\n", n);
    return 0;
}
