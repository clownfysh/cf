#include "x/clink/eoc/atom.h"
#include "x/clink/system.h"
#include "x/core/tools.h"

struct x_clink_eoc_atom_t {
  x_clink_system_t *clink;
  unsigned char last_char;
};

static int compare(void *char_a_object, void *char_b_object);

static void destroy(void *char_object);

int compare(void *char_a_object, void *char_b_object)
{
  assert(char_a_object);
  assert(char_b_object);
  char *a = char_a_object;
  char *b = char_b_object;
  int compare_result;

  if (*a < *b) {
    compare_result = -1;
  } else if (*a > *b) {
    compare_result = 1;
  } else {
    compare_result = 0;
  }

  return compare_result;
}

void destroy(void *char_object)
{
  assert(char_object);
  char *c = char_object;
  free(c);
}

x_clink_eoc_atom_t *x_clink_eoc_atom_create(unsigned long max_concepts,
    unsigned long max_links)
{
  x_clink_eoc_atom_t *atom;

  atom = malloc(sizeof *atom);
  if (atom) {
    atom->last_char = '\0';
    atom->clink = x_clink_system_create(max_concepts, max_links, compare,
        destroy);
    if (!atom->clink) {
      x_trace("x_clink_system_create");
      free(atom);
      atom = NULL;
    }
  } else {
    x_trace("malloc");
  }

  return atom;
}

void x_clink_eoc_atom_destroy(x_clink_eoc_atom_t *atom)
{
  assert(atom);
  x_clink_system_destroy(atom->clink);
  free(atom);
}

unsigned char x_clink_eoc_atom_get_direction(x_clink_eoc_atom_t *atom)
{
  assert(atom);
  unsigned char *c;
  unsigned char direction;

  c = x_clink_system_get_linked_object(atom->clink, 0, 0);
  if (c) {
    direction = *c % 9;
  } else {
    direction = random() % 9;
  }

  return direction;
}

unsigned char x_clink_eoc_atom_get_face(x_clink_eoc_atom_t *atom)
{
  assert(atom);
  unsigned char face;
  unsigned char *primary_concept_object;

  primary_concept_object = x_clink_system_get_linked_object(atom->clink, 0, 1);
  if (primary_concept_object) {
    face = *primary_concept_object;
  } else {
    face = random() % 9;
  }

  return face;
}

void *x_clink_eoc_atom_get_linked_object(x_clink_eoc_atom_t *atom,
    unsigned long concept_index, unsigned long object_index)
{
  assert(atom);
  return x_clink_system_get_linked_object(atom->clink, concept_index,
      object_index);
}

void *x_clink_eoc_atom_get_object(x_clink_eoc_atom_t *atom,
    unsigned long concept_index)
{
  assert(atom);
  return x_clink_system_get_object(atom->clink, concept_index);
}

void x_clink_eoc_atom_observe(x_clink_eoc_atom_t *atom, unsigned char c)
{
  assert(atom);
  char *current_char;
  char *last_char;

  current_char = malloc(sizeof *current_char);
  if (current_char) {
    last_char = malloc(sizeof *last_char);
    if (last_char) {
      *current_char = c;
      *last_char = atom->last_char;
      /* x_clink_system_link(atom->clink, last_char, current_char); */
      x_clink_system_link(atom->clink, current_char, last_char);
      atom->last_char = c;
    } else {
      x_trace("malloc");
      free(current_char);
    }
  } else {
    x_trace("malloc");
  }
}
