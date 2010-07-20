#ifndef x_core_bool_h
#define x_core_bool_h

#include "x/core/declarations.h"

enum x_core_bool_t {
  x_core_bool_false = 0,
  x_core_bool_true = 1,
};
typedef enum x_core_bool_t x_core_bool_t;

x_core_bool_t x_core_bool_add_to_message(void *bool_object,
    x_core_message_t *message);

void *x_core_bool_create_from_message(x_core_message_t *message);

#endif
