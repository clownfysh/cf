#ifndef cf_x_core_bool_h
#define cf_x_core_bool_h

#include "cf/x/core/declarations.h"

enum cf_x_core_bool_t {
  cf_x_core_bool_false = 0,
  cf_x_core_bool_true = 1,
};
typedef enum cf_x_core_bool_t cf_x_core_bool_t;

cf_x_core_bool_t cf_x_core_bool_add_to_message(void *bool_object,
    cf_x_core_message_t *message);

void *cf_x_core_bool_create_from_message(cf_x_core_message_t *message);

#endif
