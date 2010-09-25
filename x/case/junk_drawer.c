#include "cf/x/case/junk_drawer.h"

/*
  super-simple
  great for small collections
  last-used stuff is near the top
  stochastic
  messy
  that's it
*/

struct cf_x_case_junk_drawer_t {
};

cf_x_core_bool_t cf_x_case_junk_drawer_add
(cf_x_case_junk_drawer_t *junk_drawer, void *object)
{
  return cf_x_core_bool_false;
}

void cf_x_case_junk_drawer_clear(cf_x_case_junk_drawer_t *junk_drawer)
{
}

cf_x_case_junk_drawer_t *cf_x_case_junk_drawer_create
(cf_x_core_iobject_t *iobject)
{
  return NULL;
}

void cf_x_case_junk_drawer_destroy(cf_x_case_junk_drawer_t *junk_drawer)
{
}

void cf_x_case_junk_drawer_dont_destroy_objects
(cf_x_case_junk_drawer_t *junk_drawer)
{
}

void *cf_x_case_junk_drawer_find(cf_x_case_junk_drawer_t *junk_drawer,
    void *decoy_object)
{
  return NULL;
}

cf_x_core_iobject_t *cf_x_case_junk_drawer_get_iobject
(cf_x_case_junk_drawer_t *junk_drawer)
{
  return NULL;
}

unsigned long cf_x_case_junk_drawer_get_size
(cf_x_case_junk_drawer_t *junk_drawer)
{
  return 0;
}

void cf_x_case_junk_drawer_iterate_remove(cf_x_case_junk_drawer_t *junk_drawer)
{
}

void cf_x_case_junk_drawer_iterate_start(cf_x_case_junk_drawer_t *junk_drawer)
{
}

void *cf_x_case_junk_drawer_iterate_next(cf_x_case_junk_drawer_t *junk_drawer)
{
  return NULL;
}

cf_x_core_bool_t cf_x_case_junk_drawer_remove
(cf_x_case_junk_drawer_t *junk_drawer, void *decoy_object)
{
  return cf_x_core_bool_false;
}
