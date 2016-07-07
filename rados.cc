//#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <node_buffer.h>
#include "rados.h"
#include <stdlib.h>
#include </usr/include/rados/librados.h>

using namespace v8;
using namespace node;

Persistent<FunctionTemplate> Rados::constructor;
Rados::Rados() {};
Rados::~Rados() {};

Persistent<FunctionTemplate> Ioctx::constructor;
Ioctx::Ioctx() {};
Ioctx::~Ioctx() {};


void Rados::Init(Handle<Object> target) {
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
  tpl->SetClassName(Nan::New<v8::String>("Rados"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("connect"),
      Nan::New<FunctionTemplate>(connect)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("shutdown"),
      Nan::New<FunctionTemplate>(shutdown)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("get_fsid"),
      Nan::New<FunctionTemplate>(get_fsid)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("pool_create"),
      Nan::New<FunctionTemplate>(pool_create)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("pool_delete"),
      Nan::New<FunctionTemplate>(pool_delete)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("pool_list"),
      Nan::New<FunctionTemplate>(pool_list)->GetFunction());

  NanAssignPersistent(constructor, tpl);
  target->Set(Nan::New<v8::String>("Rados"),
      Nan::New<FunctionTemplate>(constructor)->GetFunction());
}


void Ioctx::Init(Handle<Object> target) {
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
  tpl->SetClassName(Nan::New<v8::String>("Ioctx"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("pool_stat"),
       Nan::New<FunctionTemplate>(pool_stat)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("pool_set_auid"),
      Nan::New<FunctionTemplate>(pool_set_auid)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("pool_get_auid"),
      Nan::New<FunctionTemplate>(pool_get_auid)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("destroy"),
      Nan::New<FunctionTemplate>(destroy)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("snap_create"),
      Nan::New<FunctionTemplate>(snap_create)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("snap_remove"),
      Nan::New<FunctionTemplate>(snap_remove)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("snap_rollback"),
      Nan::New<FunctionTemplate>(snap_rollback)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("read"),
      Nan::New<FunctionTemplate>(read)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("write"),
      Nan::New<FunctionTemplate>(write)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("write_full"),
      Nan::New<FunctionTemplate>(write_full)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("clone_range"),
      Nan::New<FunctionTemplate>(clone_range)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("append"),
      Nan::New<FunctionTemplate>(append)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("remove"),
      Nan::New<FunctionTemplate>(remove)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("trunc"),
      Nan::New<FunctionTemplate>(trunc)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("stat"),
      Nan::New<FunctionTemplate>(stat)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("getxattr"),
      Nan::New<FunctionTemplate>(getxattr)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("setxattr"),
      Nan::New<FunctionTemplate>(setxattr)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("rmxattr"),
      Nan::New<FunctionTemplate>(rmxattr)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("getxattrs"),
      Nan::New<FunctionTemplate>(getxattrs)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("aio_read"),
      Nan::New<FunctionTemplate>(aio_read)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("aio_write"),
      Nan::New<FunctionTemplate>(aio_write)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("aio_append"),
      Nan::New<FunctionTemplate>(aio_append)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("aio_write_full"),
      Nan::New<FunctionTemplate>(aio_write_full)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("aio_flush"),
      Nan::New<FunctionTemplate>(aio_flush)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("aio_flush_async"),
      Nan::New<FunctionTemplate>(aio_flush_async)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("objects_list"),
      Nan::New<FunctionTemplate>(objects_list)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("objects_range"),
        Nan::New<FunctionTemplate>(objects_range)->GetFunction());
  NanAssignPersistent(constructor, tpl);
  target->Set(Nan::New<v8::String>("Ioctx"),
      Nan::New<FunctionTemplate>(constructor)->GetFunction());
}

bool Rados::require_connected() {
  if ( this->state == STATE_CONNECTED ) {
    return true;
  } else {
    NanThrowError("Cluster is not in connected state");
    return false;
  }
}

bool Ioctx::require_created() {
  if ( this->rados->require_connected() ) {
    if ( this->state == STATE_CREATED ) {
      return true;
    } else {
      NanThrowError("Ioctx not in created state");
      return false;
    }
  } else {
    return false;
  }
}


NAN_METHOD(Rados::New) {
  NanScope();

  if (!args.IsConstructCall()) {
    return NanThrowError("Rados object must be instantiated with 'new' statement");
  }
  if (args.Length() < 3 ||
      !args[0]->Isv8::String() ||
      !args[1]->Isv8::String() ||
      !args[2]->Isv8::String()) {
    return NanThrowError("Bad argument.");
  }

  Rados* obj = new Rados();
  v8::String::Utf8Value cluster_name(args[0]);
  v8::String::Utf8Value user_name(args[1]);
  v8::String::Utf8Value conffile(args[2]);
  uint64_t flags = 0;

  if ( rados_create2(&obj->cluster, *cluster_name, *user_name, flags) != 0 ) {
    return NanThrowError("create rados cluster failed");
  }
  obj->state = STATE_CREATED;
  if ( rados_conf_read_file(obj->cluster, *conffile) != 0 ) {
    return NanThrowError("read conffile failed");
  }
  obj->state = STATE_CONFIGURED;

  obj->Wrap(args.This());
  NanReturnValue(args.This());
}


NAN_METHOD(Ioctx::New) {
  NanScope();

  if (!args.IsConstructCall()) {
    return NanThrowError("Ioctx object must be instantiated with 'new' statement");
  }
  if (args.Length() < 2 ||
      !args[1]->Isv8::String()) {
    return NanThrowError("Bad argument.");
  }

  Ioctx* obj = new Ioctx();
  Rados* cluster = ObjectWrap::Unwrap<Rados>(args[0]->ToObject());
  if ( !cluster->require_connected() ) NanReturnNull();
  v8::String::Utf8Value pool(args[1]);
  if ( rados_ioctx_create(cluster->cluster, *pool, &obj->ioctx) != 0 ) {
    return NanThrowError("create Ioctx failed");
  }
  obj->rados = cluster;
  obj->state = STATE_CREATED;

  obj->Wrap(args.This());
  NanReturnThis();
}


NAN_METHOD(Rados::connect) {
  NanScope();

  Rados* obj = ObjectWrap::Unwrap<Rados>(args.This());

  if ( obj->state != STATE_CONFIGURED )
    return NanThrowError("Cluster should be in configured state.");

  int err = rados_connect(obj->cluster);

  if (err == 0) {
    obj->state = STATE_CONNECTED;
  }

  NanReturnValue(Nan::New<Number>(-err));
}


NAN_METHOD(Rados::shutdown) {
  NanScope();

  Rados* obj = ObjectWrap::Unwrap<Rados>(args.This());
  if ( !obj->require_connected() ) NanReturnNull();

  rados_shutdown(obj->cluster);
  obj->state = STATE_DESTROYED;

  NanReturnNull();
}


NAN_METHOD(Rados::get_fsid) {
  NanScope();

  Rados* obj = ObjectWrap::Unwrap<Rados>(args.This());
  if ( !obj->require_connected() ) NanReturnNull();
  
  char fsid[37];
  if ( rados_cluster_fsid(obj->cluster, fsid, sizeof(fsid)) < 0) {
    NanReturnNull();
  }

  NanReturnValue(Nan::New<v8::String>(fsid));
}


NAN_METHOD(Rados::pool_create) {
  NanScope();

  if (args.Length() < 1 ||
      !args[0]->Isv8::String()) {
    return NanThrowError("Bad argument.");
  }

  Rados* obj = ObjectWrap::Unwrap<Rados>(args.This());
  if ( !obj->require_connected() ) NanReturnNull();
  
  v8::String::Utf8Value pool_name(args[0]);

  int err=0;
  switch (args.Length()) {
    case 1: {
      err = rados_pool_create(obj->cluster, *pool_name);
      break;
    }
    case 2: {
      if (args[1]->IsNumber()) {
        uint64_t auid = args[1]->IntegerValue();
        err = rados_pool_create_with_auid(obj->cluster, *pool_name, auid);
      } else {
        return NanThrowError("Bad argument.");
      }
      break;
    }
    case 3: {
      if (args[1]->IsNumber() && args[2]->IsNumber()) {
        uint64_t auid = args[1]->IntegerValue();
        uint8_t crush_rule = args[2]->Uint32Value();
        err = rados_pool_create_with_all(obj->cluster, *pool_name, auid, crush_rule);
      } else if (args[2]->IsNumber()) {
        uint8_t crush_rule = args[2]->Uint32Value();
        err = rados_pool_create_with_crush_rule(obj->cluster, *pool_name, crush_rule);
      } else {
        return NanThrowError("Bad argument.");
      }
      break;
    }
  }

  NanReturnValue(Nan::New<Number>(-err));
}


NAN_METHOD(Rados::pool_delete) {
  NanScope();

  if (args.Length() < 1 ||
      !args[0]->Isv8::String()) {
    return NanThrowError("Bad argument.");
  }

  Rados* obj = ObjectWrap::Unwrap<Rados>(args.This());
  if ( !obj->require_connected() ) NanReturnNull();
  v8::String::Utf8Value pool_name(args[0]);

  int err = rados_pool_delete(obj->cluster, *pool_name);

  NanReturnValue(Nan::New<Number>(-err));
}


NAN_METHOD(Rados::pool_list) {
  NanScope();

  Rados* obj = ObjectWrap::Unwrap<Rados>(args.This());
  if ( !obj->require_connected() ) NanReturnNull();
  
  char temp_buffer[256];
  int buff_size = rados_pool_list(obj->cluster, temp_buffer, 0);

  char buffer[buff_size];
  int r = rados_pool_list(obj->cluster, buffer, sizeof(buffer));
  if (r != buff_size) {
    NanReturnNull();
  }

  Local<Array> pools = Nan::New<Array>();
  const char *b = buffer;
  uint32_t array_id = 0;
  while (1) {
      if (*b == '\0') {
          break;
      }
      pools->Set(array_id, Nan::New<v8::String>(b));
      b += strlen(b) + 1;
      array_id++;
  }

  NanReturnValue(pools);
}

NAN_METHOD(Ioctx::pool_stat) {
  NanScope();
  Ioctx* obj = ObjectWrap::Unwrap<Ioctx>(args.This());
  if ( !obj->require_created() ) NanReturnNull();
  rados_pool_stat_t pool_stat_t;
  int err = rados_ioctx_pool_stat(obj->ioctx, &pool_stat_t);
  if (err < 0) {
    NanReturnNull();
  }

  Local<Object> stat = Nan::New<Object>();
  stat->Set( Nan::New<v8::String>("num_bytes"), Nan::New<Number>(pool_stat_t.num_bytes));
  stat->Set( Nan::New<v8::String>("num_objects"), Nan::New<Number>(pool_stat_t.num_objects) );

  NanReturnValue(stat);
}


NAN_METHOD(Ioctx::pool_set_auid) {
  NanScope();

  if (args.Length() < 1 ||
      args[1]->IsNumber()) {
    return NanThrowError("Bad argument.");
  }

  Ioctx* obj = ObjectWrap::Unwrap<Ioctx>(args.This());
  if ( !obj->require_created() ) NanReturnNull();
  uint64_t auid = args[0]->IntegerValue();

  int err = rados_ioctx_pool_set_auid(obj->ioctx, auid);

  NanReturnValue(Nan::New<Number>(-err));
}


NAN_METHOD(Ioctx::pool_get_auid) {
  NanScope();

  Ioctx* obj = ObjectWrap::Unwrap<Ioctx>(args.This());
  if ( !obj->require_created() ) NanReturnNull();

  uint64_t auid;
  int err = rados_ioctx_pool_get_auid(obj->ioctx, &auid);

  if (err < 0) {
    NanReturnValue(Nan::New<Number>(err));
  }

  NanReturnValue(Nan::New<Number>(auid));
}


NAN_METHOD(Ioctx::destroy) {
  NanScope();

  Ioctx* obj = ObjectWrap::Unwrap<Ioctx>(args.This());
  if ( !obj->require_created() ) NanReturnNull();

  rados_ioctx_destroy(obj->ioctx);
  obj->state = STATE_DESTROYED;

  NanReturnNull();
}


NAN_METHOD(Ioctx::snap_create) {
  NanScope();

  if (args.Length() < 1 ||
      !args[0]->Isv8::String()) {
    return NanThrowError("Bad argument.");
  }

  Ioctx* obj = ObjectWrap::Unwrap<Ioctx>(args.This());
  if ( !obj->require_created() ) NanReturnNull();
  v8::String::Utf8Value snapname(args[0]);

  int err = rados_ioctx_snap_create(obj->ioctx, *snapname);

  NanReturnValue(Nan::New<Number>(-err));
}


NAN_METHOD(Ioctx::snap_remove) {
  NanScope();

  if (args.Length() < 1 ||
      !args[0]->Isv8::String()) {
    return NanThrowError("Bad argument.");
  }

  Ioctx* obj = ObjectWrap::Unwrap<Ioctx>(args.This());
  if ( !obj->require_created() ) NanReturnNull();
  v8::String::Utf8Value snapname(args[0]);

  int err = rados_ioctx_snap_remove(obj->ioctx, *snapname);

  NanReturnValue(Nan::New<Number>(-err));
}


NAN_METHOD(Ioctx::snap_rollback) {
  NanScope();

  if (args.Length() < 2 ||
      !args[0]->Isv8::String() ||
      !args[0]->Isv8::String()) {
    return NanThrowError("Bad argument.");
  }

  Ioctx* obj = ObjectWrap::Unwrap<Ioctx>(args.This());
  if ( !obj->require_created() ) NanReturnNull();
  v8::String::Utf8Value oid(args[0]);
  v8::String::Utf8Value snapname(args[1]);

  int err = rados_ioctx_snap_rollback(obj->ioctx, *oid, *snapname);

  NanReturnValue(Nan::New<Number>(-err));
}


NAN_METHOD(Ioctx::read) {
  NanScope();

  if (args.Length() < 1 ||
      !args[0]->Isv8::String()) {
    return NanThrowError("Bad argument.");
  }

  Ioctx* obj = ObjectWrap::Unwrap<Ioctx>(args.This());
  if ( !obj->require_created() ) NanReturnNull();
  v8::String::Utf8Value oid(args[0]);
  size_t size = args[1]->IsNumber() ? args[1]->IntegerValue() : 8192;
  uint64_t offset = args[2]->IsNumber() ? args[2]->IntegerValue() : 0;

  char *buffer = new char[size];

  int err = rados_read(obj->ioctx, *oid, buffer, size, offset);

  if (err < 0) {
    delete buffer;
    NanReturnNull();
  } else {
    NanReturnValue(NanBufferUse(buffer, err));
  }

}


NAN_METHOD(Ioctx::write) {
  NanScope();

  if (args.Length() < 2 ||
      !args[0]->Isv8::String() ||
      !Buffer::HasInstance(args[1])) {
    return NanThrowError("Bad argument.");
  }

  Ioctx* obj = ObjectWrap::Unwrap<Ioctx>(args.This());
  if ( !obj->require_created() ) NanReturnNull();
  v8::String::Utf8Value oid(args[0]);
  char* buffer = Buffer::Data(args[1]);
  size_t size = args[2]->IsNumber() ? args[2]->Uint32Value() : Buffer::Length(args[1]);
  uint64_t offset = args[3]->IsNumber() ? args[3]->IntegerValue() : 0;

  int err = rados_write(obj->ioctx, *oid, buffer, size, offset);

  NanReturnValue(Nan::New<Number>(-err));
}


NAN_METHOD(Ioctx::write_full) {
  NanScope();

  if (args.Length() < 2 ||
      !args[0]->Isv8::String() ||
      !Buffer::HasInstance(args[1])) {
    return NanThrowError("Bad argument.");
  }

  Ioctx* obj = ObjectWrap::Unwrap<Ioctx>(args.This());
  if ( !obj->require_created() ) NanReturnNull();
  v8::String::Utf8Value oid(args[0]);
  char* buffer = Buffer::Data(args[1]);
  size_t size = args[2]->IsNumber() ? args[2]->Uint32Value() : Buffer::Length(args[1]);

  int err = rados_write_full(obj->ioctx, *oid, buffer, size);

  NanReturnValue(Nan::New<Number>(-err));
}


NAN_METHOD(Ioctx::clone_range) {
  NanScope();

  if (args.Length() < 5 ||
      !args[0]->Isv8::String() ||
      !args[1]->IsNumber() ||
      !args[2]->Isv8::String() ||
      !args[3]->IsNumber() ||
      !args[4]->IsNumber()) {
    return NanThrowError("Bad argument.");
  }

  Ioctx* obj = ObjectWrap::Unwrap<Ioctx>(args.This());
  if ( !obj->require_created() ) NanReturnNull();
  v8::String::Utf8Value dst(args[0]);
  uint64_t dst_off = args[1]->Uint32Value();
  v8::String::Utf8Value src(args[2]);
  uint64_t src_off = args[3]->Uint32Value();
  size_t size = args[4]->Uint32Value();

  int err = rados_clone_range(obj->ioctx, *dst, dst_off, *src, src_off, size);

  NanReturnValue(Nan::New<Number>(-err));
}


NAN_METHOD(Ioctx::append) {
  NanScope();

  if (args.Length() < 2 ||
      !args[0]->Isv8::String() ||
      !Buffer::HasInstance(args[1])) {
    return NanThrowError("Bad argument.");
  }

  Ioctx* obj = ObjectWrap::Unwrap<Ioctx>(args.This());
  if ( !obj->require_created() ) NanReturnNull();
  v8::String::Utf8Value oid(args[0]);
  char* buffer = Buffer::Data(args[1]);
  size_t size = args[2]->IsNumber() ? args[2]->Uint32Value() : Buffer::Length(args[1]);

  int err = rados_append(obj->ioctx, *oid, buffer, size);

  NanReturnValue(Nan::New<Number>(-err));
}


NAN_METHOD(Ioctx::remove) {
  NanScope();

  if (args.Length() != 1 ||
      !args[0]->Isv8::String()) {
    NanReturnNull();
  }

  Ioctx* obj = ObjectWrap::Unwrap<Ioctx>(args.This());
  if ( !obj->require_created() ) NanReturnNull();
  v8::String::Utf8Value oid(args[0]);

  int err = rados_remove(obj->ioctx, *oid);

  NanReturnValue(Nan::New<Number>(-err));
}


NAN_METHOD(Ioctx::trunc) {
  NanScope();

  if (args.Length() < 2 ||
      !args[0]->Isv8::String() ||
      !args[1]->IsNumber()) {
    return NanThrowError("Bad argument.");
  }

  Ioctx* obj = ObjectWrap::Unwrap<Ioctx>(args.This());
  if ( !obj->require_created() ) NanReturnNull();
  v8::String::Utf8Value oid(args[0]);
  size_t size = args[1]->Uint32Value();

  int err = rados_trunc(obj->ioctx, *oid, size);

  NanReturnValue(Nan::New<Number>(-err));
}


NAN_METHOD(Ioctx::getxattr) {
  NanScope();

  if (args.Length() < 2 ||
      !args[0]->Isv8::String() ||
      !args[1]->Isv8::String()) {
    return NanThrowError("Bad argument.");
  }

  Ioctx* obj = ObjectWrap::Unwrap<Ioctx>(args.This());
  if ( !obj->require_created() ) NanReturnNull();
  v8::String::Utf8Value oid(args[0]);
  v8::String::Utf8Value name(args[1]);
  size_t size;
  if (args[2]->IsNumber()) {
    size = args[2]->Uint32Value();
  } else {
    char temp_buffer[DEFAULT_BUFFER_SIZE];
    int ret = rados_getxattr(obj->ioctx, *oid, *name, temp_buffer, 0);
    if (ret < 0) {
      NanReturnNull();
    } else {
      size = ret;
    }
  }
  char buffer[size];

  int err = rados_getxattr(obj->ioctx, *oid, *name, buffer, size);

  if (err < 0) {
    NanReturnNull();
  }

  NanReturnValue(Nan::New<v8::String>(buffer, size));
}


NAN_METHOD(Ioctx::setxattr) {
  NanScope();

  if (args.Length() < 3 ||
      !args[0]->Isv8::String() ||
      !args[1]->Isv8::String() ||
      !args[2]->Isv8::String()) {
    return NanThrowError("Bad argument.");
  }

  Ioctx* obj = ObjectWrap::Unwrap<Ioctx>(args.This());
  if ( !obj->require_created() ) NanReturnNull();
  v8::String::Utf8Value oid(args[0]);
  v8::String::Utf8Value name(args[1]);
  v8::String::Utf8Value buffer(args[2]);
  size_t size = args[3]->IsNumber() ? args[3]->Uint32Value() : strlen(*buffer);

  int err = rados_setxattr(obj->ioctx, *oid, *name, *buffer, size);

  NanReturnValue(Nan::New<Number>(-err));
}


NAN_METHOD(Ioctx::rmxattr) {
  NanScope();

  if (args.Length() < 2 ||
      !args[0]->Isv8::String() ||
      !args[1]->Isv8::String()) {
    return NanThrowError("Bad argument.");
  }

  Ioctx* obj = ObjectWrap::Unwrap<Ioctx>(args.This());
  if ( !obj->require_created() ) NanReturnNull();
  v8::String::Utf8Value oid(args[0]);
  v8::String::Utf8Value name(args[1]);

  int err = rados_rmxattr(obj->ioctx, *oid, *name);

  NanReturnValue(Nan::New<Number>(-err));
}


NAN_METHOD(Ioctx::getxattrs) {
  NanScope();

  if (args.Length() < 1 ||
      !args[0]->Isv8::String()) {
    return NanThrowError("Bad argument.");
  }

  Ioctx* obj = ObjectWrap::Unwrap<Ioctx>(args.This());
  if ( !obj->require_created() ) NanReturnNull();
  v8::String::Utf8Value oid(args[0]);
  rados_xattrs_iter_t iter;

  Local<Object> xattrs = Nan::New<Object>();
  int err = rados_getxattrs(obj->ioctx, *oid, &iter);
  if (err < 0) {
    NanReturnNull();
  }
  while (1) {
      const char *name;
      const char *val;
      size_t len;

      err = rados_getxattrs_next(iter, &name, &val, &len);
      if (err < 0) {
        NanReturnNull();
      }
      if (name == NULL) {
          break;
      }

      xattrs->Set(Nan::New<v8::String>(name), Nan::New<v8::String>(val, len));
  }
  rados_getxattrs_end(iter);

  NanReturnValue(xattrs);
}


NAN_METHOD(Ioctx::stat) {
  NanScope();

  if (args.Length() != 1 ||
      !args[0]->Isv8::String()) {
    NanReturnNull();
  }

  Ioctx* obj = ObjectWrap::Unwrap<Ioctx>(args.This());
  if ( !obj->require_created() ) NanReturnNull();
  v8::String::Utf8Value oid(args[0]);
  uint64_t psize;
  time_t pmtime;

  int err = rados_stat(obj->ioctx, *oid, &psize, &pmtime);
  if (err < 0) {
    NanReturnNull();
  }

  Local<Object> stat = Nan::New<Object>();
  stat->Set( Nan::New<v8::String>("oid"), Nan::New<v8::String>(*oid) );
  stat->Set( Nan::New<v8::String>("psize"), Nan::New<Number>(psize) );
  stat->Set( Nan::New<v8::String>("pmtime"), Nan::New<Number>(pmtime) );

  NanReturnValue(stat);
}


void Ioctx::wait_complete(uv_work_t *req) {
  AsyncData* asyncdata = (AsyncData *)req->data;

  rados_aio_wait_for_complete(*asyncdata->comp);
  int ret = rados_aio_get_return_value(*asyncdata->comp);
  if (ret < 0) {
    asyncdata->err = -ret;
    asyncdata->size = 0;
  } else {
    asyncdata->size = ret;
  }

  rados_aio_release(*asyncdata->comp);
  delete asyncdata->comp;
}


void Ioctx::callback_complete(uv_work_t *req) {
  NanScope();

  AsyncData *asyncdata = (AsyncData *)req->data;

  if (asyncdata->cb_buffer) {
    const unsigned argc = 2;
    Local<Value> argv[argc] = {
      NanNull(),
      NanBufferUse(asyncdata->buffer, asyncdata->size) };
    if (asyncdata->err) argv[0] = Nan::New<Number>(asyncdata->err);
    asyncdata->callback.Call(argc, argv);
  }
  else {
    const unsigned argc = 1;
    Local<Value> argv[argc] = {
      NanNull() };
    if (asyncdata->err) argv[0] = Nan::New<Number>(asyncdata->err);
    asyncdata->callback.Call(argc, argv);
  }
  
  delete asyncdata;
}


NAN_METHOD(Ioctx::aio_read) {
  NanScope();

  if (args.Length() < 4 ||
      !args[0]->Isv8::String() ||
      !args[3]->IsFunction()) {
    return NanThrowError("Bad argument.");
  }

  Ioctx* obj = ObjectWrap::Unwrap<Ioctx>(args.This());
  if ( !obj->require_created() ) NanReturnNull();
  v8::String::Utf8Value oid(args[0]);
  size_t size = args[1]->IsNumber() ? args[1]->IntegerValue() : 8192;
  uint64_t offset = args[2]->IsNumber() ? args[2]->IntegerValue() : 0;

  AsyncData *asyncdata = new AsyncData;
  char *buffer = new char[size];
  rados_completion_t *comp = new rados_completion_t;

  asyncdata->callback.SetFunction(args[3].As<Function>());
  asyncdata->buffer = buffer;
  asyncdata->cb_buffer = true;
  asyncdata->size = size;
  asyncdata->comp = comp;
  asyncdata->err = 0;

  rados_aio_create_completion(NULL, NULL, NULL, comp);
  int err = rados_aio_read(obj->ioctx, *oid, *comp, buffer, size, offset);
  if (err < 0) {
    asyncdata->err = -err;
  }

  uv_work_t *req = new uv_work_t;
  req->data = asyncdata;

  uv_queue_work(
    uv_default_loop(),
    req,
    (uv_work_cb)wait_complete,
    (uv_after_work_cb)callback_complete
  );

  NanReturnUndefined();
}


NAN_METHOD(Ioctx::aio_write) {
  NanScope();

  if (args.Length() < 5 ||
      !args[0]->Isv8::String() ||
      !Buffer::HasInstance(args[1]) ||
      !args[4]->IsFunction()) {
    return NanThrowError("Bad argument.");
  }

  Ioctx* obj = ObjectWrap::Unwrap<Ioctx>(args.This());
  if ( !obj->require_created() ) NanReturnNull();
  v8::String::Utf8Value oid(args[0]);
  char* buffer = Buffer::Data(args[1]);
  size_t size = args[2]->IsNumber() ? args[2]->Uint32Value() : Buffer::Length(args[1]);
  uint64_t offset = args[3]->IsNumber() ? args[3]->IntegerValue() : 0;

  AsyncData *asyncdata = new AsyncData;
  rados_completion_t *comp = new rados_completion_t;

  asyncdata->callback.SetFunction(args[4].As<Function>());
  asyncdata->buffer = buffer;
  asyncdata->cb_buffer = false;
  asyncdata->size = size;
  asyncdata->comp = comp;
  asyncdata->err = 0;

  rados_aio_create_completion(NULL, NULL, NULL, comp);
  int err = rados_aio_write(obj->ioctx, *oid, *comp, buffer, size, offset);
  if (err < 0) {
    asyncdata->err = -err;
  }

  uv_work_t *req = new uv_work_t;
  req->data = asyncdata;

  uv_queue_work(
    uv_default_loop(),
    req,
    (uv_work_cb)wait_complete,
    (uv_after_work_cb)callback_complete
  );

  NanReturnUndefined();
}


NAN_METHOD(Ioctx::aio_append) {
  NanScope();

  if (args.Length() < 4 ||
      !args[0]->Isv8::String() ||
      !Buffer::HasInstance(args[1]) ||
      !args[3]->IsFunction()) {
    return NanThrowError("Bad argument.");
  }

  Ioctx* obj = ObjectWrap::Unwrap<Ioctx>(args.This());
  if ( !obj->require_created() ) NanReturnNull();
  v8::String::Utf8Value oid(args[0]);
  char* buffer = Buffer::Data(args[1]);
  size_t size = args[2]->IsNumber() ? args[2]->Uint32Value() : Buffer::Length(args[1]);

  AsyncData *asyncdata = new AsyncData;
  rados_completion_t *comp = new rados_completion_t;

  asyncdata->callback.SetFunction(args[3].As<Function>());
  asyncdata->buffer = buffer;
  asyncdata->cb_buffer = false;
  asyncdata->size = size;
  asyncdata->comp = comp;
  asyncdata->err = 0;

  rados_aio_create_completion(NULL, NULL, NULL, comp);
  int err = rados_aio_append(obj->ioctx, *oid, *comp, buffer, size);
  if (err < 0) {
    asyncdata->err = -err;
  }

  uv_work_t *req = new uv_work_t;
  req->data = asyncdata;

  uv_queue_work(
    uv_default_loop(),
    req,
    (uv_work_cb)wait_complete,
    (uv_after_work_cb)callback_complete
  );

  NanReturnUndefined();
}


NAN_METHOD(Ioctx::aio_write_full) {
  NanScope();

  if (args.Length() < 4 ||
      !args[0]->Isv8::String() ||
      !Buffer::HasInstance(args[1]) ||
      !args[3]->IsFunction()) {
    return NanThrowError("Bad argument.");
  }

  Ioctx* obj = ObjectWrap::Unwrap<Ioctx>(args.This());
  if ( !obj->require_created() ) NanReturnNull();
  v8::String::Utf8Value oid(args[0]);
  char* buffer = Buffer::Data(args[1]);
  size_t size = args[2]->IsNumber() ? args[2]->Uint32Value() : Buffer::Length(args[1]);

  AsyncData *asyncdata = new AsyncData;
  rados_completion_t *comp = new rados_completion_t;

  asyncdata->callback.SetFunction(args[3].As<Function>());
  asyncdata->buffer = buffer;
  asyncdata->cb_buffer = false;
  asyncdata->size = size;
  asyncdata->comp = comp;
  asyncdata->err = 0;

  rados_aio_create_completion(NULL, NULL, NULL, comp);
  int err = rados_aio_write_full(obj->ioctx, *oid, *comp, buffer, size);
  if (err < 0) {
    asyncdata->err = -err;
  }

  uv_work_t *req = new uv_work_t;
  req->data = asyncdata;

  uv_queue_work(
    uv_default_loop(),
    req,
    (uv_work_cb)wait_complete,
    (uv_after_work_cb)callback_complete
  );

  NanReturnUndefined();
}

NAN_METHOD(Ioctx::aio_flush) {
  NanScope();

  Ioctx* obj = ObjectWrap::Unwrap<Ioctx>(args.This());
  if ( !obj->require_created() ) NanReturnNull();
  
  int err = rados_aio_flush(obj->ioctx);

  NanReturnValue(Nan::New<Number>(-err));
}


NAN_METHOD(Ioctx::aio_flush_async) {
  NanScope();

  if (args.Length() < 1 ||
      !args[0]->IsFunction()) {
    return NanThrowError("Bad argument.");
  }

  Ioctx* obj = ObjectWrap::Unwrap<Ioctx>(args.This());
  if ( !obj->require_created() ) NanReturnNull();

  AsyncData *asyncdata = new AsyncData;
  rados_completion_t *comp = new rados_completion_t;

  asyncdata->callback.SetFunction(args[0].As<Function>());
  asyncdata->cb_buffer = false;
  asyncdata->comp = comp;
  asyncdata->err = 0;

  rados_aio_create_completion(NULL, NULL, NULL, comp);
  int err = rados_aio_flush_async(obj->ioctx, *comp);
  if (err < 0) {
    asyncdata->err = -err;
  }

  uv_work_t *req = new uv_work_t;
  req->data = asyncdata;

  uv_queue_work(
    uv_default_loop(),
    req,
    (uv_work_cb)wait_complete,
    (uv_after_work_cb)callback_complete
  );

  NanReturnUndefined();
}

#define ENOENT 2
NAN_METHOD(Ioctx::objects_list) {
  NanScope();

  Ioctx* obj = ObjectWrap::Unwrap<Ioctx>(args.This());
  if ( !obj->require_created() ) NanReturnNull();

  rados_list_ctx_t h_ctx;
  //Start listing objects in a pool.
  int err = rados_objects_list_open(obj->ioctx, &h_ctx);
  if (err < 0) {
    return NanThrowError("open list failed.");
  }

  Local<Array> ret_list = Nan::New<Array>();
  uint32_t array_id = 0;
  //Get the next object name and locator in the pool.

  while(0 <= err) {
    const char *obj_name;
    err = rados_objects_list_next(h_ctx, &obj_name, NULL);
    if (err == 0) {
      ret_list->Set(array_id, Nan::New(obj_name));
      array_id++;
    }
  }
  rados_objects_list_close(h_ctx);

  if (err < 0 && err != -ENOENT) {
    return NanThrowError("list_next failed.");
  }

  NanReturnValue(ret_list);
}

NAN_METHOD(Ioctx::objects_range) {
  NanScope();

  if (args.Length() < 2 ||
        !args[0]->IsNumber() ||
		!args[1]->IsNumber()
       ) {
      return NanThrowError("Bad argument.");
    }

  uint32_t offset = args[0]->IsNumber() ? args[0]->IntegerValue() : 0;
  uint32_t limit = args[1]->IsNumber() ? args[1]->IntegerValue() : 25;

  Ioctx* obj = ObjectWrap::Unwrap<Ioctx>(args.This());
  if ( !obj->require_created() ) NanReturnNull();

  rados_list_ctx_t h_ctx;

  int err = rados_objects_list_open(obj->ioctx, &h_ctx);
  if (err < 0) {
    return NanThrowError("open list failed.");
  }

  err = rados_objects_list_seek(h_ctx, offset);
  if (err < 0) {
      return NanThrowError("seek list failed.");
  }
  Local<Array> ret_list = Nan::New<Array>();
  uint32_t array_id = 0;

  while(0 <= err && array_id < limit) {
    const char *obj_name;
    err = rados_objects_list_next(h_ctx, &obj_name, NULL);
    if (err == 0) {
      ret_list->Set(array_id, Nan::New(obj_name));
      array_id++;
    }
  }
  rados_objects_list_close(h_ctx);

  if (err < 0 && err != -ENOENT) {
    return NanThrowError("list_next failed.");
  }

  NanReturnValue(ret_list);
}
