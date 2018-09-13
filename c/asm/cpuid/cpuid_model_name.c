/**
 80000002H
 80000003H
 80000004H
 EAX    Processor Brand String.
 EBX    Brand String Continued.
 ECX    Brand String Continued.
 EDX    Brand String Continued.
 */
#include <stdio.h>

static inline void get_model_name(char *model_name) {
    int eax = 0, ebx = 0, ecx = 0, edx = 0;
    // int op = 0x80000002;
    int i = 0;
    
    for (i = 0; i < 3; i++) {
        __asm__ volatile("cpuid"
                : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                : "a"(0x80000002 + i));
        sprintf(model_name + i*16, "%c%c%c%c", eax, (eax >> 8), (eax >> 16), (eax >> 24));
        sprintf(model_name + i*16 + 4, "%c%c%c%c", ebx, (ebx >> 8), (ebx >> 16), (ebx >> 24));
        sprintf(model_name + i*16 + 8, "%c%c%c%c", ecx, (ecx >> 8), (ecx >> 16), (ecx >> 24));
        sprintf(model_name + i*16 + 12, "%c%c%c%c", edx, (edx >> 8), (edx >> 16), (edx >> 24));
    }
    model_name[48] = 0x00;
}

int main(int argc, char *argv[]) {
    char model_name[49] = {0x00};

    get_model_name(model_name);
    printf("model name:%s\n", model_name);
}
