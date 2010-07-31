#ifndef cf_x_core_pair_h
#define cf_x_core_pair_h

#include "cf/x/core/object.h"
#include "cf/x/core/iobject.h"

#define CF_X_CORE_PAIR_DESTROYS cf_x_core_bool_true
#define CF_X_CORE_PAIR_DONT_DESTROY cf_x_core_bool_false

struct cf_x_core_pair_t;
typedef struct cf_x_core_pair_t cf_x_core_pair_t;

int cf_x_core_pair_compare(void *pair_object_a, void *pair_object_b);

int cf_x_core_pair_compare_left(void *pair_object_a, void *pair_object_b);

int cf_x_core_pair_compare_right(void *pair_object_a, void *pair_object_b);

void *cf_x_core_pair_copy(void *pair_object);

cf_x_core_pair_t *cf_x_core_pair_create(void *left, cf_x_core_iobject_t *left_iobject,
    void *right, cf_x_core_iobject_t *right_iobject, cf_x_core_bool_t destroys);

cf_x_core_pair_t *cf_x_core_pair_create_decoy(void *left,
    cf_x_core_iobject_t *left_iobject);

void cf_x_core_pair_destroy(void *pair_object);

void cf_x_core_pair_destroy_decoy(cf_x_core_pair_t *pair);

cf_x_core_bool_t cf_x_core_pair_equal_left(void *pair_a_object, void *pair_b_object);

char *cf_x_core_pair_get_as_string(void *pair_object);

void *cf_x_core_pair_get_left(cf_x_core_pair_t *pair);

cf_x_core_iobject_t *cf_x_core_pair_get_left_iobject(cf_x_core_pair_t *pair);

void *cf_x_core_pair_get_right(cf_x_core_pair_t *pair);

cf_x_core_iobject_t *cf_x_core_pair_get_right_iobject(cf_x_core_pair_t *pair);

void cf_x_core_pair_init_iobject(cf_x_core_iobject_t *iobject);

unsigned long cf_x_core_pair_mod_left(void *pair_object, unsigned long divisor);

#endif
