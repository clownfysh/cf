#include "cf/sage/archetype/system.h"
#include "cf/sage/classify/classifyey.h"
#include "cf/sage/classify/system.h"

struct cf_sage_classify_system_t {
  cf_sage_classify_classifyey_t classifyey;
  void *classify_object;
  cf_x_audit_log_t *log;
};

static void init_classifyey(cf_sage_classify_classifyey_t *classifyey,
    cf_sage_classify_algorithm_t algorithm);

cf_sage_classify_system_t *cf_sage_classify_system_create
(cf_x_case_array_t *classified_objects, cf_sage_classify_algorithm_t algorithm,
    cf_x_audit_log_t *log)
{
  assert(classified_objects);
  assert(cf_x_case_array_get_size(classified_objects) > 0);
  assert(cf_x_core_bitarray_get_size(cf_x_case_array_find
          (classified_objects, 0)) >= 2);
  assert(log);
  cf_sage_classify_system_t *system;

  system = malloc(sizeof *system);
  if (system) {
    system->log = log;
    init_classifyey(&system->classifyey, algorithm);
    system->classify_object = system->classifyey.create(classified_objects,
        log);
    if (!system->classify_object) {
      cf_x_audit_log_trace(log, "clss", "create");
      free(system);
      system = NULL;
    }
  } else {
    cf_x_core_trace("malloc");
  }

  return system;
}

void cf_sage_classify_system_destroy(cf_sage_classify_system_t *system)
{
  assert(system);

  system->classifyey.destroy(system->classify_object);
  free(system);
}

cf_x_core_bit_t cf_sage_classify_system_classify_object(cf_sage_classify_system_t *system,
    cf_x_core_bitarray_t *object)
{
  return system->classifyey.classify_object(system->classify_object, object);
}

cf_x_case_array_t *cf_sage_classify_system_classify_objects
(cf_sage_classify_system_t *system, cf_x_case_array_t *objects)
{
  assert(system);
  assert(objects);
  cf_x_core_bitarray_t *object;
  cf_x_case_array_t *classes;
  unsigned long object_count;
  cf_x_core_bit_t class;
  cf_x_core_bit_t *class_object;
  unsigned long index;

  object_count = cf_x_case_array_get_size(objects);
  classes = cf_x_case_array_create(object_count, cf_x_core_bit_compare,
      cf_x_core_bit_copy, cf_x_core_bit_destroy);
  if (classes) {
    cf_x_case_array_iterate_start(objects);
    index = 0;
    while ((object = cf_x_case_array_iterate_next(objects))) {
      class
        = system->classifyey.classify_object(system->classify_object, object);
      class_object = malloc(sizeof *class_object);
      if (class_object) {
        cf_x_case_array_add(classes, index, class_object);
      } else {
        cf_x_audit_log_trace(system->log, "clss", "malloc");
      }
      index++;
    }
  } else {
    cf_x_core_trace("x_case_array_create");
  }

  return classes;
}

cf_x_core_bool_t cf_sage_classify_system_learn(cf_sage_classify_system_t *system,
    unsigned long max_wall_time_microseconds)
{
  assert(system);
  struct timeval start_time;
  cf_x_core_bool_t success;

  gettimeofday(&start_time, NULL);

  success = cf_x_core_bool_true;
  do {
    if (!system->classifyey.learn(system->classify_object)) {
      success = cf_x_core_bool_false;
      cf_x_audit_log_trace(system->log, "clss", "learn");
    }
  } while (cf_x_core_time_is_remaining_microseconds
      (&start_time, max_wall_time_microseconds));

  return success;
}

cf_x_core_bool_t cf_sage_classify_system_observe_object(cf_sage_classify_system_t *system,
    cf_x_core_bitarray_t *classified_object)
{
  return system->classifyey.observe_object(system->classify_object,
      classified_object);
}

cf_x_core_bool_t cf_sage_classify_system_observe_objects(cf_sage_classify_system_t *system,
    cf_x_case_array_t *classified_objects)
{
  assert(system);
  assert(classified_objects);
  cf_x_core_bool_t success;
  cf_x_core_bitarray_t *object;

  success = cf_x_core_bool_true;

  cf_x_case_array_iterate_start(classified_objects);
  while ((object = cf_x_case_array_iterate_next(classified_objects))) {
    if (!system->classifyey.observe_object(system->classify_object, object)) {
      success = cf_x_core_bool_false;
      cf_x_audit_log_trace(system->log, "clss", "observe_object");
    }
  }

  return success;
}

void init_classifyey(cf_sage_classify_classifyey_t *classifyey,
    cf_sage_classify_algorithm_t algorithm)
{
  assert(classifyey);

  switch (algorithm) {
    default:
    case CF_SAGE_CLASSIFY_ALGORITHM_ARCHETYPE:
      cf_sage_archetype_system_init_classifyey(classifyey);
      break;
  }
}
