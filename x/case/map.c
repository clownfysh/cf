#include "cf/x/case/map.h"
#include "cf/x/case/set.h"
#include "cf/x/core/pair.h"
#include "cf/x/core/tools.h"

struct cf_x_case_map_t {
  cf_x_case_set_t *pairs;
  cf_x_core_objectey_t *left_objectey;
  cf_x_core_objectey_t *right_objectey;
  cf_x_core_bool_t destroy;
  cf_x_core_objectey_t pairs_objectey;
};

cf_x_core_bool_t cf_x_case_map_add(cf_x_case_map_t *map, void *left,
    void *right)
{
  assert(map);
  assert(left);
  assert(right);
  cf_x_core_pair_t *pair;
  cf_x_core_bool_t success;

  pair = cf_x_core_pair_create(left, map->left_objectey, right,
      map->right_objectey, map->destroy);
  if (pair) {
    success = cf_x_core_bool_true;
    cf_x_case_set_add(map->pairs, pair);
  } else {
    success = cf_x_core_bool_false;
    cf_x_core_trace("x_core_pair_create");
  }

  return success;
}

int cf_x_case_map_compare(void *map_object_a, void *map_object_b)
{
  assert(map_object_a);
  assert(map_object_b);
  cf_x_case_map_t *map_a;
  cf_x_case_map_t *map_b;

  map_a = map_object_a;
  map_b = map_object_b;

  return cf_x_case_set_compare(map_a->pairs, map_b->pairs);
}

void *cf_x_case_map_copy(void *map_object)
{
  assert(map_object);
  cf_x_case_map_t *map;
  cf_x_case_map_t *map_copy;

  map = map_object;

  map_copy = cf_x_case_map_create(map->left_objectey, map->right_objectey,
      map->destroy);
  if (map_copy) {
    map_copy->pairs = cf_x_case_set_copy(map->pairs);
    if (!map_copy->pairs) {
      cf_x_core_trace("x_case_set_copy");
      free(map_copy);
      map_copy = NULL;
    }
  } else {
    cf_x_core_trace("x_case_map_create");
  }

  return map_copy;
}

cf_x_case_map_t *cf_x_case_map_create(cf_x_core_objectey_t *left_objectey,
    cf_x_core_objectey_t *right_objectey, cf_x_core_bool_t destroy)
{
  assert(left_objectey);
  assert(right_objectey);
  cf_x_case_map_t *map;

  map = malloc(sizeof *map);
  if (map) {
    map->left_objectey = left_objectey;
    map->right_objectey = right_objectey;
    map->destroy = destroy;
    cf_x_core_objectey_init(&map->pairs_objectey, cf_x_core_pair_compare_left,
        cf_x_core_pair_copy, cf_x_core_pair_destroy, cf_x_core_pair_equal_left,
        CF_X_CORE_OBJECT_NO_GET_AS_STRING_F, cf_x_core_pair_mod_left);
    map->pairs = cf_x_case_set_create(&map->pairs_objectey);
    if (!map->pairs) {
      cf_x_core_trace("x_case_set_create");
      free(map);
      map = NULL;
    }
  } else {
    cf_x_core_trace("malloc");
  }

  return map;
}

void cf_x_case_map_destroy(void *map_object)
{
  assert(map_object);
  cf_x_case_map_t *map;

  map = map_object;

  cf_x_case_set_destroy(map->pairs);
  free(map);
}

void *cf_x_case_map_find(cf_x_case_map_t *map,
    void *left_object_decoy)
{
  assert(map);
  assert(left_object_decoy);
  cf_x_core_pair_t *found_pair;
  void *found_object;
  cf_x_core_pair_t *pair_decoy;

  pair_decoy = cf_x_core_pair_create_decoy(left_object_decoy, map->left_objectey);
  if (pair_decoy) {
    found_pair = cf_x_case_set_find(map->pairs, pair_decoy);
    cf_x_core_pair_destroy_decoy(pair_decoy);
    if (found_pair) {
      found_object = cf_x_core_pair_get_right(found_pair);
    } else {
      found_object = NULL;
    }
  } else {
    found_object = NULL;
    cf_x_core_trace("pair_create_decoy");
  }

  return found_object;
}

void cf_x_case_map_print(cf_x_case_map_t *map)
{
  cf_x_case_set_print(map->pairs, cf_x_core_pair_get_as_string);
}

cf_x_core_bool_t cf_x_case_map_remove(cf_x_case_map_t *map,
    void *left_object_decoy)
{
  assert(map);
  assert(left_object_decoy);
  cf_x_core_bool_t success;
  cf_x_core_pair_t *pair_decoy;

  pair_decoy = cf_x_core_pair_create_decoy(left_object_decoy, map->left_objectey);
  if (pair_decoy) {
    success = cf_x_case_set_remove(map->pairs, pair_decoy);
  } else {
    success = cf_x_core_bool_false;
    cf_x_core_trace("pair_create_decoy");
  }

  return success;
}
