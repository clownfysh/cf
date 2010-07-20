#include "inferno/archetype/system.h"
#include "x/container/array.h"
#include "x/core/double.h"
#include "x/core/tools.h"
#include "x/core/unsigned_short.h"

#define RECALCULATE_GRANULARITY 2

struct inferno_archetype_system_t {
  unsigned long attribute_count;
  x_container_array_t *poles;
  x_container_array_t *weights;
  double weight_total;
  x_container_array_t *classified_objects;
  x_core_bool_t new_objects_since_last_learn;
  x_audit_log_t *log;
};

static x_core_bool_t calculate_poles_and_weights
(inferno_archetype_system_t *system);

x_core_bool_t calculate_poles_and_weights(inferno_archetype_system_t *system)
{
  assert(system);
  x_core_bool_t success;
  unsigned long count_0;
  unsigned long count_1;
  unsigned long matcx_0;
  unsigned long matcx_1;
  x_core_bit_t pole;
  x_core_bit_t *pole_copy;
  unsigned long attribute_index;
  x_core_bitarray_t *object;
  x_core_bit_t attribute_value;
  x_core_bit_t class;
  double weight;
  double *weight_copy;

  success = x_core_bool_true;
  system->weight_total = 0.0;

  for (attribute_index = 0; attribute_index < system->attribute_count;
       attribute_index++) {
    count_0 = 0;
    count_1 = 0;
    matcx_0 = 0;
    matcx_1 = 0;
    x_container_array_iterate_start(system->classified_objects);
    while ((object
            = x_container_array_iterate_next(system->classified_objects))) {
      attribute_value = x_core_bitarray_get_bit(object, attribute_index);
      class = x_core_bitarray_get_bit(object, system->attribute_count);
      if (0 == attribute_value) {
        count_0++;
        if (0 == class) {
          matcx_0++;
        }
      } else {
        count_1++;
        if (1 == class) {
          matcx_1++;
        }
      }
    }
    if (count_0 > count_1) {
      pole = 0;
      weight = matcx_0 / (double) count_0;
    } else {
      pole = 1;
      weight = matcx_1 / (double) count_1;
    }
    system->weight_total += weight;
    pole_copy = x_core_bit_copy(&pole);
    if (pole_copy) {
      x_container_array_add(system->poles, attribute_index, pole_copy);
    } else {
      success = x_core_bool_false;
      x_audit_log_trace(system->log, "arch", "x_core_bit_copy");
    }
    weight_copy = x_core_double_copy(&weight);
    if (weight_copy) {
      x_container_array_add(system->weights, attribute_index, weight_copy);
    } else {
      success = x_core_bool_false;
      x_audit_log_trace(system->log, "arch", "inferno_core_double_copy");
    }
  }

  return success;
}

void *inferno_archetype_system_create(x_container_array_t *classified_objects,
    x_audit_log_t *log)
{
  assert(classified_objects);
  assert(x_container_array_get_size(classified_objects) > 0);
  assert(x_core_bitarray_get_size(x_container_array_find
          (classified_objects, 0)) >= 2);
  assert(log);
  inferno_archetype_system_t *system;
  x_core_bool_t so_far_so_good;
  x_core_bitarray_t *object;

  system = malloc(sizeof *system);
  if (system) {
    system->log = log;
    system->poles = NULL;
    system->weights = NULL;
    object = x_container_array_find(classified_objects, 0);
    system->attribute_count = x_core_bitarray_get_size(object) - 1;
    system->new_objects_since_last_learn = x_core_bool_true;
    system->classified_objects = x_container_array_copy(classified_objects);
    if (system->classified_objects) {
      so_far_so_good = x_core_bool_true;
      x_audit_log_trace(log, "arch", "x_container_array_copy");
    } else {
      so_far_so_good = x_core_bool_false;
    }
  } else {
    so_far_so_good = x_core_bool_false;
    x_core_trace("malloc");
  }

  if (so_far_so_good) {
    system->poles = x_container_array_create(system->attribute_count,
        x_core_unsigned_short_compare, x_core_unsigned_short_copy,
        x_core_unsigned_short_destroy);
    if (!system->poles) {
      so_far_so_good = x_core_bool_false;
      x_audit_log_trace(log, "arch", "x_container_array_create");
    }
  }

  if (so_far_so_good) {
    system->weights = x_container_array_create(system->attribute_count,
        x_core_double_compare, x_core_double_copy, x_core_double_destroy);
    if (!system->weights) {
      so_far_so_good = x_core_bool_false;
      x_audit_log_trace(log, "arch", "x_container_array_create");
    }
  }

  if (so_far_so_good) {
    if (!inferno_archetype_system_learn(system)) {
      so_far_so_good = x_core_bool_false;
      x_audit_log_trace(log, "arch", "inferno_archetype_system_learn");
    }
  }

  if (!so_far_so_good && system) {
    if (system->classified_objects) {
      x_container_array_destroy(system->classified_objects);
    }
    if (system->poles) {
      x_container_array_destroy(system->poles);
    }
    if (system->weights) {
      x_container_array_destroy(system->weights);
    }
  }

  return system;
}

