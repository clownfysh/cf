#include "cf/x/case/set.h"
#include "cf/x/core/tools.h"
#include "cf/x/core/unsigned_long.h"
#include "cf/x/core/uuid.h"

#define ITERATIONS 1000000
#define TEST_USING_LONGS cf_x_core_bool_true

static void print_uuid(cf_x_core_uuid_t *uuid, const char *name);

static void print_uuid(cf_x_core_uuid_t *uuid, const char *name)
{
  assert(uuid);
  assert(name);
  char *uuid_string;

  uuid_string = cf_x_core_uuid_get_string(uuid);
  printf("%s=:%s:\n", name, uuid_string);
}

int main(int argc, char *argv[])
{
  cf_x_case_set_t *set;
  long *l;
  unsigned long n;
  unsigned long items_added;
  cf_x_core_uuid_t *uuid;
  cf_x_core_objectey_t unsigned_long_objectey;
  cf_x_core_objectey_t uuid_objectey;

  printf("creating set...\n");
  if (TEST_USING_LONGS) {
    cf_x_core_basic_unsigned_long_init_objectey(&unsigned_long_objectey);
    set = cf_x_case_set_create(&unsigned_long_objectey);
  } else {
    cf_x_core_basic_unsigned_long_init_objectey(&uuid_objectey);
    set = cf_x_case_set_create(&uuid_objectey);
  }
  assert(set);

  printf("adding to set...\n");
  items_added = 0;
  for (n = 0; n < ITERATIONS; n++) {
    if (TEST_USING_LONGS) {
      l = malloc(sizeof *l);
      *l = random() % ITERATIONS;
      if (cf_x_case_set_add(set, l)) {
        items_added++;
      } else {
        cf_x_core_basic_unsigned_long_destroy(l);
      }
    } else {
      uuid = cf_x_core_uuid_create();
      if (cf_x_core_bool_false) {
        print_uuid(uuid, "uuid");
      }
      if (cf_x_case_set_add(set, uuid)) {
        items_added++;
      } else {
        cf_x_core_uuid_destroy(uuid);
      }
    }
  }
  printf("items added: %lu\n", items_added);
  printf("set size: %lu\n", cf_x_case_set_get_size(set));

  printf("removing from set...\n");
  for (n = 0; n < ITERATIONS; n++) {
    if (TEST_USING_LONGS) {
      l = malloc(sizeof *l);
      *l = random() % ITERATIONS;
      cf_x_case_set_remove(set, l);
      free(l);
    }
  }
  printf("set size: %lu\n", cf_x_case_set_get_size(set));

  if (cf_x_core_bool_false) {
    printf("doing iterate_remove on the rest of the set\n");
    cf_x_case_set_iterate_start(set);
    if (TEST_USING_LONGS) {
      while ((l = cf_x_case_set_iterate_next(set))) {
        cf_x_case_set_iterate_remove(set);
      }
    } else {
      while ((uuid = cf_x_case_set_iterate_next(set))) {
        cf_x_case_set_iterate_remove(set);
      }
    }
    printf("set size: %lu\n", cf_x_case_set_get_size(set));
    assert(0 == cf_x_case_set_get_size(set));
  }

  printf("clearing set...\n");
  cf_x_case_set_clear(set);
  printf("set size: %lu\n", cf_x_case_set_get_size(set));
  assert(0 == cf_x_case_set_get_size(set));

  printf("destroying set...\n");
  cf_x_case_set_destroy(set);

  return 0;
}
