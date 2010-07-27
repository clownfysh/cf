#ifndef x_container_mbin_h
#define x_container_mbin_h

#include "x/core/object.h"
#include "x/core/tools.h"

struct x_container_mbin_t;
typedef struct x_container_mbin_t x_container_mbin_t;

x_core_bool_t x_container_mbin_add(x_container_mbin_t *mbin, void *object);

void x_container_mbin_clear(x_container_mbin_t *mbin);

x_container_mbin_t *x_container_mbin_create(x_core_mod_f mod,
    x_core_equal_f equal,
    x_core_destroy_f destroy);

void x_container_mbin_destroy(x_container_mbin_t *mbin);

void *x_container_mbin_find(x_container_mbin_t *mbin, void *decoy_object);

unsigned long x_container_mbin_get_size(x_container_mbin_t *mbin);

void x_container_mbin_iterate_remove(x_container_mbin_t *mbin);

void x_container_mbin_iterate_start(x_container_mbin_t *mbin);

void *x_container_mbin_iterate_next(x_container_mbin_t *mbin);

x_core_bool_t x_container_mbin_remove(x_container_mbin_t *mbin,
    void *decoy_object);

#endif
