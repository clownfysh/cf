#include "x/case/hasha.h"

struct x_case_hasha_t {
};

x_core_bool_t x_case_hasha_add(x_case_hasha_t *hasha, void *object)
{
  x_core_implement();
  return x_core_bool_false;
}

void x_case_hasha_clear(x_case_hasha_t *hasha)
{
  x_core_implement();
}

x_case_hasha_t *x_case_hasha_create(x_core_objectey_t *objectey)
{
  x_core_implement();
  return NULL;
}

void x_case_hasha_destroy(x_case_hasha_t *hasha)
{
  x_core_implement();
}

void x_case_hasha_dont_destroy_objects(x_case_hasha_t *hasha)
{
  x_core_implement();
}

void *x_case_hasha_find(x_case_hasha_t *hasha, void *decoy_object)
{
  x_core_implement();
  return NULL;
}

x_core_objectey_t *x_case_hasha_get_objectey(x_case_hasha_t *hasha)
{
  x_core_implement();
  return NULL;
}

unsigned long x_case_hasha_get_size(x_case_hasha_t *hasha)
{
  x_core_implement();
  return 0;
}

void x_case_hasha_iterate_remove(x_case_hasha_t *hasha)
{
  x_core_implement();
}

void x_case_hasha_iterate_start(x_case_hasha_t *hasha)
{
  x_core_implement();
}

void *x_case_hasha_iterate_next(x_case_hasha_t *hasha)
{
  x_core_implement();
  return NULL;
}

x_core_bool_t x_case_hasha_remove(x_case_hasha_t *hasha,
    void *decoy_object)
{
  x_core_implement();
  return x_core_bool_false;
}
