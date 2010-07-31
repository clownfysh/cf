#ifndef cf_inferno_memory_system_h
#define cf_inferno_memory_system_h

#include "cf/x/case/array.h"
#include "cf/x/core/bool.h"

struct cf_inferno_memory_system_t;
typedef struct cf_inferno_memory_system_t cf_inferno_memory_system_t;

cf_x_core_bool_t cf_inferno_memory_system_add(cf_inferno_memory_system_t *system,
    unsigned long base, unsigned long link, unsigned long corollary);

cf_inferno_memory_system_t *cf_inferno_memory_system_create(unsigned long size,
    unsigned short density);

void cf_inferno_memory_system_destroy(cf_inferno_memory_system_t *system);

cf_x_core_bool_t cf_inferno_memory_system_find(cf_inferno_memory_system_t *system,
    unsigned long base, unsigned long link, unsigned long corollary);

cf_x_case_array_t *cf_inferno_memory_system_find_cloud(cf_inferno_memory_system_t *system,
    unsigned long base, unsigned long cloud_size_max);

cf_x_core_bool_t cf_inferno_memory_system_remove(cf_inferno_memory_system_t *system,
    unsigned long base, unsigned long link, unsigned long corollary);

#endif
