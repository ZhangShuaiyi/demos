import libvirt
import eventlet

event_mapping = {
    libvirt.VIR_DOMAIN_EVENT_DEFINED:   "VIR_DOMAIN_EVENT_DEFINED",
    libvirt.VIR_DOMAIN_EVENT_UNDEFINED: "VIR_DOMAIN_EVENT_UNDEFINED",
    libvirt.VIR_DOMAIN_EVENT_STARTED:   "VIR_DOMAIN_EVENT_STARTED",
    libvirt.VIR_DOMAIN_EVENT_SUSPENDED: "VIR_DOMAIN_EVENT_SUSPENDED",
    libvirt.VIR_DOMAIN_EVENT_RESUMED:   "VIR_DOMAIN_EVENT_RESUMED",
    libvirt.VIR_DOMAIN_EVENT_STOPPED:   "VIR_DOMAIN_EVENT_STOPPED",
    libvirt.VIR_DOMAIN_EVENT_SHUTDOWN:  "VIR_DOMAIN_EVENT_SHUTDOWN",
    libvirt.VIR_DOMAIN_EVENT_PMSUSPENDED: "VIR_DOMAIN_EVENT_PMSUSPENDED",
}


def foo(conn, dom, event, detail, opaque):
    print('%s %d %s' % (dom.name(), event, event_mapping.get(event, '')))


def efunc(conn):
    conn.domainEventRegisterAny(None, libvirt.VIR_DOMAIN_EVENT_ID_LIFECYCLE, foo, None)
    while True:
        libvirt.virEventRunDefaultImpl()
        eventlet.sleep(0)


if __name__ == '__main__':
    libvirt.virEventRegisterDefaultImpl()
    conn = libvirt.open("qemu:///system")
    eventlet.spawn_n(efunc, conn)
    eventlet.sleep(60)
