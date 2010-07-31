#include "cf/inferno/clink/system.h"
#include "cf/x/core/tools.h"

struct cf_inferno_clink_system_t {
  cf_inferno_clink_concept_t **concepts;
  unsigned long max_concepts;
  unsigned long max_links;
  cf_x_core_compare_f compare;
  cf_x_core_destroy_f destroy;
  void *context;
};

static cf_x_core_bool_t think_train(cf_inferno_clink_system_t *system,
    cf_inferno_clink_system_think_f think, unsigned long concept_index,
    unsigned long max_objects, unsigned long total_objects_so_far);

static cf_x_core_bool_t think_tree(cf_inferno_clink_system_t *system,
    cf_inferno_clink_system_think_f think, unsigned long concept_index,
    unsigned long *objects_remaining, unsigned long branch_density);

cf_x_core_bool_t think_train(cf_inferno_clink_system_t *system,
    cf_inferno_clink_system_think_f think, unsigned long concept_index,
    unsigned long max_objects, unsigned long total_objects_so_far)
{
  assert(system);
  assert(think);
  assert(concept_index < system->max_concepts);
  cf_x_core_bool_t success = cf_x_core_bool_true;
  cf_inferno_clink_concept_t *concept;
  void *object;
  void *linked_object;
  unsigned long next_concept_index;

  concept = *(system->concepts + concept_index);
  if (concept) {
    object = cf_inferno_clink_concept_get_object(concept);
    assert(object);
    if (think(system, object, system->context)) {
      total_objects_so_far++;
      if (total_objects_so_far < max_objects) {
        linked_object
          = cf_inferno_clink_system_get_linked_object(system, concept_index, 0);
        if (linked_object) {
          if (cf_inferno_clink_system_get_index(system, linked_object,
                  &next_concept_index)) {
            success = think_train(system, think, next_concept_index,
                max_objects, total_objects_so_far);
          }
        }
      }
    } else {
      success = cf_x_core_bool_false;
      cf_x_trace("think");
    }
  }

  return success;
}

cf_x_core_bool_t think_tree(cf_inferno_clink_system_t *system,
    cf_inferno_clink_system_think_f think, unsigned long concept_index,
    unsigned long *objects_remaining, unsigned long branch_density)
{
  assert(system);
  assert(think);
  assert(concept_index < system->max_concepts);
  assert(*objects_remaining > 0);
  assert(branch_density > 0);
  cf_x_core_bool_t success = cf_x_core_bool_true;
  void *linked_object;
  unsigned long next_concept_index;
  unsigned long link_index;
  unsigned long objects_remaining_in_branch;

  for (link_index = 0;
       (link_index < branch_density) && (*objects_remaining > 0);
       link_index++) {
    linked_object = cf_inferno_clink_system_get_linked_object
      (system, concept_index, link_index);
    if (linked_object) {
      if (think(system, linked_object, system->context)) {
        (*objects_remaining)--;
      } else {
        success = cf_x_core_bool_false;
        cf_x_trace("think");
      }
    } else {
      break;
    }
  }

  for (link_index = 0;
       (link_index < branch_density) && (*objects_remaining > 0);
       link_index++) {
    linked_object = cf_inferno_clink_system_get_linked_object
      (system, concept_index, link_index);
    if (linked_object) {
      if (cf_inferno_clink_system_get_index(system, linked_object,
              &next_concept_index)) {
        objects_remaining_in_branch
          = *objects_remaining / (branch_density - link_index);
        *objects_remaining -= objects_remaining_in_branch;
        if (objects_remaining_in_branch > 0) {
          success = think_tree(system, think, next_concept_index,
              &objects_remaining_in_branch, branch_density);
          /*  printf("%lu", objects_remaining_in_branch);  */
          *objects_remaining += objects_remaining_in_branch;
        }
      }
    } else {
      break;
    }
  }

  return success;
}

cf_inferno_clink_system_t *cf_inferno_clink_system_create(unsigned long max_concepts,
    unsigned long max_links, cf_x_core_compare_f compare,
    cf_x_core_destroy_f destroy, void *context)
{
  assert(compare);
  cf_inferno_clink_system_t *system;

  system = malloc(sizeof *system);
  if (system) {
    system->max_concepts = max_concepts;
    system->max_links = max_links;
    system->compare = compare;
    system->destroy = destroy;
    system->context = context;
    system->concepts = malloc((sizeof *system->concepts) * max_concepts);
    if (system->concepts) {
      memset(system->concepts, 0, (sizeof *system->concepts) * max_concepts);
    } else {
      cf_x_trace("malloc");
      free(system);
      system = NULL;
    }
  } else {
    cf_x_trace("malloc");
  }

  return system;
}

void cf_inferno_clink_system_destroy(cf_inferno_clink_system_t *system)
{
  assert(system);
  unsigned long concept;

  for (concept = 0; concept < system->max_concepts; concept++) {
    if (*(system->concepts + concept)) {
      cf_inferno_clink_concept_destroy(*(system->concepts + concept));
    }
  }
  free(system->concepts);
  free(system);
  system = NULL;
}

cf_inferno_clink_concept_t *cf_inferno_clink_system_get_concept(cf_inferno_clink_system_t *system,
    unsigned long concept_index)
{
  assert(system);
  assert(concept_index < system->max_concepts);
  return *(system->concepts + concept_index);
}

