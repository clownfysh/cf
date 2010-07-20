#include "inferno/archetype/system.h"
#include "inferno/classify/classifyey.h"
#include "inferno/classify/system.h"

struct inferno_classify_system_t {
  inferno_classify_classifyey_t classifyey;
  void *classify_object;
  x_audit_log_t *log;
};

static void init_classifyey(inferno_classify_classifyey_t *classifyey,
    inferno_classify_algorithm_t algorithm);

inferno_classify_system_t *inferno_classify_system_create
(x_container_array_t *classified_objects, inferno_classify_algorithm_t algorithm,
    x_audit_log_t *log)
{
  assert(classified_objects);
  assert(x_container_array_get_size(classified_objects) > 0);
  assert(x_core_bitarray_get_size(x_container_array_find
          (classified_objects, 0)) >= 2);
  assert(log);
  inferno_classify_system_t *system;

  system = malloc(sizeof *system);
  if (system) {
    system->log = log;
    init_classifyey(&system->classifyey, algorithm);
    system->classify_object = system->classifyey.create(classified_objects,
        log);
    if (!system->classify_object) {
      x_audit_log_trace(log, "clss", "create");
      free(system);
      system = NULL;
    }
  } else {
    x_core_trace("malloc");
  }

  return system;
}

void inferno_classify_system_destroy(inferno_classify_system_t *system)
{
  assert(system);

  system->classifyey.destroy(system->classify_object);
  free(system);
}

x_core_bit_t inferno_classify_system_classify_object(inferno_classify_system_t *system,
    x_core_bitarray_t *object)
{
  return system->classifyey.classify_object(system->classify_object, object);
}

x_container_array_t *inferno_classify_system_classify_objects
(inferno_classify_system_t *system, x_container_array_t *objects)
{
  assert(system);
  assert(objects);
  x_core_bitarray_t *object;
  x_container_array_t *classes;
  unsigned long object_count;
  x_core_bit_t class;
  x_core_bit_t *class_object;
  unsigned long index;

  object_count = x_container_array_get_size(objects);
  classes = x_container_array_create(object_count, x_core_bit_compare,
      x_core_bit_copy, x_core_bit_destroy);
  if (classes) {
    x_container_array_iterate_start(objects);
    index = 0;
    while ((object = x_container_array_iterate_next(objects))) {
      class
        = system->classifyey.classify_object(system->classify_object, object);
      class_object = malloc(sizeof *class_object);
      if (class_object) {
        x_container_array_add(classes, index, class_object);
      } else {
        x_audit_log_trace(system->log, "clss", "malloc");
      }
      index++;
    }
  } else {
    x_core_trace("x_container_array_create");
  }

  return classes;
}

x_core_bool_t inferno_classify_system_learn(inferno_classify_system_t *system,
    unsigned long max_wall_time_microseconds)
{
  assert(system);
  struct timeval start_time;
  x_core_bool_t success;

  gettimeofday(&start_time, NULL);

  success = x_core_bool_true;
  do {
    if (!system->classifyey.learn(system->classify_object)) {
      success = x_core_bool_false;
      x_audit_log_trace(system->log, "clss", "learn");
    }
  } while (x_core_time_is_remaining_microseconds
      (&start_time, max_wall_time_microseconds));

  return success;
}

x_core_bool_t inferno_classify_system_observe_object(inferno_classify_system_t *system,
    x_core_bitarray_t *classified_object)
{
  return system->classifyey.observe_object(system->classify_object,
      classified_object);
}

x_core_bool_t inferno_classify_system_observe_objects(inferno_classify_system_t *system,
    x_container_array_t *classified_objects)
{
  assert(system);
  assert(classified_objects);
  x_core_bool_t success;
  x_core_bitarray_t *object;

  success = x_core_bool_true;

  x_container_array_iterate_start(classified_objects);
  while ((object = x_container_array_iterate_next(classified_objects))) {
    if (!system->classifyey.observe_object(system->classify_object, object)) {
      success = x_core_bool_false;
      x_audit_log_trace(system->log, "clss", "observe_object");
    }
  }

  return success;
}

void init_classifyey(inferno_classify_classifyey_t *classifyey,
    inferno_classify_algorithm_t algorithm)
{
  assert(classifyey);

  switch (algorithm) {
    default:
    case INFERNO_CLASSIFY_ALGORITHM_ARCHETYPE:
      inferno_archetype_system_init_classifyey(classifyey);
      break;
  }
}
