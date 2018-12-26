import eventlet
eventlet.monkey_patch()

from oslo_config import cfg
from oslo_context import context
from oslo_log import log as logging
import oslo_messaging
import sys

LOG = logging.getLogger(__name__)
CONF = cfg.CONF
DOMAIN = "demo"


logging.register_options(CONF)
CONF(sys.argv[1:])

logging.setup(CONF, DOMAIN)

transport = oslo_messaging.get_rpc_transport(cfg.CONF)
target = oslo_messaging.Target(topic='foo', server='server1')
client = oslo_messaging.RPCClient(transport, target)
ctxt = context.RequestContext()
ret = client.call(ctxt, 'foo', arg=10)
LOG.info(ret)
