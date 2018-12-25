import eventlet


def foo(evt, b):
    for i in xrange(1, 10):
        evt.send(b + i)
        evt.reset()
        eventlet.sleep(1)


if __name__ == '__main__':
    evt = eventlet.event.Event()
    eventlet.spawn_n(foo, evt, 3)
    for i in xrange(5):
        print(evt.wait())
