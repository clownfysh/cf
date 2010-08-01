#ifndef cf_inferno_archetype_system_h
#define cf_inferno_archetype_system_h

#include "cf/x/core/log.h"
#include "cf/x/case/array.h"
#include "cf/x/core/bitarray.h"
#include "cf/inferno/classify/iclassify.h"

struct cf_inferno_archetype_system_t;
typedef struct cf_inferno_archetype_system_t cf_inferno_archetype_system_t;

void *cf_inferno_archetype_system_create(cf_x_case_array_t *classified_objects,
    cf_x_core_log_t *log);

void cf_inferno_archetype_system_destroy(void *system_void);

cf_x_core_bit_t cf_inferno_archetype_system_classify_object(void *system_void,
    cf_x_core_bitarray_t *object);

void cf_inferno_archetype_system_init_iclassify(cf_inferno_classify_iclassify_t *iclassify);

cf_x_core_bool_t cf_inferno_archetype_system_learn(void *system_void);

cf_x_core_bool_t cf_inferno_archetype_system_observe_object(void *system_void,
    cf_x_core_bitarray_t *classified_object);

#endif
