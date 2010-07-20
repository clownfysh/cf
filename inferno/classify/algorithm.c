#include "inferno/classify/algorithm.h"
#include "x/core/tools.h"

x_core_bool_t inferno_classify_algorithm_get_from_string(char *string,
    inferno_classify_algorithm_t *algorithm)
{
  assert(string);
  assert(algorithm);
  x_core_bool_t success;

  if (0 == strcmp(string, "archetype")) {
    success = x_core_bool_true;
    *algorithm = INFERNO_CLASSIFY_ALGORITHM_ARCHETYPE;

  } else if (0 == strcmp(string, "...")) {
    success = x_core_bool_true;
    *algorithm = INFERNO_CLASSIFY_ALGORITHM_ARCHETYPE;

  } else {
    success = x_core_bool_false;
  }

  return success;
}

char *inferno_classify_algorithm_get_string(inferno_classify_algorithm_t algorithm)
{
  char *string;

  switch (algorithm) {
    case INFERNO_CLASSIFY_ALGORITHM_ARCHETYPE:
      string = "archetype";
      break;
    default:
      string = NULL;
      break;
  }

  return string;
}
