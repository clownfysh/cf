#ifndef inferno_archetype_system_h
#define inferno_archetype_system_h

#include "x/audit/log.h"
#include "x/container/array.h"
#include "x/core/bitarray.h"
#include "inferno/classify/classifyey.h"

struct inferno_archetype_system_t;
typedef struct inferno_archetype_system_t inferno_archetype_system_t;

void *inferno_archetype_system_create(x_container_array_t *classified_objects,
    x_audit_log_t *log);

void inferno_archetype_system_destroy(void *system_void);

x_core_bit_t inferno_archetype_system_classify_object(void *system_void,
    x_core_bitarray_t *object);

void inferno_archetype_system_init_classifyey(inferno_classify_classifyey_t *classifyey);

x_core_bool_t inferno_archetype_system_learn(void *system_void);

x_core_bool_t inferno_archetype_system_observe_object(void *system_void,
    x_core_bitarray_t *classified_object);

#endif
