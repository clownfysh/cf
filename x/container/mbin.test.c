#include "x/container/mbin.h"
#include "x/core/long.h"
#include "x/core/tools.h"

#define ITERATIONS 10000

int main(int argc, char *argv[])
{
  x_container_mbin_t *mbin;
  long *l;
  unsigned long n;
  unsigned long items_added;

  printf("creating mbin...\n");
  mbin = x_container_mbin_create(x_core_long_mod, x_core_long_equal,
      x_core_long_destroy);
  assert(mbin);

  printf("adding to mbin...\n");
  items_added = 0;
  for (n = 0; n < ITERATIONS; n++) {
    l = malloc(sizeof *l);
    *l = random() % ITERATIONS;
    if (x_container_mbin_add(mbin, l)) {
      items_added++;
    } else {
      x_core_long_destroy(l);
    }
  }
  printf("items added: %lu\n", items_added);
  printf("mbin size: %lu\n", x_container_mbin_get_size(mbin));

  printf("removing from mbin...\n");
  for (n = 0; n < ITERATIONS; n++) {
    l = malloc(sizeof *l);
    *l = random() % ITERATIONS;
    x_container_mbin_remove(mbin, l);
    free(l);
  }
  printf("mbin size: %lu\n", x_container_mbin_get_size(mbin));

  if (x_core_bool_true) {
    printf("doing iterate_remove on the rest of the mbin\n");
    x_container_mbin_iterate_start(mbin);
      while ((l = x_container_mbin_iterate_next(mbin))) {
        x_container_mbin_iterate_remove(mbin);
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
