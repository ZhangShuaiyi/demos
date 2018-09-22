#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    struct stat sb;

    if (argc != 2) {
        printf("Usage: %s <pathname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (stat(argv[1], &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    printf("I-node number:\t\t %ld\n", (long)sb.st_ino);
    printf("Link count:\t\t %ld\n", (long)sb.st_nlink);
    printf("block size:\t\t %ld bytes\n", (long)sb.st_blksize);
    printf("File size:\t\t %lld bytes\n", (long long)sb.st_size);
    printf("Blocks allocated:\t %lld\n", (long long)sb.st_blocks);

    exit(EXIT_SUCCESS);
}
