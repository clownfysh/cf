#ifndef i_mbin_h
#define i_mbin_h

#include "i/object.h"
#include "i/tools.h"

struct i_mbin_t;
typedef struct i_mbin_t i_mbin_t;

i_bool_t i_mbin_add(i_mbin_t *mbin, void *object);
i_mbin_t *i_mbin_create(i_mod_f mod, i_equal_f equal);
void i_mbin_destroy(i_mbin_t *mbin);
void *i_mbin_find(i_mbin_t *mbin, void *decoy_object);
void i_mbin_iterate_start(i_mbin_t *mbin);
void *i_mbin_iterate_next(i_mbin_t *mbin);
void *i_mbin_remove(i_mbin_t *mbin, void *decoy_object);

#endif
