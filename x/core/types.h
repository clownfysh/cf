#ifndef x_core_types_h
#define x_core_types_h

#include "x/core/object.h"

enum x_core_type_t {
  X_CORE_TYPE_UNKNOWN = 0,
  X_CORE_TYPE_BIT = 1,
  X_CORE_TYPE_X_CORE_BOOL_T = 2,
  X_CORE_TYPE_CHAR = 3,
  X_CORE_TYPE_DOUBLE = 4,
  X_CORE_TYPE_ENUM = 5,
  X_CORE_TYPE_LONG = 6,
  X_CORE_TYPE_SHORT = 7,
  X_CORE_TYPE_STRING = 8,
  X_CORE_TYPE_UNSIGNED_CHAR = 9,
  X_CORE_TYPE_UNSIGNED_LONG = 10,
  X_CORE_TYPE_UNSIGNED_SHORT = 11,
};
typedef enum x_core_type_t x_core_type_t;

typedef void *(*thread_f)(void *thread_context);

#define X_CORE_TIME_STRING_LENGTH 15
typedef char x_core_time_string_t[X_CORE_TIME_STRING_LENGTH + 1];

#endif