void inferno_archetype_system_destroy(void *system_void)
{
  assert(system_void);
  inferno_archetype_system_t *system;

  system = system_void;

  x_container_array_destroy(system->classified_objects);
  x_container_array_destroy(system->poles);
  x_container_array_destroy(system->weights);
  free(system);
}

x_core_bit_t inferno_archetype_system_classify_object(void *system_void,
    x_core_bitarray_t *object)
{
  assert(system_void);
  assert(object);
  assert(x_core_bitarray_get_size(object)
      >= ((inferno_archetype_system_t *) system_void)->attribute_count);
  inferno_archetype_system_t *system;
  x_core_bit_t class;
  unsigned long attribute_index;
  double object_weight;
  double *attribute_weight;
  x_core_bit_t *pole;
  x_core_bit_t attribute_value;
  double object_essence;

  system = system_void;
  object_weight = 0.0;

  for (attribute_index = 0; attribute_index < system->attribute_count;
       attribute_index++) {
    pole = x_container_array_find(system->poles, attribute_index);
    attribute_value = x_core_bitarray_get_bit(object, attribute_index);
    if (*pole == attribute_value) {
      attribute_weight
        = x_container_array_find(system->weights, attribute_index);
      object_weight += *attribute_weight;
    }
  }

  object_essence = object_weight / system->weight_total;
  if (object_essence > 0.5) {
    class = 1;
  } else {
    class = 0;
  }

  return class;
}

void inferno_archetype_system_init_classifyey(inferno_classify_classifyey_t *classifyey)
{
  return inferno_classify_classifyey_init(classifyey, inferno_archetype_system_create,
      inferno_archetype_system_destroy, inferno_archetype_system_classify_object,
      inferno_archetype_system_learn, inferno_archetype_system_observe_object);
}

x_core_bool_t inferno_archetype_system_learn(void *system_void)
{
  assert(system_void);
  inferno_archetype_system_t *system;
  x_core_bool_t success;

  system = system_void;

  success = x_core_bool_true;

  if (system->new_objects_since_last_learn) {
    if (!calculate_poles_and_weights(system)) {
      success = x_core_bool_false;
      x_audit_log_trace(system->log, "arch", "calculate_poles");
    }
    if (success) {
      system->new_objects_since_last_learn = x_core_bool_false;
    }
  }

  return success;
}

x_core_bool_t inferno_archetype_system_observe_object(void *system_void,
    x_core_bitarray_t *classified_object)
{
  assert(system_void);
  assert(classified_object);
  inferno_archetype_system_t *system;
  x_core_bool_t success;
  x_core_bitarray_t *classified_object_copy;

  system = system_void;

  classified_object_copy = x_core_bitarray_copy(classified_object);
  if (classified_object_copy) {
    system->new_objects_since_last_learn = x_core_bool_true;
    x_container_array_replace_random(system->classified_objects,
        classified_object_copy);
  } else {
    success = x_core_bool_false;
    x_audit_log_trace(system->log, "arch", "x_core_bitarray_copy");
  }

  return success;
}
