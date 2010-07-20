#ifndef x_mbin_h
#define x_mbin_h

#include "x/core/object.h"
#include "x/core/tools.h"

struct x_mbin_t;
typedef struct x_mbin_t x_mbin_t;

x_core_bool_t x_mbin_add(x_mbin_t *mbin, void *object);
x_mbin_t *x_mbin_create(x_core_mod_f mod, x_core_equal_f equal);
void x_mbin_destroy(x_mbin_t *mbin);
void *x_mbin_find(x_mbin_t *mbin, void *decoy_object);
void x_mbin_iterate_start(x_mbin_t *mbin);
void *x_mbin_iterate_next(x_mbin_t *mbin);
void *x_mbin_remove(x_mbin_t *mbin, void *decoy_object);

#endif
