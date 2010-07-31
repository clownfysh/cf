#ifndef cf_x_core_iobject_h
#define cf_x_core_iobject_h

#include "cf/x/core/object.h"

struct cf_x_core_iobject_t {
  cf_x_core_object_compare_f compare;
  cf_x_core_object_compare_equal_f compare_equal;
  cf_x_core_object_copy_f copy;
  cf_x_core_object_destroy_f destroy;
  cf_x_core_object_get_as_string_f get_as_string;
  cf_x_core_object_mod_f mod;
};
typedef struct cf_x_core_iobject_t cf_x_core_iobject_t;

void cf_x_core_iobject_init(cf_x_core_iobject_t *iobject,
    cf_x_core_object_compare_f compare,
    cf_x_core_object_compare_equal_f compare_equal,
    cf_x_core_object_copy_f copy, cf_x_core_object_destroy_f destroy,
    cf_x_core_object_get_as_string_f get_as_string,
    cf_x_core_object_mod_f mod);

#endif
