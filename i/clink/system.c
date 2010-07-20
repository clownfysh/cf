#include "i/clink/system.h"
#include "i/tools.h"

struct i_clink_system_t {
  i_clink_concept_t **concepts;
  unsigned long max_concepts;
  unsigned long max_links;
  i_compare_f compare;
  i_destroy_f destroy;
};

i_clink_system_t *i_clink_system_create(unsigned long max_concepts,
    unsigned long max_links, i_compare_f compare, i_destroy_f destroy)
{
  assert(compare);
  i_clink_system_t *system;

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
      i_trace("malloc");
      free(system);
      system = NULL;
    }
  } else {
    i_trace("malloc");
  }

  return system;
}

void i_clink_system_destroy(i_clink_system_t *system)
{
  assert(system);
  unsigned long concept;

  for (concept = 0; concept < system->max_concepts; concept++) {
    i_clink_concept_destroy(*(system->concepts + concept));
  }
  free(system);
  system = NULL;
}

i_clink_concept_t *i_clink_system_get_concept(i_clink_system_t *system,
    unsigned long concept_index)
{
  assert(system);
  assert(concept_index < system->max_concepts);
  return *(system->concepts + concept_index);
}

void *i_clink_system_get_linked_object(i_clink_system_t *system,
    unsigned long concept_index, unsigned long object_index)
{
  assert(system);
  assert(concept_index < system->max_concepts);
  assert(object_index < system->max_links);
  void *object;

  if (*(system->concepts + concept_index)) {
    object = i_clink_concept_get_linked_object
      (*(system->concepts + concept_index), object_index);
  } else {
    object = NULL;
  }

  return object;
}

void *i_clink_system_get_object(i_clink_system_t *system,
    unsigned long concept_index)
{
  assert(system);
  assert(concept_index < system->max_concepts);
  void *object;

  if (*(system->concepts + concept_index)) {
    object = i_clink_concept_get_object(*(system->concepts + concept_index));
  } else {
    object = NULL;
  }

  return object;
}

void i_clink_system_link(i_clink_system_t *system, void *object_a,
    void *object_b)
{
  assert(system);
  assert(object_a);
  assert(object_b);
  unsigned long i;
  i_clink_concept_t *temporary;
  i_bool_t noted = i_bool_false;

  for (i = 0; (i < system->max_concepts) && (*(system->concepts + i)); i++) {
    if (0 == system->compare
        (i_clink_concept_get_object(*(system->concepts + i)), object_a)) {
      i_clink_concept_note_object(*(system->concepts + i), object_b);
      if (i > 0) {
        temporary = *(system->concepts + (i - 1));
        *(system->concepts + (i - 1)) = *(system->concepts + i);
        *(system->concepts + i) = temporary;
      }
      noted = i_bool_true;
      break;
    }
  }

  if (!noted) {
    for (i = 0; i < system->max_concepts; i++) {
      if (!*(system->concepts + i)) {
        *(system->concepts + i)
          = i_clink_concept_create(object_a, system->max_links,
              system->compare, system->destroy);
        if (*(system->concepts + i)) {
          i_clink_concept_note_object(*(system->concepts + i), object_b);
        } else {
          i_trace("i_clink_concept_create");
        }
        noted = i_bool_true;
        break;
      }
    }
  }

  if (!noted) {
    assert(*(system->concepts + (system->max_concepts - 1)));
    assert(0 != system->compare(i_clink_concept_get_object
            (*(system->concepts + (system->max_concepts - 1))), object_a));
    i_clink_concept_destroy(*(system->concepts + (system->max_concepts - 1)));
    *(system->concepts + (system->max_concepts - 1))
      = i_clink_concept_create(object_a, system->max_links, system->compare,
          system->destroy);
    if (*(system->concepts + (system->max_concepts - 1))) {
      i_clink_concept_note_object
        (*(system->concepts + (system->max_concepts - 1)), object_b);
    } else {
      i_trace("i_clink_concept_create");
    }
  }
}

void i_clink_system_print(i_clink_system_t *system,
    i_get_as_string_f get_as_string)
{
  assert(system);
  assert(get_as_string);
  unsigned long i;

  printf("clink::\n");
  for (i = 0; i < system->max_concepts; i++) {
    if (*(system->concepts + i)) {
      i_clink_concept_print(*(system->concepts + i), get_as_string);
    } else {
      printf("---\n");
    }
  }
}
