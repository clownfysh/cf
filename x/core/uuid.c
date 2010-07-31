#include "cf/x/core/bool.h"
#include "cf/x/core/tools.h"
#include "cf/x/core/uuid.h"

#define NULL_UUID "a8fffbb4-675c-4896-81f0-6eccf8bd8760"

struct cf_x_core_uuid_t {
  uuid_t uuid;
  char *string;
};

cf_x_core_bool_t cf_x_core_uuid_add_to_message(void *uuid_object,
    cf_x_core_message_t *message)
{
  assert(uuid_object);
  assert(message);
  cf_x_core_bool_t success;
  char *uuid_string;
  cf_x_core_uuid_t *uuid;

  uuid = uuid_object;

  uuid_string = cf_x_core_uuid_get_string(uuid);
  if (cf_x_core_message_add_string(message, uuid_string)) {
    success = cf_x_core_bool_true;
  } else {
    success = cf_x_core_bool_false;
    cf_x_core_trace("x_core_message_add_string");
  }

  return success;
}

int cf_x_core_uuid_compare(void *uuid_a_object, void *uuid_b_object)
{
  assert(uuid_a_object);
  assert(uuid_b_object);
  cf_x_core_uuid_t *uuid_a = uuid_a_object;
  cf_x_core_uuid_t *uuid_b = uuid_b_object;

  return uuid_compare(uuid_a->uuid, uuid_b->uuid);
}

void *cf_x_core_uuid_copy(void *uuid_object)
{
  assert(uuid_object);
  cf_x_core_uuid_t *original;
  cf_x_core_uuid_t *copy;

  original = uuid_object;

  copy = malloc(sizeof *copy);
  if (copy) {
    copy->string = NULL;
    uuid_copy(copy->uuid, original->uuid);
  } else {
    cf_x_core_trace("malloc");
  }

  return copy;
}

cf_x_core_uuid_t *cf_x_core_uuid_create()
{
  cf_x_core_uuid_t *uuid;

  uuid = malloc(sizeof *uuid);
  if (uuid) {
    uuid_generate(uuid->uuid);
    uuid->string = NULL;
  } else {
    cf_x_core_trace("malloc");
  }

  return uuid;
}

void *cf_x_core_uuid_create_from_message(cf_x_core_message_t *message)
{
  assert(message);
  cf_x_core_uuid_t *uuid;
  char *uuid_string;

  uuid_string = cf_x_core_message_take_string(message);
  if (uuid_string) {
    uuid = cf_x_core_uuid_create_from_string(uuid_string);
    if (!uuid) {
      cf_x_core_trace("x_core_uuid_create_from_string");
    }
    free(uuid_string);
  } else {
    cf_x_core_trace("x_core_message_take_string");
    uuid = NULL;
  }

  return uuid;
}

cf_x_core_uuid_t *cf_x_core_uuid_create_from_string(const char *uuid_string)
{
  cf_x_core_uuid_t *uuid;

  uuid = malloc(sizeof *uuid);
  if (uuid) {
    uuid->string = NULL;
    if (0 != uuid_parse(uuid_string, uuid->uuid)) {
      free(uuid);
      uuid = NULL;
    }
  } else {
    cf_x_core_trace("malloc");
  }

  return uuid;
}

cf_x_core_uuid_t *cf_x_core_uuid_create_null()
{
  return cf_x_core_uuid_create_from_string(NULL_UUID);
}

void cf_x_core_uuid_destroy(void *uuid_object)
{
  assert(uuid_object);
  cf_x_core_uuid_t *uuid;

  uuid = uuid_object;

  if (uuid->string) {
    if (strcmp(uuid->string, NULL_UUID) != 0) {
      free(uuid->string);
    }
  }
  free(uuid);
}

cf_x_core_bool_t cf_x_core_uuid_equal(void *uuid_a_object, void *uuid_b_object)
{
  assert(uuid_a_object);
  assert(uuid_b_object);
  return (0 == cf_x_core_uuid_compare(uuid_a_object, uuid_b_object));
}

char *cf_x_core_uuid_get_as_string(void *uuid_object)
{
  assert(uuid_object);
  char *s;

  s = strdup(cf_x_core_uuid_get_string(uuid_object));
  if (!s) {
    cf_x_trace("strdup");
  }

  return s;
}

unsigned long cf_x_core_uuid_get_memory_size_bytes(cf_x_core_uuid_t *uuid)
{
  assert(uuid);
  unsigned long size;

  size = sizeof *uuid;
  size += sizeof(uuid_t);

  return size;
}

char *cf_x_core_uuid_get_string(void *uuid_object)
{
  assert(uuid_object);
  cf_x_core_uuid_t *uuid;

  uuid = uuid_object;

  if (!uuid->string) {
    uuid->string = malloc(37);
    if (uuid->string) {
      uuid_unparse(uuid->uuid, uuid->string);
    } else {
      uuid->string = NULL_UUID;
      cf_x_core_trace("malloc");
    }
  }

  assert(uuid->string);
  return uuid->string;
}

unsigned long cf_x_core_uuid_hash(cf_x_core_uuid_t *uuid)
{
  assert(uuid);
  unsigned long hash;

  cf_x_core_uuid_get_string(uuid);
  hash = cf_x_core_hash_djb2_xor(uuid->string);

  return hash;
}

void cf_x_core_uuid_init_objectey(cf_x_core_objectey_t *objectey)
{
  cf_x_core_objectey_init(objectey, cf_x_core_uuid_compare, cf_x_core_uuid_copy,
      cf_x_core_uuid_destroy, cf_x_core_uuid_equal, cf_x_core_uuid_get_as_string,
      cf_x_core_uuid_mod);
}

cf_x_core_bool_t cf_x_core_uuid_is_null(cf_x_core_uuid_t *uuid)
{
  assert(uuid);
  cf_x_core_bool_t is_null;
  cf_x_core_uuid_t *null_uuid;

  is_null = cf_x_core_bool_false;

  null_uuid = cf_x_core_uuid_create_null();
  if (null_uuid) {
    if (0 == cf_x_core_uuid_compare(uuid, null_uuid)) {
      is_null = cf_x_core_bool_true;
    }
    cf_x_core_uuid_destroy(null_uuid);
  } else {
    cf_x_core_trace("x_core_uuid_create_null");
  }

  return is_null;
}

#if defined CF_PLATFORM_LINUX
#include "cf/x/core/uuid.impl.linux.c"
#endif
