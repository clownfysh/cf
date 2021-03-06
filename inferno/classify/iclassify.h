#ifndef cf_inferno_classify_iclassify_h
#define cf_inferno_classify_iclassify_h

#include "cf/x/core/log.h"
#include "cf/x/case/array.h"
#include "cf/x/core/bitarray.h"

typedef void *(*cf_inferno_classify_iclassify_create_f)
  (cf_x_case_array_t *classified_objects, cf_x_core_log_t *log);

typedef void (*cf_inferno_classify_iclassify_destroy_f)(void *system_void);

typedef cf_x_core_bit_t (*cf_inferno_classify_iclassify_classify_object_f)
(void *system_void, cf_x_core_bitarray_t *object);

typedef cf_x_core_bool_t (*cf_inferno_classify_iclassify_learn_f)(void *system_void);

typedef cf_x_core_bool_t (*cf_inferno_classify_iclassify_observe_object_f)
(void *system_void, cf_x_core_bitarray_t *classified_object);

struct cf_inferno_classify_iclassify_t {
  cf_inferno_classify_iclassify_create_f create;
  cf_inferno_classify_iclassify_destroy_f destroy;
  cf_inferno_classify_iclassify_classify_object_f classify_object;
  cf_inferno_classify_iclassify_learn_f learn;
  cf_inferno_classify_iclassify_observe_object_f observe_object;
};
typedef struct cf_inferno_classify_iclassify_t cf_inferno_classify_iclassify_t;

void cf_inferno_classify_iclassify_free(cf_inferno_classify_iclassify_t *iclassify);

void cf_inferno_classify_iclassify_init(cf_inferno_classify_iclassify_t *iclassify,
    cf_inferno_classify_iclassify_create_f create,
    cf_inferno_classify_iclassify_destroy_f destroy,
    cf_inferno_classify_iclassify_classify_object_f classify_object,
    cf_inferno_classify_iclassify_learn_f learn,
    cf_inferno_classify_iclassify_observe_object_f observe_object);

#endif
