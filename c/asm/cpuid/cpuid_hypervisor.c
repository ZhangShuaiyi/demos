#include <stdio.h>

static inline int get_hypervisor_bit() {
    int op = 0x01;
    int ecx = 0;

    __asm__ volatile("cpuid"
                    : "=c"(ecx)
                    : "a"(op));
    return (ecx >> 31) & 0x1;
}

static inline void get_hypervisor_vendor(char *vendor) {
    int op = 0x40000000;
    int ebx = 0, ecx = 0, edx = 0;

    __asm__ volatile("cpuid"
                    : "=b"(ebx), "=c"(ecx), "=d"(edx)
                    : "a"(op));
    sprintf(vendor  , "%c%c%c%c", ebx, (ebx >> 8), (ebx >> 16), (ebx >> 24));
    sprintf(vendor+4, "%c%c%c%c", ecx, (ecx >> 8), (ecx >> 16), (ecx >> 24));
    sprintf(vendor+8, "%c%c%c%c", edx, (edx >> 8), (edx >> 16), (edx >> 24));
    vendor[12] = 0x00;
}

int main(int argc, char *argv[]) {
    int hv_bit = 0;
    char vendor[13] = {0x00};
    
    hv_bit = get_hypervisor_bit();
    printf("hypervisor bit:%d\n", hv_bit);
    if (hv_bit) {
        get_hypervisor_vendor(vendor);
        printf("hypervisor vendor:%s\n", vendor);
    } else {
        printf("Maybe in physical machine\n");
    }
    return 0;
}
