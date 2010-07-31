#include "x/case/shardset.h"
#include "x/core/string.h"
#include "x/core/tools.h"

#define SHARD_COUNT X_CASE_SHARDSET_MAX_SHARDS

int main(int argc, char *argv[])
{
  x_case_shardset_t *shardset;
  char *string;

  shardset = x_case_shardset_create(x_core_string_compare,
      x_core_string_copy, X_CORE_NO_DESTROY_FUNCTION, x_core_string_equal,
      x_core_string_hash, x_core_string_mod, SHARD_COUNT);
  if (!shardset) {
    x_core_trace_exit("x_case_shardset_create");
  }

  x_case_shardset_add(shardset, "apple");
  x_case_shardset_add(shardset, "bicycle");
  x_case_shardset_add(shardset, "color");

  x_case_shardset_iterate_start(shardset);
  while ((string = x_case_shardset_iterate_next(shardset))) {
    printf("%s\n", string);
    if (0 == strcmp("bicycle", string)) {
      x_case_shardset_iterate_remove(shardset);
    }
  }

  printf("\n");

  x_case_shardset_iterate_start(shardset);
  while ((string = x_case_shardset_iterate_next(shardset))) {
    printf("%s\n", string);
  }

  return 0;
}
