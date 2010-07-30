#include "x/container/mbin.h"
#include "x/core/tools.h"
#include "x/core/unsigned_long.h"
#include "x/core/uuid.h"

#define ITERATIONS 100000
#define TEST_USING_LONGS x_core_bool_false

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
  x_container_mbin_t *mbin;
  long *l;
  unsigned long n;
  unsigned long items_added;
  x_core_uuid_t *uuid;
  x_core_objectey_t objectey;

  printf("creating mbin...\n");
  if (TEST_USING_LONGS) {
    x_core_unsigned_long_init_objectey(&objectey);
    mbin = x_container_mbin_create(&objectey, X_CONTAINER_MBIN_SET_TYPE_SET);
  } else {
    x_core_uuid_init_objectey(&objectey);
    mbin = x_container_mbin_create(&objectey, X_CONTAINER_MBIN_SET_TYPE_SET);
  }
  assert(mbin);

  printf("adding to mbin...\n");
  items_added = 0;
  for (n = 0; n < ITERATIONS; n++) {
    if (TEST_USING_LONGS) {
      l = malloc(sizeof *l);
      *l = random() % ITERATIONS;
      if (x_container_mbin_add(mbin, l)) {
        /*  printf("added %lu\n", *l);  */
        items_added++;
      } else {
        x_core_unsigned_long_destroy(l);
      }
    } else {
      uuid = x_core_uuid_create();
      if (x_core_bool_false) {
        print_uuid(uuid, "uuid");
      }
      if (x_container_mbin_add(mbin, uuid)) {
        items_added++;
      } else {
        x_core_uuid_destroy(uuid);
      }
    }
  }
  printf("items added: %lu\n", items_added);
  printf("mbin size: %lu\n", x_container_mbin_get_size(mbin));

  printf("removing from mbin...\n");
  for (n = 0; n < ITERATIONS; n++) {
    if (TEST_USING_LONGS) {
      l = malloc(sizeof *l);
      *l = random() % ITERATIONS;
      x_container_mbin_remove(mbin, l);
      /*  printf("removed %lu\n", *l);  */
      free(l);
    }
  }
  printf("mbin size: %lu\n", x_container_mbin_get_size(mbin));

  if (x_core_bool_false) {
    printf("doing iterate_remove on the rest of the mbin\n");
    x_container_mbin_iterate_start(mbin);
    if (TEST_USING_LONGS) {
      while ((l = x_container_mbin_iterate_next(mbin))) {
        x_container_mbin_iterate_remove(mbin);
      }
    } else {
      while ((uuid = x_container_mbin_iterate_next(mbin))) {
        x_container_mbin_iterate_remove(mbin);
      }
    }
    printf("mbin size: %lu\n", x_container_mbin_get_size(mbin));
    assert(0 == x_container_mbin_get_size(mbin));
  }

  printf("clearing mbin...\n");
  x_container_mbin_clear(mbin);
  printf("mbin size: %lu\n", x_container_mbin_get_size(mbin));
  assert(0 == x_container_mbin_get_size(mbin));

  printf("destroying mbin...\n");
  x_container_mbin_destroy(mbin);

  return 0;
}
