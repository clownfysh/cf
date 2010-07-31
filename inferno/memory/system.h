#ifndef inferno_memory_system_h
#define inferno_memory_system_h

#include "x/case/array.h"
#include "x/core/bool.h"

struct inferno_memory_system_t;
typedef struct inferno_memory_system_t inferno_memory_system_t;

x_core_bool_t inferno_memory_system_add(inferno_memory_system_t *system,
    unsigned long base, unsigned long link, unsigned long corollary);

inferno_memory_system_t *inferno_memory_system_create(unsigned long size,
    unsigned short density);

void inferno_memory_system_destroy(inferno_memory_system_t *system);

x_core_bool_t inferno_memory_system_find(inferno_memory_system_t *system,
    unsigned long base, unsigned long link, unsigned long corollary);

x_case_array_t *inferno_memory_system_find_cloud(inferno_memory_system_t *system,
    unsigned long base, unsigned long cloud_size_max);

x_core_bool_t inferno_memory_system_remove(inferno_memory_system_t *system,
    unsigned long base, unsigned long link, unsigned long corollary);

#endif
