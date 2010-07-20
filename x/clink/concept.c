#include "x/clink/concept.h"
#include "x/core/tools.h"

struct x_clink_concept_t {
  void *object;
  void **objects;
  unsigned long max_links;
  x_core_compare_f compare;
  x_core_destroy_f destroy;
};

x_clink_concept_t *x_clink_concept_create(void *object,
    unsigned long max_links, x_core_compare_f compare, x_core_destroy_f destroy)
{
  assert(object);
  assert(compare);
  x_clink_concept_t *concept;

  concept = malloc(sizeof *concept);
  if (concept) {
    concept->object = object;
    concept->max_links = max_links;
    concept->compare = compare;
    concept->destroy = destroy;
    concept->objects = malloc((sizeof *concept->objects) * max_links);
    if (concept->objects) {
      memset(concept->objects, 0, (sizeof *concept->objects) * max_links);
    } else {
      x_trace("malloc");
    }
  } else {
    x_trace("malloc");
  }

  return concept;
}

void x_clink_concept_destroy(x_clink_concept_t *concept)
{
  assert(concept);
  unsigned long link;

  if (concept->destroy) {
    for (link = 0; (link < concept->max_links) && (*(concept->objects + link));
         link++) {
      concept->destroy(*(concept->objects + link));
    }
  }
  free(concept->objects);
  free(concept);
}

void *x_clink_concept_get_linked_object(x_clink_concept_t *concept,
    unsigned long object_index)
{
  assert(concept);
  assert(object_index < concept->max_links);
  return *(concept->objects + object_index);
}

void *x_clink_concept_get_object(x_clink_concept_t *concept)
{
  assert(concept);
  return concept->object;
}

void x_clink_concept_note_object(x_clink_concept_t *concept, void *object)
{
  assert(concept);
  assert(object);
  unsigned long link;
  void *temporary;
  x_core_bool_t noted = x_core_bool_false;

  for (link = 0; (link < concept->max_links) && (*(concept->objects + link));
       link++) {
    if (0 == concept->compare(*(concept->objects + link), object)) {
      if (link > 0) {
        temporary = *(concept->objects + (link - 1));
        *(concept->objects + (link - 1)) = *(concept->objects + link);
        *(concept->objects + link) = temporary;
      }
      noted = x_core_bool_true;
      break;
    }
  }

  if (!noted) {
    for (link = 0; link < concept->max_links; link++) {
      if (!*(concept->objects + link)) {
        *(concept->objects + link) = object;
        noted = x_core_bool_true;
        break;
      }
    }
  }

  if (!noted) {
    assert(*(concept->objects + (concept->max_links - 1)));
    assert(0 != concept->compare
        (object, *(concept->objects + (concept->max_links - 1))));
    if (concept->destroy) {
      concept->destroy(*(concept->objects + (concept->max_links - 1)));
    }
    *(concept->objects + (concept->max_links - 1)) = object;
  }
}

void x_clink_concept_print(x_clink_concept_t *concept,
    x_core_get_as_string_f get_as_string)
{
  assert(concept);
  assert(get_as_string);
  unsigned long i;
  char *s;

  s = get_as_string(concept->object);
  if (s) {
    printf("%s::", s);
    free(s);
  } else {
    x_trace("get_as_string");
  }
  for (i = 0; i < concept->max_links; i++) {
    if (*(concept->objects + i)) {
      s = get_as_string(*(concept->objects + i));
      if (s) {
        printf("%s", s);
        free(s);
      } else {
        x_trace("get_as_string");
      }
    } else {
      printf("-");
    }
  }
  printf("\n");
}
