# https://github.com/torvalds/linux/blob/v4.9/Documentation/vm/pagemap.txt
import ctypes
import mmap
import os
import struct
import sys


# http://varx.org/wordpress/2016/02/03/bit-fields-in-python/
class PacketBits(ctypes.LittleEndianStructure):
    _fields_ = [
        ("pfn", ctypes.c_uint64, 55),
        ("soft_dirty", ctypes.c_uint64, 1),
        ("zero", ctypes.c_uint64, 5),
        ("file_page", ctypes.c_uint64, 1),
        ("swapped", ctypes.c_uint64, 1),
        ("present", ctypes.c_uint64, 1),
    ]
 

class Packet(ctypes.Union):
    _fields_ = [("bits", PacketBits),
                ("binary_data", ctypes.c_uint64)]


class PagemapEntry(object):
    def __init__(self, data, offset=0):
        packet = Packet()
        packet.binary_data = data
        for x in packet.bits._fields_:
            setattr(self, x[0], getattr(packet.bits, x[0]))
        self.paddr = self.pfn * mmap.PAGESIZE + offset


def get_pfn_kpagefd(fd, pfn=-1):
        if pfn >= 0:
            offset = pfn * 8
            fd.seek(offset)
        data = fd.read(8)
        return struct.unpack('<Q', data)[0]


def pagemap_get_entry(pagemap_fd, vaddr=-1):
    if (vaddr >= 0):
        vpn = vaddr / mmap.PAGESIZE
        offset = vpn * 8
        pagemap_fd.seek(offset)
    data = pagemap_fd.read(8)
    entry = PagemapEntry(struct.unpack("<Q", data)[0])
    return entry


def list_pagemap_phy_info(pid, start, end):
    pagemap_file = '/proc/%d/pagemap' % pid
    zeros = 0
    pages = 0
    zero_start = zero_end = 0
    zero_info = []
    count_fd = open('/proc/kpagecount', 'rb')
    flags_fd = open('/proc/kpageflags', 'rb')
    count_info = {}
    flags_info = {}
    with open(pagemap_file, 'rb') as f:
        vpn = start / mmap.PAGESIZE
        offset = vpn * 8
        f.seek(offset)
        for vaddr in range(start, end, mmap.PAGESIZE):
            entry = pagemap_get_entry(f)
            if entry.pfn != 0:
                pages += 1
                count = get_pfn_kpagefd(count_fd, entry.pfn)
                flags = get_pfn_kpagefd(flags_fd, entry.pfn)
                count_info.setdefault(count, 0)
                flags_info.setdefault(flags, 0)
                count_info[count] += 1
                flags_info[flags] += 1
                if (zero_start != 0 and zero_end != 0):
                    zero_info.append([zero_start, zero_end])
                    zero_start = 0
                    zero_end = 0
            else:
                zeros += 1
                if zero_start == 0:
                    zero_start = vaddr
                    zero_end = vaddr
                else:
                    zero_end = vaddr

    print('pages num:%d zeros num:%d' % (pages, zeros))
    '''
    print('zeros:')
    for info in zero_info:
        print('\t0x%x:0x%x' % (info[0], info[1]))
    '''
    print(count_info)
    print(flags_info)


def test():
    entry = PagemapEntry(0x860000000006b600L)
    print('0x%x' % entry.pfn)


if __name__ == '__main__':
    if (len(sys.argv) < 3):
        print('Usage:%s <pid> <vaddr_start> <vaddr_end>' % sys.argv[0])
        sys.exit(1)
    pid = int(sys.argv[1], 0)
    start = int(sys.argv[2], 0)
    end = start + mmap.PAGESIZE
    if (len(sys.argv) > 3):
        end = int(sys.argv[3], 0)
    
    # list_pagemap_detail(pid, start, end)
    list_pagemap_phy_info(pid, start, end)
