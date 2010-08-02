#ifndef cf_inferno_classify_system_h
#define cf_inferno_classify_system_h

#include "cf/x/core/log.h"
#include "cf/x/case/array.h"
#include "cf/x/core/bit.h"
#include "cf/x/core/bitarray.h"
#include "cf/x/core/tools.h"
#include "cf/inferno/core/goal.h"
#include "cf/inferno/classify/algorithm.h"

struct cf_inferno_classify_system_t;
typedef struct cf_inferno_classify_system_t cf_inferno_classify_system_t;

cf_inferno_classify_system_t *cf_inferno_classify_system_create
(cf_x_case_array_t *classified_objects,
    cf_inferno_classify_algorithm_t algorithm, cf_x_core_log_t *log);

void cf_inferno_classify_system_destroy(cf_inferno_classify_system_t *system);

cf_x_core_bit_t cf_inferno_classify_system_classify_object
(cf_inferno_classify_system_t *system, cf_x_core_bitarray_t *object);

cf_x_case_array_t *cf_inferno_classify_system_classify_objects
(cf_inferno_classify_system_t *system, cf_x_case_array_t *objects);

cf_x_core_bool_t cf_inferno_classify_system_learn
(cf_inferno_classify_system_t *system,
    unsigned long max_wall_time_microseconds);

cf_x_core_bool_t cf_inferno_classify_system_observe_object
(cf_inferno_classify_system_t *system,
    cf_x_core_bitarray_t *classified_object);

cf_x_core_bool_t cf_inferno_classify_system_observe_objects
(cf_inferno_classify_system_t *system, cf_x_case_array_t *classified_objects);

#endif
