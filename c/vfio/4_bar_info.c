#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <endian.h>
#include <linux/vfio.h>
#include <linux/pci_regs.h>

int main(int argc, char *argv[]) {
    int group = 0, container = 0, device = 0;
    int ret, i;
    unsigned int pci_bar;
    unsigned long config_offset;
    struct vfio_device_info device_info = { .argsz = sizeof(device_info) };
    struct vfio_region_info reg = { .argsz = sizeof(reg) };
    
    if (argc < 3) {
        printf("Usage: %s /dev/vfio/<group> xxxx:xx:xx:x\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    group = open(argv[1], O_RDWR);
    if (group < 0) {
        perror(argv[1]);
        exit(EXIT_FAILURE);
    }

    container = open("/dev/vfio/vfio", O_RDWR);
    if (container < 0) {
        perror("failed to open /dev/vfio/vfio");
        goto fail_group;
    }

    ioctl(group, VFIO_GROUP_SET_CONTAINER, &container);
    ioctl(container, VFIO_SET_IOMMU, VFIO_TYPE1_IOMMU);
    device = ioctl(group, VFIO_GROUP_GET_DEVICE_FD, argv[2]);
    ioctl(device, VFIO_DEVICE_GET_INFO, &device_info);
    printf("device_info {num_irqs:%d, num_regions:%d}\n", device_info.num_irqs, device_info.num_regions);

    reg.index = VFIO_PCI_CONFIG_REGION_INDEX;
    ioctl(device, VFIO_DEVICE_GET_REGION_INFO, &reg);
    printf("config region info {argsz:%d, flags:0x%x, cap_offset:%d size:%ld offset:0x%lx}\n",
        reg.argsz, reg.flags, reg.cap_offset, reg.size, reg.offset);

    config_offset = reg.offset;
    for (i = 0; i < 6; i++) {
        char ioport, mem64;
        ret = pread(device, &pci_bar, sizeof(pci_bar), config_offset + PCI_BASE_ADDRESS_0 + (4 * i));
        pci_bar = le32toh(pci_bar);
        ioport = pci_bar & PCI_BASE_ADDRESS_SPACE_IO;
        mem64 = ioport ? 0 : (pci_bar & PCI_BASE_ADDRESS_MEM_TYPE_64);
        printf("%d bar:0x%x ioport:0x%x mem64:0x%x\n", i, pci_bar, ioport, mem64);
    }

    close(container);

fail_group:
    close(group);
    exit(EXIT_SUCCESS);
}
