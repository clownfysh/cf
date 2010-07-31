#include "x/core/bool.h"
#include "x/core/tools.h"
#include "x/core/uuid.h"

#define NULL_UUID "a8fffbb4-675c-4896-81f0-6eccf8bd8760"

struct x_core_uuid_t {
  uuid_t uuid;
  char *string;
};

x_core_bool_t x_core_uuid_add_to_message(void *uuid_object,
    x_core_message_t *message)
{
  assert(uuid_object);
  assert(message);
  x_core_bool_t success;
  char *uuid_string;
  x_core_uuid_t *uuid;

  uuid = uuid_object;

  uuid_string = x_core_uuid_get_string(uuid);
  if (x_core_message_add_string(message, uuid_string)) {
    success = x_core_bool_true;
  } else {
    success = x_core_bool_false;
    x_core_trace("x_core_message_add_string");
  }

  return success;
}

int x_core_uuid_compare(void *uuid_a_object, void *uuid_b_object)
{
  assert(uuid_a_object);
  assert(uuid_b_object);
  x_core_uuid_t *uuid_a = uuid_a_object;
  x_core_uuid_t *uuid_b = uuid_b_object;

  return uuid_compare(uuid_a->uuid, uuid_b->uuid);
}

void *x_core_uuid_copy(void *uuid_object)
{
  assert(uuid_object);
  x_core_uuid_t *original;
  x_core_uuid_t *copy;

  original = uuid_object;

  copy = malloc(sizeof *copy);
  if (copy) {
    copy->string = NULL;
    uuid_copy(copy->uuid, original->uuid);
  } else {
    x_core_trace("malloc");
  }

  return copy;
}

x_core_uuid_t *x_core_uuid_create()
{
  x_core_uuid_t *uuid;

  uuid = malloc(sizeof *uuid);
  if (uuid) {
    uuid_generate(uuid->uuid);
    uuid->string = NULL;
  } else {
    x_core_trace("malloc");
  }

  return uuid;
}

void *x_core_uuid_create_from_message(x_core_message_t *message)
{
  assert(message);
  x_core_uuid_t *uuid;
  char *uuid_string;

  uuid_string = x_core_message_take_string(message);
  if (uuid_string) {
    uuid = x_core_uuid_create_from_string(uuid_string);
    if (!uuid) {
      x_core_trace("x_core_uuid_create_from_string");
    }
    free(uuid_string);
  } else {
    x_core_trace("x_core_message_take_string");
    uuid = NULL;
  }

  return uuid;
}

x_core_uuid_t *x_core_uuid_create_from_string(const char *uuid_string)
{
  x_core_uuid_t *uuid;

  uuid = malloc(sizeof *uuid);
  if (uuid) {
    uuid->string = NULL;
    if (0 != uuid_parse(uuid_string, uuid->uuid)) {
      free(uuid);
      uuid = NULL;
    }
  } else {
    x_core_trace("malloc");
  }

  return uuid;
}

x_core_uuid_t *x_core_uuid_create_null()
{
  return x_core_uuid_create_from_string(NULL_UUID);
}

void x_core_uuid_destroy(void *uuid_object)
{
  assert(uuid_object);
  x_core_uuid_t *uuid;

  uuid = uuid_object;

  if (uuid->string) {
    if (strcmp(uuid->string, NULL_UUID) != 0) {
      free(uuid->string);
    }
  }
  free(uuid);
}

x_core_bool_t x_core_uuid_equal(void *uuid_a_object, void *uuid_b_object)
{
  assert(uuid_a_object);
  assert(uuid_b_object);
  return (0 == x_core_uuid_compare(uuid_a_object, uuid_b_object));
}

char *x_core_uuid_get_as_string(void *uuid_object)
{
  assert(uuid_object);
  char *s;

  s = strdup(x_core_uuid_get_string(uuid_object));
  if (!s) {
    x_trace("strdup");
  }

  return s;
}

unsigned long x_core_uuid_get_memory_size_bytes(x_core_uuid_t *uuid)
{
  assert(uuid);
  unsigned long size;

  size = sizeof *uuid;
  size += sizeof(uuid_t);

  return size;
}

char *x_core_uuid_get_string(void *uuid_object)
{
  assert(uuid_object);
  x_core_uuid_t *uuid;

  uuid = uuid_object;

  if (!uuid->string) {
    uuid->string = malloc(37);
    if (uuid->string) {
      uuid_unparse(uuid->uuid, uuid->string);
    } else {
      uuid->string = NULL_UUID;
      x_core_trace("malloc");
    }
  }

  assert(uuid->string);
  return uuid->string;
}

unsigned long x_core_uuid_hash(x_core_uuid_t *uuid)
{
  assert(uuid);
  unsigned long hash;

  x_core_uuid_get_string(uuid);
  hash = x_core_hash_djb2_xor(uuid->string);

  return hash;
}

void x_core_uuid_init_objectey(x_core_objectey_t *objectey)
{
  x_core_objectey_init(objectey, x_core_uuid_compare, x_core_uuid_copy,
      x_core_uuid_destroy, x_core_uuid_equal, x_core_uuid_get_as_string,
      x_core_uuid_mod);
}

x_core_bool_t x_core_uuid_is_null(x_core_uuid_t *uuid)
{
  assert(uuid);
  x_core_bool_t is_null;
  x_core_uuid_t *null_uuid;

  is_null = x_core_bool_false;

  null_uuid = x_core_uuid_create_null();
  if (null_uuid) {
    if (0 == x_core_uuid_compare(uuid, null_uuid)) {
      is_null = x_core_bool_true;
    }
    x_core_uuid_destroy(null_uuid);
  } else {
    x_core_trace("x_core_uuid_create_null");
  }

  return is_null;
}

#if defined CF_PLATFORM_LINUX
#include "x/core/uuid.impl.linux.c"
#endif
