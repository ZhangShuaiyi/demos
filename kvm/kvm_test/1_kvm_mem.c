#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/kvm.h>

uint8_t *get_codes(char *name, int *len) {
    FILE *fc;
    uint8_t *code = NULL;

    fc = fopen(name, "rb");
    fseek(fc, 0L, SEEK_END);
    *len = ftell(fc);
    fseek(fc, 0L, SEEK_SET);
    code = (uint8_t *)malloc(*len * sizeof(uint8_t));
    *len = fread(code, 1, *len, fc);
    fclose(fc);
    return code;
}

int main(int argc, char *argv[]) {
    int fd, vmfd, vcpufd;
    int nr_slots = 0;
    uint8_t *code = NULL;
    int code_len = 0;
    size_t mem_size = 0x1000;

    uint8_t *mem;
    int running = 1;
    struct kvm_sregs sregs;
    struct kvm_regs regs;
    size_t mmap_size;
    struct kvm_run *run;


    code = get_codes(argv[1], &code_len);

    fd = open("/dev/kvm", O_RDWR|O_CLOEXEC);
    nr_slots = ioctl(fd, KVM_CHECK_EXTENSION, KVM_CAP_NR_MEMSLOTS);
    printf("KVM_CAP_NR_MEMSLOTS nr_slots: %d\n", nr_slots);
    
    vmfd = ioctl(fd, KVM_CREATE_VM, (unsigned long)0);
    mem = mmap(NULL, mem_size, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    memcpy(mem, code, code_len);
    free(code);

    struct kvm_userspace_memory_region region= {
        .slot = 0,
        .guest_phys_addr= 0x0,
        .memory_size = mem_size,
        .userspace_addr = (uint64_t)mem,
    };
    ioctl(vmfd, KVM_SET_USER_MEMORY_REGION, &region);

    vcpufd = ioctl(vmfd, KVM_CREATE_VCPU, (unsigned long)0);
    mmap_size = ioctl(fd, KVM_GET_VCPU_MMAP_SIZE, NULL);
    printf("vcpufd(%d) mmap_size:%d\n", vcpufd, mmap_size);
    run = mmap(NULL, mmap_size, PROT_READ|PROT_WRITE, MAP_SHARED, vcpufd, 0);

    ioctl(vcpufd, KVM_GET_SREGS, &sregs);
    printf("KVM_GET_SREGS cs.base:0x%lx cs.selector:0x%x\n", sregs.cs.base, sregs.cs.selector);
    sregs.cs.base = 0;
    sregs.cs.selector = 0;
    ioctl(vcpufd, KVM_SET_SREGS, &sregs);

    ioctl(vcpufd, KVM_GET_REGS, &regs);
    printf("KVM_GET_REGS rip:0x%lx rflags:0x%lx\n", regs.rip, regs.rflags);
    regs.rip = 0x0;
    regs.rflags = 0x2;
    ioctl(vcpufd, KVM_SET_REGS, &regs);

    while(running) {
        ioctl(vcpufd, KVM_RUN, NULL);
        switch (run->exit_reason) {
        case KVM_EXIT_HLT:
            fprintf(stdout, "KVM_EXIT_HLT\n");
            running = 0;
            break;
        case KVM_EXIT_IO:
            if (run->io.direction==KVM_EXIT_IO_OUT && run->io.size==1 && run->io.port==0x3f8 && run->io.count==1) {
                putchar(*(((char *)run) + run->io.data_offset));
            } else {
                fprintf(stderr, "unhandled KVM_EXIT_IO\n");
                running = 0;
            }
            break;
        default:
            fprintf(stderr, "exit_reason=0x%x\n", run->exit_reason);
            running = 0;
        }
    }

    close(fd);
    return 0;
}
