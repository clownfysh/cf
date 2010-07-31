#ifndef cf_x_case_mbin_h
#define cf_x_case_mbin_h

#include "cf/x/core/object.h"
#include "cf/x/core/objectey.h"
#include "cf/x/core/tools.h"

enum cf_x_case_mbin_set_type_t {
  CF_X_CASE_MBIN_SET_TYPE_MULTISET,
  CF_X_CASE_MBIN_SET_TYPE_SET
};
typedef enum cf_x_case_mbin_set_type_t cf_x_case_mbin_set_type_t;

struct cf_x_case_mbin_t;
typedef struct cf_x_case_mbin_t cf_x_case_mbin_t;

cf_x_core_bool_t cf_x_case_mbin_add(cf_x_case_mbin_t *mbin, void *object);

void cf_x_case_mbin_clear(cf_x_case_mbin_t *mbin);

cf_x_case_mbin_t *cf_x_case_mbin_create(cf_x_core_objectey_t *objectey,
    cf_x_case_mbin_set_type_t set_type);

void cf_x_case_mbin_destroy(cf_x_case_mbin_t *mbin);

void cf_x_case_mbin_dont_destroy_objects(cf_x_case_mbin_t *mbin);

void *cf_x_case_mbin_find(cf_x_case_mbin_t *mbin, void *decoy_object);

cf_x_core_objectey_t *cf_x_case_mbin_get_objectey(cf_x_case_mbin_t *mbin);

unsigned long cf_x_case_mbin_get_size(cf_x_case_mbin_t *mbin);

void cf_x_case_mbin_iterate_remove(cf_x_case_mbin_t *mbin);

void cf_x_case_mbin_iterate_start(cf_x_case_mbin_t *mbin);

void *cf_x_case_mbin_iterate_next(cf_x_case_mbin_t *mbin);

cf_x_core_bool_t cf_x_case_mbin_remove(cf_x_case_mbin_t *mbin,
    void *decoy_object);

#endif
