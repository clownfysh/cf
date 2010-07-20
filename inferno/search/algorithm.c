#include "inferno/search/algorithm.h"
#include "x/core/tools.h"

x_core_bool_t inferno_searcx_algorithm_get_from_string(char *string,
    inferno_searcx_algorithm_t *algorithm)
{
  assert(string);
  assert(algorithm);
  x_core_bool_t success;

  if (0 == strcmp(string, "bios")) {
    success = x_core_bool_true;
    *algorithm = INFERNO_SEARCX_ALGORITHM_BIOS;

  } else if (0 == strcmp(string, "cor3")) {
    success = x_core_bool_true;
    *algorithm = INFERNO_SEARCX_ALGORITHM_COR3;

  } else if (0 == strcmp(string, "eos")) {
    success = x_core_bool_true;
    *algorithm = INFERNO_SEARCX_ALGORITHM_EOS;

  } else if (0 == strcmp(string, "genetic")) {
    success = x_core_bool_true;
    *algorithm = INFERNO_SEARCX_ALGORITHM_GENETIC;

  } else {
    success = x_core_bool_false;
  }

  return success;
}

char *inferno_searcx_algorithm_get_string(inferno_searcx_algorithm_t algorithm)
{
  char *string;

  switch (algorithm) {
    case INFERNO_SEARCX_ALGORITHM_BIOS:
      string = "bios";
      break;
    case INFERNO_SEARCX_ALGORITHM_COR3:
      string = "cor3";
      break;
    case INFERNO_SEARCX_ALGORITHM_EOS:
      string = "eos";
      break;
    case INFERNO_SEARCX_ALGORITHM_GENETIC:
      string = "genetic";
      break;
    default:
      string = NULL;
      break;
  }

  return string;
}
