#include "cf/sage/search/algorithm.h"
#include "cf/x/core/tools.h"

cf_x_core_bool_t cf_sage_searcx_algorithm_get_from_string(char *string,
    cf_sage_searcx_algorithm_t *algorithm)
{
  assert(string);
  assert(algorithm);
  cf_x_core_bool_t success;

  if (0 == strcmp(string, "bios")) {
    success = cf_x_core_bool_true;
    *algorithm = CF_SAGE_SEARCX_ALGORITHM_BIOS;

  } else if (0 == strcmp(string, "cor3")) {
    success = cf_x_core_bool_true;
    *algorithm = CF_SAGE_SEARCX_ALGORITHM_COR3;

  } else if (0 == strcmp(string, "eos")) {
    success = cf_x_core_bool_true;
    *algorithm = CF_SAGE_SEARCX_ALGORITHM_EOS;

  } else if (0 == strcmp(string, "genetic")) {
    success = cf_x_core_bool_true;
    *algorithm = CF_SAGE_SEARCX_ALGORITHM_GENETIC;

  } else {
    success = cf_x_core_bool_false;
  }

  return success;
}

char *cf_sage_searcx_algorithm_get_string(cf_sage_searcx_algorithm_t algorithm)
{
  char *string;

  switch (algorithm) {
    case CF_SAGE_SEARCX_ALGORITHM_BIOS:
      string = "bios";
      break;
    case CF_SAGE_SEARCX_ALGORITHM_COR3:
      string = "cor3";
      break;
    case CF_SAGE_SEARCX_ALGORITHM_EOS:
      string = "eos";
      break;
    case CF_SAGE_SEARCX_ALGORITHM_GENETIC:
      string = "genetic";
      break;
    default:
      string = NULL;
      break;
  }

  return string;
}
