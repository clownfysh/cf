#ifndef x_case_hash_table_h
#define x_case_hash_table_h

#include "x/core/object.h"
#include "x/core/objectey.h"
#include "x/core/tools.h"

struct x_case_hash_table_t;
typedef struct x_case_hash_table_t x_case_hash_table_t;

x_core_bool_t x_case_hash_table_add(x_case_hash_table_t *hash_table,
    void *object);

void x_case_hash_table_clear(x_case_hash_table_t *hash_table);

x_case_hash_table_t *x_case_hash_table_create
(x_core_objectey_t *objectey);

void x_case_hash_table_destroy(x_case_hash_table_t *hash_table);

void x_case_hash_table_dont_destroy_objects
(x_case_hash_table_t *hash_table);

void *x_case_hash_table_find(x_case_hash_table_t *hash_table,
    void *decoy_object);

x_core_objectey_t *x_case_hash_table_get_objectey
(x_case_hash_table_t *hash_table);

unsigned long x_case_hash_table_get_size
(x_case_hash_table_t *hash_table);

void x_case_hash_table_iterate_remove
(x_case_hash_table_t *hash_table);

void x_case_hash_table_iterate_start
(x_case_hash_table_t *hash_table);

void *x_case_hash_table_iterate_next
(x_case_hash_table_t *hash_table);

x_core_bool_t x_case_hash_table_remove
(x_case_hash_table_t *hash_table, void *decoy_object);

#endif
