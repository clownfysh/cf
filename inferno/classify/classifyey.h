#ifndef inferno_classify_classifyey_h
#define inferno_classify_classifyey_h

#include "x/audit/log.h"
#include "x/container/array.h"
#include "x/core/bitarray.h"

typedef void *(*inferno_classify_classifyey_create_f)
  (x_container_array_t *classified_objects, x_audit_log_t *log);

typedef void (*inferno_classify_classifyey_destroy_f)(void *system_void);

typedef x_core_bit_t (*inferno_classify_classifyey_classify_object_f)
(void *system_void, x_core_bitarray_t *object);

typedef x_core_bool_t (*inferno_classify_classifyey_learn_f)(void *system_void);

typedef x_core_bool_t (*inferno_classify_classifyey_observe_object_f)
(void *system_void, x_core_bitarray_t *classified_object);

struct inferno_classify_classifyey_t {
  inferno_classify_classifyey_create_f create;
  inferno_classify_classifyey_destroy_f destroy;
  inferno_classify_classifyey_classify_object_f classify_object;
  inferno_classify_classifyey_learn_f learn;
  inferno_classify_classifyey_observe_object_f observe_object;
};
typedef struct inferno_classify_classifyey_t inferno_classify_classifyey_t;

void inferno_classify_classifyey_free(inferno_classify_classifyey_t *classifyey);

void inferno_classify_classifyey_init(inferno_classify_classifyey_t *classifyey,
    inferno_classify_classifyey_create_f create,
    inferno_classify_classifyey_destroy_f destroy,
    inferno_classify_classifyey_classify_object_f classify_object,
    inferno_classify_classifyey_learn_f learn,
    inferno_classify_classifyey_observe_object_f observe_object);

#endif
