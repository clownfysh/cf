#include "cf/x/case/hash_table.h"

struct cf_x_case_hash_table_t {
};

cf_x_core_bool_t cf_x_case_hash_table_add(cf_x_case_hash_table_t *hash_table,
    void *object)
{
  cf_x_core_implement();
  return cf_x_core_bool_false;
}

void cf_x_case_hash_table_clear(cf_x_case_hash_table_t *hash_table)
{
  cf_x_core_implement();
}

cf_x_case_hash_table_t *cf_x_case_hash_table_create
(cf_x_core_iobject_t *iobject)
{
  cf_x_core_implement();
  return NULL;
}

void cf_x_case_hash_table_destroy(cf_x_case_hash_table_t *hash_table)
{
  cf_x_core_implement();
}

void cf_x_case_hash_table_dont_destroy_objects
(cf_x_case_hash_table_t *hash_table)
{
  cf_x_core_implement();
}

void *cf_x_case_hash_table_find(cf_x_case_hash_table_t *hash_table,
    void *decoy_object)
{
  cf_x_core_implement();
  return NULL;
}

cf_x_core_iobject_t *cf_x_case_hash_table_get_iobject
(cf_x_case_hash_table_t *hash_table)
{
  cf_x_core_implement();
  return NULL;
}

unsigned long cf_x_case_hash_table_get_size
(cf_x_case_hash_table_t *hash_table)
{
  cf_x_core_implement();
  return 0;
}

void cf_x_case_hash_table_iterate_remove
(cf_x_case_hash_table_t *hash_table)
{
  cf_x_core_implement();
}

void cf_x_case_hash_table_iterate_start(cf_x_case_hash_table_t *hash_table)
{
  cf_x_core_implement();
}

void *cf_x_case_hash_table_iterate_next(cf_x_case_hash_table_t *hash_table)
{
  cf_x_core_implement();
  return NULL;
}

cf_x_core_bool_t cf_x_case_hash_table_remove
(cf_x_case_hash_table_t *hash_table, void *decoy_object)
{
  cf_x_core_implement();
  return cf_x_core_bool_false;
}
