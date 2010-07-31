#ifndef cf_x_xcache_item_h
#define cf_x_xcache_item_h

#include "cf/x/core/object.h"
#include "cf/x/core/uuid.h"

struct cf_x_xcache_item_t;
typedef struct cf_x_xcache_item_t cf_x_xcache_item_t;

int cf_x_xcache_item_compare(void *item_object_a, void *item_object_b);

void *cf_x_xcache_item_copy(void *item_object);

cf_x_xcache_item_t *cf_x_xcache_item_create(cf_x_core_uuid_t *object_uuid,
    void *object, cf_x_core_copy_f copy,
    cf_x_core_destroy_f destroy, unsigned long lifespan_seconds);

cf_x_xcache_item_t *cf_x_xcache_item_create_decoy(cf_x_core_uuid_t *object_uuid);

void cf_x_xcache_item_destroy(void *item_object);

void cf_x_xcache_item_destroy_decoy(void *item_object);

void cf_x_xcache_item_expire(cf_x_xcache_item_t *item);

void *cf_x_xcache_item_get_object(cf_x_xcache_item_t *item);

void *cf_x_xcache_item_get_object_copy(cf_x_xcache_item_t *item);

cf_x_core_uuid_t *cf_x_xcache_item_get_object_uuid(cf_x_xcache_item_t *item);

cf_x_core_bool_t cf_x_xcache_item_is_expired(void *item_object);

#endif
