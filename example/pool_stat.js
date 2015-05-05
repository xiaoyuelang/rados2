var rados = require('./build/Release/rados');

var cluster = new rados.Rados( "ceph", "client.app", "/etc/ceph/ceph.conf");

var err = cluster.connect();
if (err) {
	console.log("Error " + err);
	throw err;
}

var ioctx = new rados.Ioctx(cluster, "app_wxb");

var stat = ioctx.pool_stat();

console.error(stat);