#include "inferno/classify/algorithm.h"
#include "h/core/tools.h"

h_core_bool_t inferno_classify_algorithm_get_from_string(char *string,
    inferno_classify_algorithm_t *algorithm)
{
  assert(string);
  assert(algorithm);
  h_core_bool_t success;

  if (0 == strcmp(string, "archetype")) {
    success = h_core_bool_true;
    *algorithm = INFERNO_CLASSIFY_ALGORITHM_ARCHETYPE;

  } else if (0 == strcmp(string, "...")) {
    success = h_core_bool_true;
    *algorithm = INFERNO_CLASSIFY_ALGORITHM_ARCHETYPE;

  } else {
    success = h_core_bool_false;
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
