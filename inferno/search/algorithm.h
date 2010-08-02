#ifndef cf_inferno_search_algorithm_h
#define cf_inferno_search_algorithm_h

#include "cf/x/core/bool.h"

enum cf_inferno_search_algorithm_t {
  CF_INFERNO_SEARCH_ALGORITHM_BIOS,
  CF_INFERNO_SEARCH_ALGORITHM_COR3,
  CF_INFERNO_SEARCH_ALGORITHM_EOS,
  CF_INFERNO_SEARCH_ALGORITHM_GENETIC,
};
typedef enum cf_inferno_search_algorithm_t cf_inferno_search_algorithm_t;

cf_x_core_bool_t cf_inferno_search_algorithm_get_from_string(char *string,
    cf_inferno_search_algorithm_t *algorithm);

char *cf_inferno_search_algorithm_get_string
(cf_inferno_search_algorithm_t algorithm);

#endif
