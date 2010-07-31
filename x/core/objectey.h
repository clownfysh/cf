#ifndef cf_x_core_objectey_h
#define cf_x_core_objectey_h

#include "cf/x/core/object.h"

struct cf_x_core_objectey_t {
  cf_x_core_compare_f compare;
  cf_x_core_copy_f copy;
  cf_x_core_destroy_f destroy;
  cf_x_core_equal_f equal;
  cf_x_core_get_as_string_f get_as_string;
  cf_x_core_mod_f mod;
};
typedef struct cf_x_core_objectey_t cf_x_core_objectey_t;

void cf_x_core_objectey_init(cf_x_core_objectey_t *objectey,
    cf_x_core_compare_f compare, cf_x_core_copy_f copy, cf_x_core_destroy_f destroy,
    cf_x_core_equal_f equal, cf_x_core_get_as_string_f get_as_string,
    cf_x_core_mod_f mod);

#endif
