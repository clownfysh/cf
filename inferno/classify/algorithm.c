#include "cf/inferno/classify/algorithm.h"
#include "cf/x/core/tools.h"

cf_x_core_bool_t cf_inferno_classify_algorithm_get_from_string(char *string,
    cf_inferno_classify_algorithm_t *algorithm)
{
  assert(string);
  assert(algorithm);
  cf_x_core_bool_t success;

  if (0 == strcmp(string, "archetype")) {
    success = cf_x_core_bool_true;
    *algorithm = CF_INFERNO_CLASSIFY_ALGORITHM_ARCHETYPE;

  } else if (0 == strcmp(string, "...")) {
    success = cf_x_core_bool_true;
    *algorithm = CF_INFERNO_CLASSIFY_ALGORITHM_ARCHETYPE;

  } else {
    success = cf_x_core_bool_false;
  }

  return success;
}

char *cf_inferno_classify_algorithm_get_string(cf_inferno_classify_algorithm_t algorithm)
{
  char *string;

  switch (algorithm) {
    case CF_INFERNO_CLASSIFY_ALGORITHM_ARCHETYPE:
      string = "archetype";
      break;
    default:
      string = NULL;
      break;
  }

  return string;
}
