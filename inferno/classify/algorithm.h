#ifndef cf_inferno_classify_algorithm_h
#define cf_inferno_classify_algorithm_h

#include "cf/x/core/bool.h"

enum cf_inferno_classify_algorithm_t {
  CF_INFERNO_CLASSIFY_ALGORITHM_ARCHETYPE,
};
typedef enum cf_inferno_classify_algorithm_t cf_inferno_classify_algorithm_t;

cf_x_core_bool_t cf_inferno_classify_algorithm_get_from_string(char *string,
    cf_inferno_classify_algorithm_t *algorithm);

char *cf_inferno_classify_algorithm_get_string
(cf_inferno_classify_algorithm_t algorithm);

#endif
