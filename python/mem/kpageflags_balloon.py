import pickle
import struct
import sys
import time


BALLOON = 1 << 23 


def get_pfn_kpagefd(fd, pfn=-1):
    if pfn >= 0:
        offset = pfn * 8
        fd.seek(offset)
    data = fd.read(8)
    return struct.unpack('<Q', data)[0]


def get_kpage_balloon():
    count_fd = open('/proc/kpagecount', 'rb')
    flags_fd = open('/proc/kpageflags', 'rb')
    balloons = set()
    count_info = {}
    flags_info = {}
    pfn = 0
    while True:
        try:
            count = get_pfn_kpagefd(count_fd)
            flags = get_pfn_kpagefd(flags_fd)
            if flags & BALLOON:
                balloons.add(pfn)
                count_info.setdefault(count, 0)
                flags_info.setdefault(flags, 0)
                count_info[count] += 1
                flags_info[flags] += 1
            pfn += 1
        except struct.error:
            break
    count_fd.close()
    flags_fd.close()
    print(count_info)
    print(flags_info)
    print(len(balloons))
    return balloons



def get_balloon_pfn():
    flags_fd = open('/proc/kpageflags', 'rb')
    balloons = set()
    pfn = 0
    while True:
        try:
            flags = get_pfn_kpagefd(flags_fd)
            if flags & BALLOON:
                balloons.add(pfn)
            pfn += 1
        except struct.error:
            break
    return balloons


def diff_balloons(times=60):
    balloon1 = get_balloon_pfn()
    print('ballon len:%d' % len(balloon1))
    time.sleep(60)
    balloon2 = get_balloon_pfn()
    print('ballon len:%d' % len(balloon2))
    print(balloon1 - balloon2)
    print(balloon2 - balloon1)



if __name__ == '__main__':
    balloon = get_kpage_balloon()
    if len(sys.argv) > 1:
        with open(sys.argv[1], 'w') as f:
            pickle.dump(balloon, f, pickle.HIGHEST_PROTOCOL)
