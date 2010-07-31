#ifndef cf_x_core_types_h
#define cf_x_core_types_h

#include "cf/x/core/object.h"

enum cf_x_core_type_t {
  CF_X_CORE_TYPE_UNKNOWN = 0,
  CF_X_CORE_TYPE_BIT = 1,
  CF_X_CORE_TYPE_X_CORE_BOOL_T = 2,
  CF_X_CORE_TYPE_CHAR = 3,
  CF_X_CORE_TYPE_DOUBLE = 4,
  CF_X_CORE_TYPE_ENUM = 5,
  CF_X_CORE_TYPE_LONG = 6,
  CF_X_CORE_TYPE_SHORT = 7,
  CF_X_CORE_TYPE_STRING = 8,
  CF_X_CORE_TYPE_UNSIGNED_CHAR = 9,
  CF_X_CORE_TYPE_UNSIGNED_LONG = 10,
  CF_X_CORE_TYPE_UNSIGNED_SHORT = 11,
};
typedef enum cf_x_core_type_t cf_x_core_type_t;

typedef void *(*thread_f)(void *thread_context);

#define CF_X_CORE_TIME_STRING_LENGTH 15
typedef char cf_x_core_time_string_t[CF_X_CORE_TIME_STRING_LENGTH + 1];

#endif
