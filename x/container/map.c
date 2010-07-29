#include "x/container/map.h"
#include "x/container/set.h"
#include "x/core/pair.h"
#include "x/core/tools.h"

struct x_container_map_t {
  x_container_set_t *pairs;
  x_core_objectey_t *left_objectey;
  x_core_objectey_t *right_objectey;
  x_core_bool_t destroy;
  x_core_objectey_t pairs_objectey;
};

x_core_bool_t x_container_map_add(x_container_map_t *map, void *left,
    void *right)
{
  assert(map);
  assert(left);
  assert(right);
  x_core_pair_t *pair;
  x_core_bool_t success;

  pair = x_core_pair_create(left, map->left_objectey, right,
      map->right_objectey, map->destroy);
  if (pair) {
    success = x_core_bool_true;
    x_container_set_add(map->pairs, pair);
  } else {
    success = x_core_bool_false;
    x_core_trace("x_core_pair_create");
  }

  return success;
}

int x_container_map_compare(void *map_object_a, void *map_object_b)
{
  assert(map_object_a);
  assert(map_object_b);
  x_container_map_t *map_a;
  x_container_map_t *map_b;

  map_a = map_object_a;
  map_b = map_object_b;

  return x_container_set_compare(map_a->pairs, map_b->pairs);
}

void *x_container_map_copy(void *map_object)
{
  assert(map_object);
  x_container_map_t *map;
  x_container_map_t *map_copy;

  map = map_object;

  map_copy = x_container_map_create(map->left_objectey, map->right_objectey,
      map->destroy);
  if (map_copy) {
    map_copy->pairs = x_container_set_copy(map->pairs);
    if (!map_copy->pairs) {
      x_core_trace("x_container_set_copy");
      free(map_copy);
      map_copy = NULL;
    }
  } else {
    x_core_trace("x_container_map_create");
  }

  return map_copy;
}

x_container_map_t *x_container_map_create(x_core_objectey_t *left_objectey,
    x_core_objectey_t *right_objectey, x_core_bool_t destroy)
{
  assert(left_objectey);
  assert(right_objectey);
  x_container_map_t *map;

  map = malloc(sizeof *map);
  if (map) {
    map->left_objectey = left_objectey;
    map->right_objectey = right_objectey;
    map->destroy = destroy;
    x_core_objectey_init(&map->pairs_objectey, x_core_pair_compare_left,
        x_core_pair_copy, x_core_pair_destroy, X_CORE_NO_EQUAL_FUNCTION,
        X_CORE_NO_GET_AS_STRING_FUNCTION, X_CORE_NO_MOD_FUNCTION);
    map->pairs = x_container_set_create(&map->pairs_objectey);
    if (!map->pairs) {
      x_core_trace("x_container_set_create");
      free(map);
      map = NULL;
    }
  } else {
    x_core_trace("malloc");
  }

  return map;
}

void x_container_map_destroy(void *map_object)
{
  assert(map_object);
  x_container_map_t *map;

  map = map_object;

  x_container_set_destroy(map->pairs);
  free(map);
}

void *x_container_map_find(x_container_map_t *map,
    void *left_object_decoy)
{
  assert(map);
  assert(left_object_decoy);
  x_core_pair_t *found_pair;
  void *found_object;
  x_core_pair_t *pair_decoy;

  pair_decoy = x_core_pair_create_decoy(left_object_decoy, map->left_objectey);
  if (pair_decoy) {
    found_pair = x_container_set_find(map->pairs, pair_decoy);
    x_core_pair_destroy_decoy(pair_decoy);
    if (found_pair) {
      found_object = x_core_pair_get_right(found_pair);
    } else {
      found_object = NULL;
    }
  } else {
    found_object = NULL;
    x_core_trace("pair_create_decoy");
  }

  return found_object;
}

void x_container_map_print(x_container_map_t *map)
{
  x_container_set_print(map->pairs, x_core_pair_get_as_string);
}

x_core_bool_t x_container_map_remove(x_container_map_t *map,
    void *left_object_decoy)
{
  assert(map);
  assert(left_object_decoy);
  x_core_bool_t success;
  x_core_pair_t *pair_decoy;

  pair_decoy = x_core_pair_create_decoy(left_object_decoy, map->left_objectey);
  if (pair_decoy) {
    success = x_container_set_remove(map->pairs, pair_decoy);
  } else {
    success = x_core_bool_false;
    x_core_trace("pair_create_decoy");
  }

  return success;
}
