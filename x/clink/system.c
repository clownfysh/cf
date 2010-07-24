#include "x/clink/system.h"
#include "x/core/tools.h"

struct x_clink_system_t {
  x_clink_concept_t **concepts;
  unsigned long max_concepts;
  unsigned long max_links;
  x_core_compare_f compare;
  x_core_destroy_f destroy;
};

static x_core_bool_t think_train(x_clink_system_t *system,
    x_clink_system_think_f think, unsigned long max_concepts,
    unsigned long total_concepts_so_far);

static x_core_bool_t think_tree(x_clink_system_t *system,
    x_clink_system_think_f think, unsigned long max_concepts,
    unsigned long total_concepts_so_far);

x_core_bool_t think_train(x_clink_system_t *system,
    x_clink_system_think_f think, unsigned long max_concepts,
    unsigned long total_concepts_so_far)
{
  assert(system);
  assert(think);
  x_core_bool_t success = x_core_bool_true;

  if (total_concepts_so_far <= max_concepts) {
    fz;
  }

  return success;
}

x_core_bool_t think_tree(x_clink_system_t *system,
    x_clink_system_think_f think, unsigned long max_concepts,
    unsigned long total_concepts_so_far)
{
  return x_core_bool_false;
}

x_clink_system_t *x_clink_system_create(unsigned long max_concepts,
    unsigned long max_links, x_core_compare_f compare,
    x_core_destroy_f destroy)
{
  assert(compare);
  x_clink_system_t *system;

  system = malloc(sizeof *system);
  if (system) {
    system->max_concepts = max_concepts;
    system->max_links = max_links;
    system->compare = compare;
    system->destroy = destroy;
    system->concepts = malloc((sizeof *system->concepts) * max_concepts);
    if (system->concepts) {
      memset(system->concepts, 0, (sizeof *system->concepts) * max_concepts);
    } else {
      x_trace("malloc");
      free(system);
      system = NULL;
    }
  } else {
    x_trace("malloc");
  }

  return system;
}

void x_clink_system_destroy(x_clink_system_t *system)
{
  assert(system);
  unsigned long concept;

  for (concept = 0; concept < system->max_concepts; concept++) {
    x_clink_concept_destroy(*(system->concepts + concept));
  }
  free(system);
  system = NULL;
}

x_clink_concept_t *x_clink_system_get_concept(x_clink_system_t *system,
    unsigned long concept_index)
{
  assert(system);
  assert(concept_index < system->max_concepts);
  return *(system->concepts + concept_index);
}

void *x_clink_system_get_linked_object(x_clink_system_t *system,
    unsigned long concept_index, unsigned long object_index)
{
  assert(system);
  assert(concept_index < system->max_concepts);
  assert(object_index < system->max_links);
  void *object;

  if (*(system->concepts + concept_index)) {
    object = x_clink_concept_get_linked_object
      (*(system->concepts + concept_index), object_index);
  } else {
    object = NULL;
  }

  return object;
}

void *x_clink_system_get_object(x_clink_system_t *system,
    unsigned long concept_index)
{
  assert(system);
  assert(concept_index < system->max_concepts);
  void *object;

  if (*(system->concepts + concept_index)) {
    object = x_clink_concept_get_object(*(system->concepts + concept_index));
  } else {
    object = NULL;
  }

  return object;
}

void x_clink_system_link(x_clink_system_t *system, void *object_a,
    void *object_b)
{
  assert(system);
  assert(object_a);
  assert(object_b);
  unsigned long i;
  x_clink_concept_t *temporary;
  x_core_bool_t noted = x_core_bool_false;

  for (i = 0; (i < system->max_concepts) && (*(system->concepts + i)); i++) {
    if (0 == system->compare
        (x_clink_concept_get_object(*(system->concepts + i)), object_a)) {
      x_clink_concept_note_object(*(system->concepts + i), object_b);
      if (i > 0) {
        temporary = *(system->concepts + (i - 1));
        *(system->concepts + (i - 1)) = *(system->concepts + i);
        *(system->concepts + i) = temporary;
      }
      noted = x_core_bool_true;
      break;
    }
  }

  if (!noted) {
    for (i = 0; i < system->max_concepts; i++) {
      if (!*(system->concepts + i)) {
        *(system->concepts + i)
          = x_clink_concept_create(object_a, system->max_links,
              system->compare, system->destroy);
        if (*(system->concepts + i)) {
          x_clink_concept_note_object(*(system->concepts + i), object_b);
        } else {
          x_trace("x_clink_concept_create");
        }
        noted = x_core_bool_true;
        break;
      }
    }
  }

  if (!noted) {
    assert(*(system->concepts + (system->max_concepts - 1)));
    assert(0 != system->compare(x_clink_concept_get_object
            (*(system->concepts + (system->max_concepts - 1))), object_a));
    x_clink_concept_destroy(*(system->concepts + (system->max_concepts - 1)));
    *(system->concepts + (system->max_concepts - 1))
      = x_clink_concept_create(object_a, system->max_links, system->compare,
          system->destroy);
    if (*(system->concepts + (system->max_concepts - 1))) {
      x_clink_concept_note_object
        (*(system->concepts + (system->max_concepts - 1)), object_b);
    } else {
      x_trace("x_clink_concept_create");
    }
  }
}

void x_clink_system_print(x_clink_system_t *system,
    x_core_get_as_string_f get_as_string)
{
  assert(system);
  assert(get_as_string);
  unsigned long i;

  printf("clink::\n");
  for (i = 0; i < system->max_concepts; i++) {
    if (*(system->concepts + i)) {
      x_clink_concept_print(*(system->concepts + i), get_as_string);
    } else {
      printf("---\n");
    }
  }
}

x_core_bool_t x_clink_system_think_train(x_clink_system_t *system,
    x_clink_system_think_f think, unsigned long max_concepts)
{
  assert(system);
  assert(think);
  return think_train(system, think, max_concepts, 0);
}

x_core_bool_t x_clink_system_think_tree(x_clink_system_t *system,
    x_clink_system_think_f think, unsigned long max_concepts)
{
  assert(system);
  assert(think);
  return think_tree(system, think, max_concepts, 0);
}