cf_x_core_bool_t cf_inferno_clink_system_get_index(cf_inferno_clink_system_t *system, void *object,
    unsigned long *index)
{
  assert(system);
  assert(object);
  assert(index);
  cf_x_core_bool_t found = cf_x_core_bool_false;

  *index = 0;
  while ((*index < system->max_concepts)
      && *(system->concepts + *index)
      && !found) {
    if (0 == system->compare(object,
            cf_inferno_clink_concept_get_object(*(system->concepts + *index)))) {
      found = cf_x_core_bool_true;
    } else {
      (*index)++;
    }
  }

  return found;
}

void *cf_inferno_clink_system_get_linked_object(cf_inferno_clink_system_t *system,
    unsigned long concept_index, unsigned long link_index)
{
  assert(system);
  assert(concept_index < system->max_concepts);
  assert(link_index < system->max_links);
  void *object;

  if (*(system->concepts + concept_index)) {
    object = cf_inferno_clink_concept_get_linked_object
      (*(system->concepts + concept_index), link_index);
  } else {
    object = NULL;
  }

  return object;
}

void *cf_inferno_clink_system_get_object(cf_inferno_clink_system_t *system,
    unsigned long concept_index)
{
  assert(system);
  assert(concept_index < system->max_concepts);
  void *object;

  if (*(system->concepts + concept_index)) {
    object = cf_inferno_clink_concept_get_object(*(system->concepts + concept_index));
  } else {
    object = NULL;
  }

  return object;
}

void cf_inferno_clink_system_link(cf_inferno_clink_system_t *system, void *object_a,
    void *object_b)
{
  assert(system);
  assert(object_a);
  assert(object_b);
  unsigned long i;
  cf_inferno_clink_concept_t *temporary;
  cf_x_core_bool_t noted = cf_x_core_bool_false;

  for (i = 0; (i < system->max_concepts) && (*(system->concepts + i)); i++) {
    if (0 == system->compare
        (cf_inferno_clink_concept_get_object(*(system->concepts + i)), object_a)) {
      cf_inferno_clink_concept_note_object(*(system->concepts + i), object_b);
      if (i > 0) {
        temporary = *(system->concepts + (i - 1));
        *(system->concepts + (i - 1)) = *(system->concepts + i);
        *(system->concepts + i) = temporary;
      }
      noted = cf_x_core_bool_true;
      break;
    }
  }

  if (!noted) {
    for (i = 0; i < system->max_concepts; i++) {
      if (!*(system->concepts + i)) {
        *(system->concepts + i)
          = cf_inferno_clink_concept_create(object_a, system->max_links,
              system->compare, system->destroy);
        if (*(system->concepts + i)) {
          cf_inferno_clink_concept_note_object(*(system->concepts + i), object_b);
        } else {
          cf_x_trace("x_clink_concept_create");
        }
        noted = cf_x_core_bool_true;
        break;
      }
    }
  }

  if (!noted) {
    assert(*(system->concepts + (system->max_concepts - 1)));
    assert(0 != system->compare(cf_inferno_clink_concept_get_object
            (*(system->concepts + (system->max_concepts - 1))), object_a));
    cf_inferno_clink_concept_destroy(*(system->concepts + (system->max_concepts - 1)));
    *(system->concepts + (system->max_concepts - 1))
      = cf_inferno_clink_concept_create(object_a, system->max_links, system->compare,
          system->destroy);
    if (*(system->concepts + (system->max_concepts - 1))) {
      cf_inferno_clink_concept_note_object
        (*(system->concepts + (system->max_concepts - 1)), object_b);
    } else {
      cf_x_trace("x_clink_concept_create");
    }
  }
}

void cf_inferno_clink_system_print(cf_inferno_clink_system_t *system,
    cf_x_core_get_as_string_f get_as_string)
{
  assert(system);
  assert(get_as_string);
  unsigned long i;

  printf("clink::\n");
  for (i = 0; i < system->max_concepts; i++) {
    if (*(system->concepts + i)) {
      cf_inferno_clink_concept_print(*(system->concepts + i), get_as_string);
    } else {
      printf("---\n");
    }
  }
}

cf_x_core_bool_t cf_inferno_clink_system_think_train(cf_inferno_clink_system_t *system,
    cf_inferno_clink_system_think_f think, unsigned long max_objects)
{
  assert(system);
  assert(think);
  assert(max_objects > 0);
  return think_train(system, think, 0, max_objects, 0);
}

cf_x_core_bool_t cf_inferno_clink_system_think_tree(cf_inferno_clink_system_t *system,
    cf_inferno_clink_system_think_f think, unsigned long max_objects,
    unsigned long branch_density)
{
  assert(system);
  assert(think);
  assert(max_objects > 0);
  assert(branch_density > 0);
  cf_inferno_clink_concept_t *concept;
  void *object;
  cf_x_core_bool_t success = cf_x_core_bool_true;
  unsigned long objects_remaining = max_objects;

  concept = *(system->concepts);
  if (concept) {
    object = cf_inferno_clink_concept_get_object(concept);
    assert(object);
    if (think(system, object, system->context)) {
      objects_remaining--;
      if (objects_remaining > 0) {
        success
          = think_tree(system, think, 0, &objects_remaining, branch_density);
      }
    } else {
      success = cf_x_core_bool_false;
      cf_x_trace("think");
    }
  }

  return success;
}
