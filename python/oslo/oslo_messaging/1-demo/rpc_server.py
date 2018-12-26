import eventlet
eventlet.monkey_patch()

from oslo_config import cfg
from oslo_context import context
from oslo_log import log as logging
import oslo_messaging
import time

LOG = logging.getLogger(__name__)
CONF = cfg.CONF
DOMAIN = "demo"

logging.register_options(CONF)
logging.setup(CONF, DOMAIN)

class FooEndpoint(object):

    def foo(self, ctx, arg):
        LOG.info(arg)
        return arg*2

transport = oslo_messaging.get_rpc_transport(cfg.CONF)
target = oslo_messaging.Target(topic='foo', server='server1')
endpoints = [FooEndpoint()]
server = oslo_messaging.get_rpc_server(transport, target, endpoints,
                                        executor='eventlet')

LOG.info("Starting server")
try:
    server.start()
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    LOG.info("Stopping server")

server.stop()
server.wait()
