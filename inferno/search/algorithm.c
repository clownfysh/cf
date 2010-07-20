#include "inferno/search/algorithm.h"
#include "h/core/tools.h"

h_core_bool_t inferno_search_algorithm_get_from_string(char *string,
    inferno_search_algorithm_t *algorithm)
{
  assert(string);
  assert(algorithm);
  h_core_bool_t success;

  if (0 == strcmp(string, "bios")) {
    success = h_core_bool_true;
    *algorithm = INFERNO_SEARCH_ALGORITHM_BIOS;

  } else if (0 == strcmp(string, "cor3")) {
    success = h_core_bool_true;
    *algorithm = INFERNO_SEARCH_ALGORITHM_COR3;

  } else if (0 == strcmp(string, "eos")) {
    success = h_core_bool_true;
    *algorithm = INFERNO_SEARCH_ALGORITHM_EOS;

  } else if (0 == strcmp(string, "genetic")) {
    success = h_core_bool_true;
    *algorithm = INFERNO_SEARCH_ALGORITHM_GENETIC;

  } else {
    success = h_core_bool_false;
  }

  return success;
}

char *inferno_search_algorithm_get_string(inferno_search_algorithm_t algorithm)
{
  char *string;

  switch (algorithm) {
    case INFERNO_SEARCH_ALGORITHM_BIOS:
      string = "bios";
      break;
    case INFERNO_SEARCH_ALGORITHM_COR3:
      string = "cor3";
      break;
    case INFERNO_SEARCH_ALGORITHM_EOS:
      string = "eos";
      break;
    case INFERNO_SEARCH_ALGORITHM_GENETIC:
      string = "genetic";
      break;
    default:
      string = NULL;
      break;
  }

  return string;
}
