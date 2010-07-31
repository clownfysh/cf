#include "cf/x/core/object.h"
#include "cf/x/core/pair.h"
#include "cf/x/core/tools.h"

struct cf_x_core_pair_t {
  void *left;
  cf_x_core_objectey_t *left_objectey;

  void *right;
  cf_x_core_objectey_t *right_objectey;

  cf_x_core_bool_t destroy;
};

int cf_x_core_pair_compare(void *pair_object_a, void *pair_object_b)
{
  cf_x_core_trace_exit("TODO: implement");
  return 0;
}

int cf_x_core_pair_compare_left(void *pair_object_a, void *pair_object_b)
{
  assert(pair_object_a);
  assert(pair_object_b);
  cf_x_core_pair_t *pair_a;
  cf_x_core_pair_t *pair_b;
  int compare_result;
  cf_x_core_compare_f compare;

  pair_a = pair_object_a;
  pair_b = pair_object_b;

  compare = pair_a->left_objectey->compare;
  compare_result = compare(pair_a->left, pair_b->left);

  return compare_result;
}

int cf_x_core_pair_compare_right(void *pair_object_a, void *pair_object_b)
{
  assert(pair_object_a);
  assert(pair_object_b);
  cf_x_core_pair_t *pair_a;
  cf_x_core_pair_t *pair_b;
  int compare_result;
  cf_x_core_compare_f compare;

  pair_a = pair_object_a;
  pair_b = pair_object_b;

  compare = pair_a->right_objectey->compare;
  compare_result = compare(pair_a->right, pair_b->right);

  return compare_result;
}

void *cf_x_core_pair_copy(void *pair_object)
{
  cf_x_core_trace_exit("TODO: implement");
  return NULL;
}

cf_x_core_pair_t *cf_x_core_pair_create(void *left, cf_x_core_objectey_t *left_objectey,
    void *right, cf_x_core_objectey_t *right_objectey, cf_x_core_bool_t destroy)
{
  assert(left);
  assert(left_objectey);
  assert(right);
  assert(right_objectey);
  cf_x_core_pair_t *pair;

  pair = malloc(sizeof *pair);
  if (pair) {
    pair->left = left;
    pair->left_objectey = left_objectey;
    pair->right = right;
    pair->right_objectey = right_objectey;
    pair->destroy = destroy;
  } else {
    cf_x_core_trace("malloc");
  }

  return pair;
}

cf_x_core_pair_t *cf_x_core_pair_create_decoy(void *left,
    cf_x_core_objectey_t *left_objectey)
{
  assert(left);
  assert(left_objectey);
  cf_x_core_pair_t *pair;

  pair = malloc(sizeof *pair);
  if (pair) {
    pair->left = left;
    pair->left_objectey = left_objectey;
  } else {
    cf_x_core_trace("malloc");
  }

  return pair;
}

void cf_x_core_pair_destroy(void *pair_object)
{
  assert(pair_object);
  cf_x_core_pair_t *pair;

  pair = pair_object;

  if (pair->destroy) {
    if (pair->left_objectey->destroy) {
      pair->left_objectey->destroy(pair->left);
    }
    if (pair->right_objectey->destroy) {
      pair->right_objectey->destroy(pair->right);
    }
  }

  free(pair);
}

void cf_x_core_pair_destroy_decoy(cf_x_core_pair_t *pair)
{
  free(pair);
}

cf_x_core_bool_t cf_x_core_pair_equal_left(void *pair_a_object, void *pair_b_object)
{
  return (0 == cf_x_core_pair_compare_left(pair_a_object, pair_b_object));
}

char *cf_x_core_pair_get_as_string(void *pair_object)
{
  assert(pair_object);
  cf_x_core_pair_t *pair;
  char *string;
  cf_x_core_get_as_string_f left_get_as_string;
  cf_x_core_get_as_string_f right_get_as_string;
  char *left_string;
  char *right_string;

  pair = pair_object;

  left_get_as_string = pair->left_objectey->get_as_string;
  right_get_as_string = pair->right_objectey->get_as_string;

  left_string = left_get_as_string(pair->left);
  right_string = right_get_as_string(pair->right);

  string = cf_x_core_string_append(NULL, right_string);
  string = cf_x_core_string_append(string, ":");
  string = cf_x_core_string_append(string, right_string);

  free(left_string);
  free(right_string);

  return string;
}

void *cf_x_core_pair_get_left(cf_x_core_pair_t *pair)
{
  return pair->left;
}

cf_x_core_objectey_t *cf_x_core_pair_get_left_objectey(cf_x_core_pair_t *pair)
{
  return pair->left_objectey;
}

void *cf_x_core_pair_get_right(cf_x_core_pair_t *pair)
{
  return pair->right;
}

cf_x_core_objectey_t *cf_x_core_pair_get_right_objectey(cf_x_core_pair_t *pair)
{
  return pair->right_objectey;
}

void cf_x_core_pair_init_objectey(cf_x_core_objectey_t *objectey)
{
  assert(objectey);

  objectey->compare = cf_x_core_pair_compare;
  objectey->copy = cf_x_core_pair_copy;
  objectey->destroy = cf_x_core_pair_destroy;
  objectey->get_as_string = cf_x_core_pair_get_as_string;
}

unsigned long cf_x_core_pair_mod_left(void *pair_object, unsigned long divisor)
{
  assert(pair_object);
  cf_x_core_pair_t *pair = pair_object;
  return pair->left_objectey->mod(pair->left, divisor);
}
