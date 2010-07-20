#include "inferno/archetype/system.h"
#include "inferno/classify/classifyey.h"
#include "inferno/classify/system.h"

struct inferno_classify_system_t {
  inferno_classify_classifyey_t classifyey;
  void *classify_object;
  h_audit_log_t *log;
};

static void init_classifyey(inferno_classify_classifyey_t *classifyey,
    inferno_classify_algorithm_t algorithm);

inferno_classify_system_t *inferno_classify_system_create
(h_container_array_t *classified_objects, inferno_classify_algorithm_t algorithm,
    h_audit_log_t *log)
{
  assert(classified_objects);
  assert(h_container_array_get_size(classified_objects) > 0);
  assert(h_core_bitarray_get_size(h_container_array_find
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
      h_audit_log_trace(log, "clss", "create");
      free(system);
      system = NULL;
    }
  } else {
    h_core_trace("malloc");
  }

  return system;
}

void inferno_classify_system_destroy(inferno_classify_system_t *system)
{
  assert(system);

  system->classifyey.destroy(system->classify_object);
  free(system);
}

h_core_bit_t inferno_classify_system_classify_object(inferno_classify_system_t *system,
    h_core_bitarray_t *object)
{
  return system->classifyey.classify_object(system->classify_object, object);
}

h_container_array_t *inferno_classify_system_classify_objects
(inferno_classify_system_t *system, h_container_array_t *objects)
{
  assert(system);
  assert(objects);
  h_core_bitarray_t *object;
  h_container_array_t *classes;
  unsigned long object_count;
  h_core_bit_t class;
  h_core_bit_t *class_object;
  unsigned long index;

  object_count = h_container_array_get_size(objects);
  classes = h_container_array_create(object_count, h_core_bit_compare,
      h_core_bit_copy, h_core_bit_destroy);
  if (classes) {
    h_container_array_iterate_start(objects);
    index = 0;
    while ((object = h_container_array_iterate_next(objects))) {
      class
        = system->classifyey.classify_object(system->classify_object, object);
      class_object = malloc(sizeof *class_object);
      if (class_object) {
        h_container_array_add(classes, index, class_object);
      } else {
        h_audit_log_trace(system->log, "clss", "malloc");
      }
      index++;
    }
  } else {
    h_core_trace("h_container_array_create");
  }

  return classes;
}

h_core_bool_t inferno_classify_system_learn(inferno_classify_system_t *system,
    unsigned long max_wall_time_microseconds)
{
  assert(system);
  struct timeval start_time;
  h_core_bool_t success;

  gettimeofday(&start_time, NULL);

  success = h_core_bool_true;
  do {
    if (!system->classifyey.learn(system->classify_object)) {
      success = h_core_bool_false;
      h_audit_log_trace(system->log, "clss", "learn");
    }
  } while (h_core_time_is_remaining_microseconds
      (&start_time, max_wall_time_microseconds));

  return success;
}

h_core_bool_t inferno_classify_system_observe_object(inferno_classify_system_t *system,
    h_core_bitarray_t *classified_object)
{
  return system->classifyey.observe_object(system->classify_object,
      classified_object);
}

h_core_bool_t inferno_classify_system_observe_objects(inferno_classify_system_t *system,
    h_container_array_t *classified_objects)
{
  assert(system);
  assert(classified_objects);
  h_core_bool_t success;
  h_core_bitarray_t *object;

  success = h_core_bool_true;

  h_container_array_iterate_start(classified_objects);
  while ((object = h_container_array_iterate_next(classified_objects))) {
    if (!system->classifyey.observe_object(system->classify_object, object)) {
      success = h_core_bool_false;
      h_audit_log_trace(system->log, "clss", "observe_object");
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
