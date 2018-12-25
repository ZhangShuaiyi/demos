import struct


def get_pfn_kpagefd(fd, pfn=-1):
    if pfn >= 0:
        offset = pfn * 8
        fd.seek(offset)
    data = fd.read(8)
    return struct.unpack('<Q', data)[0]


def get_kpage_info():
    count_fd = open('/proc/kpagecount', 'rb')
    flags_fd = open('/proc/kpageflags', 'rb')
    count_info = {}
    flags_info = {}
    pfn = 0
    while True:
        try:
            count = get_pfn_kpagefd(count_fd)
            flags = get_pfn_kpagefd(flags_fd)
            count_info.setdefault(count, 0)
            flags_info.setdefault(flags, 0)
            count_info[count] += 1
            flags_info[flags] += 1
            pfn += 1
        except struct.error:
            break
    count_fd.close()
    flags_fd.close()
    print('last pfn:0x%x' % pfn)
    return count_info, flags_info


if __name__ == '__main__':
    count_info, flags_info = get_kpage_info()
    for k, v in count_info.items():
        print('%d pfns count:%d' % (v, k))
    for k,v in flags_info.items():
        print('%d pfns flag:0x%x' % (v, k))
