import eventlet
import time


eventlet.monkey_patch()

def lock_testf(c):
    print('===>lock_testf c:%s' % c)
    time.sleep(5)
    print('<===lock_testf c:%s' % c)


def testf(sem, func, c):
    r = sem.acquire(timeout=6)
    if r:
        func(c)
        sem.release()
    else:
        print('acquire failed %s' % c)


if __name__ == '__main__':
    num = 2
    print('semaphore nmu:%s' % num);
    sem = eventlet.semaphore.Semaphore(num)
    for i in xrange(1, 11):
        eventlet.spawn_n(testf, sem, lock_testf, i)
    time.sleep(30)
