#ifndef x_core_pair_h
#define x_core_pair_h

#include "x/core/object.h"
#include "x/core/objectey.h"

#define X_CORE_PAIR_DESTROYS x_core_bool_true
#define X_CORE_PAIR_DONT_DESTROY x_core_bool_false

struct x_core_pair_t;
typedef struct x_core_pair_t x_core_pair_t;

int x_core_pair_compare(void *pair_object_a, void *pair_object_b);

int x_core_pair_compare_left(void *pair_object_a, void *pair_object_b);

int x_core_pair_compare_right(void *pair_object_a, void *pair_object_b);

void *x_core_pair_copy(void *pair_object);

x_core_pair_t *x_core_pair_create(void *left, x_core_objectey_t *left_objectey,
    void *right, x_core_objectey_t *right_objectey, x_core_bool_t destroys);

x_core_pair_t *x_core_pair_create_decoy(void *left,
    x_core_objectey_t *left_objectey);

void x_core_pair_destroy(void *pair_object);

void x_core_pair_destroy_decoy(x_core_pair_t *pair);

x_core_bool_t x_core_pair_equal_left(void *pair_a_object, void *pair_b_object);

char *x_core_pair_get_as_string(void *pair_object);

void *x_core_pair_get_left(x_core_pair_t *pair);

x_core_objectey_t *x_core_pair_get_left_objectey(x_core_pair_t *pair);

void *x_core_pair_get_right(x_core_pair_t *pair);

x_core_objectey_t *x_core_pair_get_right_objectey(x_core_pair_t *pair);

void x_core_pair_init_objectey(x_core_objectey_t *objectey);

unsigned long x_core_pair_mod_left(void *pair_object, unsigned long divisor);

#endif
