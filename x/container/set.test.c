#include "x/container/set.h"
#include "x/core/tools.h"
#include "x/core/unsigned_long.h"
#include "x/core/uuid.h"

#define ITERATIONS 100000
#define TEST_USING_LONGS x_core_bool_true

static void print_uuid(x_core_uuid_t *uuid, const char *name);

static void print_uuid(x_core_uuid_t *uuid, const char *name)
{
  assert(uuid);
  assert(name);
  char *uuid_string;

  uuid_string = x_core_uuid_get_string(uuid);
  printf("%s=:%s:\n", name, uuid_string);
}

int main(int argc, char *argv[])
{
  x_container_set_t *set;
  long *l;
  unsigned long n;
  unsigned long items_added;
  x_core_uuid_t *uuid;

  printf("creating set...\n");
  if (TEST_USING_LONGS) {
    set = x_container_set_create(x_core_unsigned_long_compare,
        x_core_unsigned_long_copy, x_core_unsigned_long_destroy);
  } else {
    set = x_container_set_create(x_core_uuid_compare, x_core_uuid_copy,
        x_core_uuid_destroy);
  }
  assert(set);

  printf("adding to set...\n");
  items_added = 0;
  for (n = 0; n < ITERATIONS; n++) {
    if (TEST_USING_LONGS) {
      l = malloc(sizeof *l);
      *l = random() % ITERATIONS;
      if (x_container_set_add(set, l)) {
        items_added++;
      } else {
        x_core_unsigned_long_destroy(l);
      }
    } else {
      uuid = x_core_uuid_create();
      if (x_core_bool_false) {
        print_uuid(uuid, "uuid");
      }
      if (x_container_set_add(set, uuid)) {
        items_added++;
      } else {
        x_core_uuid_destroy(uuid);
      }
    }
  }
  printf("items added: %lu\n", items_added);
  printf("set size: %lu\n", x_container_set_get_size(set));

  printf("removing from set...\n");
  for (n = 0; n < ITERATIONS; n++) {
    if (TEST_USING_LONGS) {
      l = malloc(sizeof *l);
      *l = random() % ITERATIONS;
      x_container_set_remove(set, l);
      free(l);
    }
  }
  printf("set size: %lu\n", x_container_set_get_size(set));

  if (x_core_bool_false) {
    printf("doing iterate_remove on the rest of the set\n");
    x_container_set_iterate_start(set);
    if (TEST_USING_LONGS) {
      while ((l = x_container_set_iterate_next(set))) {
        x_container_set_iterate_remove(set);
      }
    } else {
      while ((uuid = x_container_set_iterate_next(set))) {
        x_container_set_iterate_remove(set);
      }
    }
    printf("set size: %lu\n", x_container_set_get_size(set));
    assert(0 == x_container_set_get_size(set));
  }

  printf("clearing set...\n");
  x_container_set_clear(set);
  printf("set size: %lu\n", x_container_set_get_size(set));
  assert(0 == x_container_set_get_size(set));

  printf("destroying set...\n");
  x_container_set_destroy(set);

  return 0;
}
