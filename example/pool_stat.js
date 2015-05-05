var rados = require('../build/Release/rados');

var cluster = new rados.Rados( "ceph", "client.app", "/etc/ceph/ceph.conf");

var err = cluster.connect();
if (err) {
	console.log("Error " + err);
	throw err;
}

var ioctx = new rados.Ioctx(cluster, "app_wxb");
console.error(ioctx);
var stat = ioctx.pool_stat();

console.error(stat);

var objects = ioctx.objects_range(0, 3);

console.error(objects);