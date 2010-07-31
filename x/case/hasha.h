#ifndef cf_x_case_hasha_h
#define cf_x_case_hasha_h

#include "cf/x/core/object.h"
#include "cf/x/core/iobject.h"
#include "cf/x/core/tools.h"

struct cf_x_case_hasha_t;
typedef struct cf_x_case_hasha_t cf_x_case_hasha_t;

cf_x_core_bool_t cf_x_case_hasha_add(cf_x_case_hasha_t *hasha, void *object);

void cf_x_case_hasha_clear(cf_x_case_hasha_t *hasha);

cf_x_case_hasha_t *cf_x_case_hasha_create(cf_x_core_iobject_t *iobject);

void cf_x_case_hasha_destroy(cf_x_case_hasha_t *hasha);

void cf_x_case_hasha_dont_destroy_objects(cf_x_case_hasha_t *hasha);

void *cf_x_case_hasha_find(cf_x_case_hasha_t *hasha, void *decoy_object);

cf_x_core_iobject_t *cf_x_case_hasha_get_iobject(cf_x_case_hasha_t *hasha);

unsigned long cf_x_case_hasha_get_size(cf_x_case_hasha_t *hasha);

void cf_x_case_hasha_iterate_remove(cf_x_case_hasha_t *hasha);

void cf_x_case_hasha_iterate_start(cf_x_case_hasha_t *hasha);

void *cf_x_case_hasha_iterate_next(cf_x_case_hasha_t *hasha);

cf_x_core_bool_t cf_x_case_hasha_remove(cf_x_case_hasha_t *hasha,
    void *decoy_object);

#endif
