#include "x/container/hash_table.h"

struct x_container_hash_table_t {
};

x_core_bool_t x_container_hash_table_add(x_container_hash_table_t *hash_table,
    void *object)
{
  x_core_implement();
  return x_core_bool_false;
}

void x_container_hash_table_clear(x_container_hash_table_t *hash_table)
{
  x_core_implement();
}

x_container_hash_table_t *x_container_hash_table_create
(x_core_objectey_t *objectey)
{
  x_core_implement();
  return NULL;
}

void x_container_hash_table_destroy(x_container_hash_table_t *hash_table)
{
  x_core_implement();
}

void x_container_hash_table_dont_destroy_objects
(x_container_hash_table_t *hash_table)
{
  x_core_implement();
}

void *x_container_hash_table_find(x_container_hash_table_t *hash_table,
    void *decoy_object)
{
  x_core_implement();
  return NULL;
}

x_core_objectey_t *x_container_hash_table_get_objectey
(x_container_hash_table_t *hash_table)
{
  x_core_implement();
  return NULL;
}

unsigned long x_container_hash_table_get_size
(x_container_hash_table_t *hash_table)
{
  x_core_implement();
  return 0;
}

void x_container_hash_table_iterate_remove
(x_container_hash_table_t *hash_table)
{
  x_core_implement();
}

void x_container_hash_table_iterate_start(x_container_hash_table_t *hash_table)
{
  x_core_implement();
}

void *x_container_hash_table_iterate_next(x_container_hash_table_t *hash_table)
{
  x_core_implement();
  return NULL;
}

x_core_bool_t x_container_hash_table_remove
(x_container_hash_table_t *hash_table, void *decoy_object)
{
  x_core_implement();
  return x_core_bool_false;
}
