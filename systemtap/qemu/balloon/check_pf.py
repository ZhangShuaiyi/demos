import sys


def check_pf3(bal_file, pf_file):
    balloons = []
    faults = []
    # mask = 0xffffffff0000L
    mask = 0xffffffffe000L
    with open(bal_file, 'r') as f:
        balloons = f.read().splitlines()
    balloons = [(int(i, 0)&mask) for i in balloons]
    print('balloon length:%d' % len(balloons))
    for i in balloons[:3]:
        print('\t0x%lx' % i)
    sballoons = set(balloons)
    print('balloon set length:%d' % len(sballoons))
    with open(pf_file, 'r') as f:
        faults = f.read().splitlines()
    faults = [(int(i, 0)&mask) for i in faults]
    print('faults length:%d' % len(faults))
    for i in faults[:3]:
        print('\t0x%lx' % i)
    sfaults = set(faults)
    print('faults set length:%d' % len(sfaults))
    ret = sballoons & sfaults
    print(len(ret))


def check_pf2(bal_file, pf_file):
    balloons = []
    faults = []
    with open(bal_file, 'r') as f:
        balloons = f.read().splitlines()
    balloons = [int(i, 0) for i in balloons]
    print('balloon length:%d' % len(balloons))
    for i in balloons[:3]:
        print('\t0x%lx' % i)
    with open(pf_file, 'r') as f:
        faults = f.read().splitlines()
    faults = [(int(i, 0)<<12) for i in faults]
    print('faults length:%d' % len(faults))
    for i in faults[:3]:
        print('\t0x%lx' % i)
    ret = set(balloons) & set(faults)
    print(len(ret))


def check_pf(bal_file, pf_file):
    balloons = []
    faults = []
    with open(bal_file, 'r') as f:
        balloons = f.read().splitlines()
    balloons = [int(i, 0) for i in balloons]
    print('balloon length:%d' % len(balloons))
    for i in balloons[:3]:
        print('\t0x%lx' % i)
    with open(pf_file, 'r') as f:
        faults = f.read().splitlines()
    faults = [(int(i, 0)&0xfffffffff000) for i in faults]
    print('faults length:%d' % len(faults))
    for i in faults[:3]:
        print('\t0x%lx' % i)
    ret = set(balloons) & set(faults)
    print(len(ret))


if __name__ == '__main__':
    if len(sys.argv) < 3:
        print('Usage: %s <balloon_gpa> <page_fault_gpa>' % sys.argv[0])
    check_pf(sys.argv[1], sys.argv[2])
    check_pf2(sys.argv[1], sys.argv[2])
    check_pf3(sys.argv[1], sys.argv[2])
