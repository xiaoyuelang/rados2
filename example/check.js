var fs = require('fs');
var rados = require('rados');

var cluster = new rados.Rados("ceph", "client.wxb", "/etc/ceph/ceph.conf");

var err = cluster.connect();
if (err) {
	console.log("Error " + err);
	throw err;
}

var ioctx = new rados.Ioctx(cluster, "wxb");

var LineByLineReader = require('line-by-line'), lr = new LineByLineReader('2013');

lr.on('error', function(err) {
	console.info('err', err);
});
lr.on('line', function(line) {
	var stat = ioctx.stat(line);
	if (stat) {
		
	}
	else {
		console.info('file not found', line)
	}
});

lr.on('end', function() {
	ioctx.destroy();
	cluster.shutdown();
});
