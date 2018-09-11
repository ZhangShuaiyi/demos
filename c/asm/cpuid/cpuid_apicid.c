/**
 指定cpu运行: taskset -c 0 ./a.out

 intel-64-architecture-processor-topology-enumeration.pdf
 */
#include <stdio.h>
#include <sched.h>

static int smt_mask_width = 0;
static int smt_select_mask = 0;
static int coreplus_mask_width = 0;
static int coreonly_select_mask = 0;


// TODO
// x2APIC ID = CPUID.(EAX=1,ECX=0):EDX[31:0]
static inline int get_apic_id() {
    int op = 0x01;
    int ebx = 0;
    int apicid = 0;

    __asm__ volatile("cpuid"
                : "=b"(ebx)
                : "a"(op));
    // printf("0x%08x ebx:0x%08x\n", op, ebx);
    apicid = (ebx >> 24) & 0xff;
    return apicid;
}

static inline void run_eax_0b(int op_ecx, int *eax, int *ebx, int *ecx) {
    __asm__ volatile("cpuid"
                : "=a"(*eax), "=b"(*ebx), "=c"(*ecx)
                : "a"(0x0b), "c"(op_ecx));
    switch(op_ecx) {
    case 0:
        if (((*ecx >> 8) & 0xff) == 1) {
            smt_mask_width = *eax & 0xf;
            smt_select_mask = ~((-1) << smt_mask_width);
        }
        break;
    case 1:
        if (((*ecx >> 8) & 0xff) == 2) {
            coreplus_mask_width = *eax & 0xf;
            coreonly_select_mask = (~((-1)<< coreplus_mask_width)) ^ smt_select_mask;
        }
        break;
    // default:
    //    printf("0x%08x 0x%02x ignore\n", 0x0b, op_ecx);
    }
}

static inline void loop_eax_0b() {
    int op_ecx = 0;
    int eax = 0, ebx = 0, ecx = 0;

    // 循环调用cpuid指令，eax=0x0b，ecx++，直到返回的EBX[15:0] == 0
    for (op_ecx=0; op_ecx<16; op_ecx++) {
        run_eax_0b(op_ecx, &eax, &ebx, &ecx);
        if ((ebx & 0xffff) == 0) {
            break;
        }
        // printf("0x%08x 0x%02x ebx:0x%08x ecx:0x%08x\n", 0x0b, op_ecx, ebx, ecx);
    }
}

static inline int get_smt_id(int apicid) {
    return apicid & smt_select_mask;
}

static inline int get_core_id(int apicid) {
    return (apicid & coreonly_select_mask) >> smt_mask_width;
}

static inline int get_pkg_id(int apicid) {
    int pkg_select_mask = (-1) << coreplus_mask_width;
    return (apicid & pkg_select_mask) >> coreplus_mask_width;
}

int main(int argc, char *argv[]) {
    int apicid = 0;
    int cpuno = 0;

    cpuno = sched_getcpu();
    printf("current cpu number:%d\n", cpuno);

    apicid = get_apic_id();
    printf("apicid %d\n", apicid);
    loop_eax_0b();
    printf("pkg_id:%d\n", get_pkg_id(apicid));
    printf("core_id:%d\n", get_core_id(apicid));
    printf("smt_id:%d\n", get_smt_id(apicid));
    return 0;
}
