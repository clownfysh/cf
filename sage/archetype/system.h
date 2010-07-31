#ifndef cf_sage_archetype_system_h
#define cf_sage_archetype_system_h

#include "cf/x/audit/log.h"
#include "cf/x/case/array.h"
#include "cf/x/core/bitarray.h"
#include "cf/sage/classify/classifyey.h"

struct cf_sage_archetype_system_t;
typedef struct cf_sage_archetype_system_t cf_sage_archetype_system_t;

void *cf_sage_archetype_system_create(cf_x_case_array_t *classified_objects,
    cf_x_audit_log_t *log);

void cf_sage_archetype_system_destroy(void *system_void);

cf_x_core_bit_t cf_sage_archetype_system_classify_object(void *system_void,
    cf_x_core_bitarray_t *object);

void cf_sage_archetype_system_init_classifyey(cf_sage_classify_classifyey_t *classifyey);

cf_x_core_bool_t cf_sage_archetype_system_learn(void *system_void);

cf_x_core_bool_t cf_sage_archetype_system_observe_object(void *system_void,
    cf_x_core_bitarray_t *classified_object);

#endif
