/**
EAX=0: Get vendor ID
his returns the CPU's manufacturer ID string – a twelve-character ASCII string stored in EBX, EDX, ECX (in that order). 
*/
#include <stdio.h>

static inline void get_manufacturer(char *manu) {
    int op = 0x0;
    int ebx = 0, ecx = 0, edx = 0;
    __asm__ volatile("cpuid" 
                : "=b"(ebx), "=c"(ecx), "=d"(edx)
                : "a"(op));
    printf("0x%08x ebx:0x%x ecx:0x%x edx:0x%x\n", op, ebx, ecx, edx);
    sprintf(manu, "%c%c%c%c", ebx, (ebx >> 8), (ebx >> 16), (ebx >> 24));
    sprintf(manu+4, "%c%c%c%c", edx, (edx >> 8), (edx >> 16), (edx >> 24));
    sprintf(manu+8, "%c%c%c%c", ecx, (ecx >> 8), (ecx >> 16), (ecx >> 24));
    manu[12] = 0x00;
}

int main(int argc, char *argv[]) {
    char manufacturer[13] = {0x0};
    get_manufacturer(manufacturer);
    printf("manufacturer:%s\n", manufacturer);
    return 0;
}
