import os
import subprocess
import sys
import time

def parted_block(blk):
    dev = os.path.join('/dev', blk)
    cmd = 'parted -s %s mklabel gpt' % dev
    subprocess.check_output(cmd, shell=True)
    cmd = 'parted %s mkpart primary 0%% 100%%' % dev
    subprocess.check_output(cmd, shell=True)


def mkfs_xfs_part(blk):
    cmd = 'mkfs.xfs -f /dev/%s1' % blk
    subprocess.check_output(cmd, shell=True)


def mount_add_fstab(blk, point):
    cmd = 'lsblk -n -o uuid /dev/%s1' % blk
    uuid = subprocess.check_output(cmd, shell=True)
    uuid = uuid.strip()
    if not uuid:
        print('%s uuid is none!' % blk)
        sys.exit(1)
    # print(uuid)
    cmd = 'mkdir %s' % point
    subprocess.check_output(cmd, shell=True)
    line = 'UUID=%s %s                    xfs     defaults        0 0' % (uuid, point)
    cmd = 'echo %s >> /etc/fstab' % line
    subprocess.check_output(cmd, shell=True)
    cmd = 'mount %s' % point
    subprocess.check_output(cmd, shell=True)


def parted_mount_blk(blk, point):
    parted_block(blk)
    mkfs_xfs_part(blk)
    time.sleep(0.5)
    mount_add_fstab(blk, point)


def parted_mount_loop(st, ed, mnt_pre, index):
    pre = st[0:2]
    st_num = ord(st[-1])
    ed_num = ord(ed[-1])
    num = index
    for i in range(st_num, ed_num + 1):
        blk = pre + chr(i)
        point = '%s%d' % (mnt_pre, num)
        num += 1
        # print(blk, point)
        parted_mount_blk(blk, point)


if __name__ == '__main__':
    # parted_mount_blk('sdo', '/home/sata3')
    parted_mount_loop('sdp', 'sds', '/home/sata', 5)
