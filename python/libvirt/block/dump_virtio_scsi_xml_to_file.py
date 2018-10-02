import subprocess

def dump_virtio_scsi(f, dev, i):
    s = '''<disk type='file' device='disk'>
  <driver name='qemu' type='raw' cache='none' io='native' discard='unmap'/>
  <source file='%s'/>
  <target dev='%s' bus='scsi'/>
</disk>'''
    s = s % (f, dev)
    name = '%d.xml' % i
    with open(name, 'w') as f:
        f.write(s)

def dump_loops():
    start = 1
    end = 9
    start_dev = 'c'
    num = 0
    for i in range(start, end+1):
        f = '/home/sata%d/sata.raw' % i
        dev = 'sd' + chr(ord(start_dev) + num)
        num += 1
        dump_virtio_scsi(f, dev, i)

if __name__ == '__main__':
    dump_loops()
