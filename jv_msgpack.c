#include <msgpack.h>

#include "jv.h"

jv msgpack_to_jv(msgpack_object obj);

static jv msgpack_array_to_jv(msgpack_object_array array) {
  jv a = jv_array();
  size_t i;
  for (i = 0; i < array.size; i++) {
    a = jv_array_append(a, msgpack_to_jv(array.ptr[i]));
  }
  return a;
}

static jv msgpack_map_to_jv(msgpack_object_map map) {
  jv o = jv_object();
  size_t i;
  for (i = 0; i < map.size; i++) {
    o = jv_object_set(o, msgpack_to_jv(map.ptr[i].key), msgpack_to_jv(map.ptr[i].val));
  }
  return o;
}

jv msgpack_to_jv(msgpack_object obj) {
  switch (obj.type) {
  case MSGPACK_OBJECT_NIL:
    return jv_null();
  case MSGPACK_OBJECT_BOOLEAN:
    if (obj.via.boolean) return jv_true();
    else return jv_false();
  case MSGPACK_OBJECT_POSITIVE_INTEGER:
    return jv_number(obj.via.u64);
  case MSGPACK_OBJECT_NEGATIVE_INTEGER:
    return jv_number(obj.via.i64);
  case MSGPACK_OBJECT_DOUBLE:
    return jv_number(obj.via.dec);
  case MSGPACK_OBJECT_RAW:
    return jv_string_sized(obj.via.raw.ptr, obj.via.raw.size);
  case MSGPACK_OBJECT_ARRAY:
    return msgpack_array_to_jv(obj.via.array);
  case MSGPACK_OBJECT_MAP:
    return msgpack_map_to_jv(obj.via.map);
  default:
    return jv_null();
  }
}
