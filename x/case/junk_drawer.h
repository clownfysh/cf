#ifndef cf_x_case_junk_drawer_h
#define cf_x_case_junk_drawer_h

#include "cf/x/core/object.h"
#include "cf/x/core/iobject.h"
#include "cf/x/core/tools.h"

struct cf_x_case_junk_drawer_t;
typedef struct cf_x_case_junk_drawer_t cf_x_case_junk_drawer_t;

cf_x_core_bool_t cf_x_case_junk_drawer_add
(cf_x_case_junk_drawer_t *junk_drawer, void *object);

void cf_x_case_junk_drawer_clear(cf_x_case_junk_drawer_t *junk_drawer);

cf_x_case_junk_drawer_t *cf_x_case_junk_drawer_create
(cf_x_core_iobject_t *iobject);

void cf_x_case_junk_drawer_destroy(cf_x_case_junk_drawer_t *junk_drawer);

void cf_x_case_junk_drawer_dont_destroy_objects
(cf_x_case_junk_drawer_t *junk_drawer);

void *cf_x_case_junk_drawer_find(cf_x_case_junk_drawer_t *junk_drawer,
    void *decoy_object);

cf_x_core_iobject_t *cf_x_case_junk_drawer_get_iobject
(cf_x_case_junk_drawer_t *junk_drawer);

unsigned long cf_x_case_junk_drawer_get_size
(cf_x_case_junk_drawer_t *junk_drawer);

void cf_x_case_junk_drawer_iterate_remove
(cf_x_case_junk_drawer_t *junk_drawer);

void cf_x_case_junk_drawer_iterate_start(cf_x_case_junk_drawer_t *junk_drawer);

void *cf_x_case_junk_drawer_iterate_next(cf_x_case_junk_drawer_t *junk_drawer);

cf_x_core_bool_t cf_x_case_junk_drawer_remove
(cf_x_case_junk_drawer_t *junk_drawer, void *decoy_object);

#endif
