#include "cf/inferno/archetype/system.h"
#include "cf/x/case/array.h"
#include "cf/x/core/basic/double.h"
#include "cf/x/core/basic/unsigned_short.h"
#include "cf/x/core/tools.h"

#define RECALCULATE_GRANULARITY 2

struct cf_inferno_archetype_system_t {
  unsigned long attribute_count;
  cf_x_case_array_t *poles;
  cf_x_case_array_t *weights;
  double weight_total;
  cf_x_case_array_t *classified_objects;
  cf_x_core_bool_t new_objects_since_last_learn;
  cf_x_core_log_t *log;
};

static cf_x_core_bool_t calculate_poles_and_weights
(cf_inferno_archetype_system_t *system);

cf_x_core_bool_t calculate_poles_and_weights
(cf_inferno_archetype_system_t *system)
{
  assert(system);
  cf_x_core_bool_t success;
  unsigned long count_0;
  unsigned long count_1;
  unsigned long matcx_0;
  unsigned long matcx_1;
  cf_x_core_bit_t pole;
  cf_x_core_bit_t *pole_copy;
  unsigned long attribute_index;
  cf_x_core_bitarray_t *object;
  cf_x_core_bit_t attribute_value;
  cf_x_core_bit_t class;
  double weight;
  double *weight_copy;

  success = cf_x_core_bool_true;
  system->weight_total = 0.0;

  for (attribute_index = 0; attribute_index < system->attribute_count;
       attribute_index++) {
    count_0 = 0;
    count_1 = 0;
    matcx_0 = 0;
    matcx_1 = 0;
    cf_x_case_array_iterate_start(system->classified_objects);
    while ((object
            = cf_x_case_array_iterate_next(system->classified_objects))) {
      attribute_value = cf_x_core_bitarray_get_bit(object, attribute_index);
      class = cf_x_core_bitarray_get_bit(object, system->attribute_count);
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
    pole_copy = cf_x_core_bit_copy(&pole);
    if (pole_copy) {
      cf_x_case_array_add(system->poles, attribute_index, pole_copy);
    } else {
      success = cf_x_core_bool_false;
      cf_x_core_log_trace(system->log, "arch", "x_core_bit_copy");
    }
    weight_copy = cf_x_core_basic_double_copy(&weight);
    if (weight_copy) {
      cf_x_case_array_add(system->weights, attribute_index, weight_copy);
    } else {
      success = cf_x_core_bool_false;
      cf_x_core_log_trace(system->log, "arch", "cf_x_core_basic_double_copy");
    }
  }

  return success;
}

void *cf_inferno_archetype_system_create(cf_x_case_array_t *classified_objects,
    cf_x_core_log_t *log)
{
  assert(classified_objects);
  assert(cf_x_case_array_get_size(classified_objects) > 0);
  assert(cf_x_core_bitarray_get_size(cf_x_case_array_find
          (classified_objects, 0)) >= 2);
  assert(log);
  cf_inferno_archetype_system_t *system;
  cf_x_core_bool_t so_far_so_good;
  cf_x_core_bitarray_t *object;

  system = malloc(sizeof *system);
  if (system) {
    system->log = log;
    system->poles = NULL;
    system->weights = NULL;
    object = cf_x_case_array_find(classified_objects, 0);
    system->attribute_count = cf_x_core_bitarray_get_size(object) - 1;
    system->new_objects_since_last_learn = cf_x_core_bool_true;
    system->classified_objects = cf_x_case_array_copy(classified_objects);
    if (system->classified_objects) {
      so_far_so_good = cf_x_core_bool_true;
      cf_x_core_log_trace(log, "arch", "x_case_array_copy");
    } else {
      so_far_so_good = cf_x_core_bool_false;
    }
  } else {
    so_far_so_good = cf_x_core_bool_false;
    cf_x_core_trace("malloc");
  }

  if (so_far_so_good) {
    system->poles = cf_x_case_array_create(system->attribute_count,
        cf_x_core_unsigned_short_compare, cf_x_core_unsigned_short_copy,
        cf_x_core_unsigned_short_destroy);
    if (!system->poles) {
      so_far_so_good = cf_x_core_bool_false;
      cf_x_core_log_trace(log, "arch", "x_case_array_create");
    }
  }

  if (so_far_so_good) {
    system->weights = cf_x_case_array_create(system->attribute_count,
        cf_x_core_basic_double_compare, cf_x_core_basic_double_copy, cf_x_core_basic_double_destroy);
    if (!system->weights) {
      so_far_so_good = cf_x_core_bool_false;
      cf_x_core_log_trace(log, "arch", "x_case_array_create");
    }
  }

  if (so_far_so_good) {
    if (!cf_inferno_archetype_system_learn(system)) {
      so_far_so_good = cf_x_core_bool_false;
      cf_x_core_log_trace(log, "arch", "inferno_archetype_system_learn");
    }
  }

  if (!so_far_so_good && system) {
    if (system->classified_objects) {
      cf_x_case_array_destroy(system->classified_objects);
    }
    if (system->poles) {
      cf_x_case_array_destroy(system->poles);
    }
    if (system->weights) {
      cf_x_case_array_destroy(system->weights);
    }
  }

  return system;
}

void cf_inferno_archetype_system_destroy(void *system_void)
{
  assert(system_void);
  cf_inferno_archetype_system_t *system;

  system = system_void;

  cf_x_case_array_destroy(system->classified_objects);
  cf_x_case_array_destroy(system->poles);
  cf_x_case_array_destroy(system->weights);
  free(system);
}

cf_x_core_bit_t cf_inferno_archetype_system_classify_object(void *system_void,
    cf_x_core_bitarray_t *object)
{
  assert(system_void);
  assert(object);
  assert(cf_x_core_bitarray_get_size(object)
      >= ((cf_inferno_archetype_system_t *) system_void)->attribute_count);
  cf_inferno_archetype_system_t *system;
  cf_x_core_bit_t class;
  unsigned long attribute_index;
  double object_weight;
  double *attribute_weight;
  cf_x_core_bit_t *pole;
  cf_x_core_bit_t attribute_value;
  double object_essence;

  system = system_void;
  object_weight = 0.0;

  for (attribute_index = 0; attribute_index < system->attribute_count;
       attribute_index++) {
    pole = cf_x_case_array_find(system->poles, attribute_index);
    attribute_value = cf_x_core_bitarray_get_bit(object, attribute_index);
    if (*pole == attribute_value) {
      attribute_weight
        = cf_x_case_array_find(system->weights, attribute_index);
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

void cf_inferno_archetype_system_init_iclassify(cf_inferno_classify_iclassify_t *iclassify)
{
  return cf_inferno_classify_iclassify_init(iclassify, cf_inferno_archetype_system_create,
      cf_inferno_archetype_system_destroy, cf_inferno_archetype_system_classify_object,
      cf_inferno_archetype_system_learn, cf_inferno_archetype_system_observe_object);
}

cf_x_core_bool_t cf_inferno_archetype_system_learn(void *system_void)
{
  assert(system_void);
  cf_inferno_archetype_system_t *system;
  cf_x_core_bool_t success;

  system = system_void;

  success = cf_x_core_bool_true;

  if (system->new_objects_since_last_learn) {
    if (!calculate_poles_and_weights(system)) {
      success = cf_x_core_bool_false;
      cf_x_core_log_trace(system->log, "arch", "calculate_poles");
    }
    if (success) {
      system->new_objects_since_last_learn = cf_x_core_bool_false;
    }
  }

  return success;
}

cf_x_core_bool_t cf_inferno_archetype_system_observe_object(void *system_void,
    cf_x_core_bitarray_t *classified_object)
{
  assert(system_void);
  assert(classified_object);
  cf_inferno_archetype_system_t *system;
  cf_x_core_bool_t success;
  cf_x_core_bitarray_t *classified_object_copy;

  system = system_void;

  classified_object_copy = cf_x_core_bitarray_copy(classified_object);
  if (classified_object_copy) {
    system->new_objects_since_last_learn = cf_x_core_bool_true;
    cf_x_case_array_replace_random(system->classified_objects,
        classified_object_copy);
  } else {
    success = cf_x_core_bool_false;
    cf_x_core_log_trace(system->log, "arch", "x_core_bitarray_copy");
  }

  return success;
}
