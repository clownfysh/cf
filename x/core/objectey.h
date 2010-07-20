#ifndef x_core_objectey_h
#define x_core_objectey_h

#include "x/core/object.h"

struct x_core_objectey_t {
  x_core_compare_f compare;
  x_core_copy_f copy;
  x_core_destroy_f destroy;
  x_core_get_as_string_f get_as_string;
};
typedef struct x_core_objectey_t x_core_objectey_t;

void x_core_objectey_init(x_core_objectey_t *objectey,
    x_core_compare_f compare, x_core_copy_f copy,
    x_core_destroy_f destroy,
    x_core_get_as_string_f get_as_string);

#endif
