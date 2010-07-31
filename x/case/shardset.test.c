#include "cf/x/case/shardset.h"
#include "cf/x/core/string.h"
#include "cf/x/core/tools.h"

#define SHARD_COUNT CF_X_CASE_SHARDSET_MAX_SHARDS

int main(int argc, char *argv[])
{
  cf_x_case_shardset_t *shardset;
  char *string;

  shardset = cf_x_case_shardset_create(cf_x_core_string_compare,
      cf_x_core_string_copy, CF_X_CORE_OBJECT_NO_DESTROY_F, cf_x_core_string_equal,
      cf_x_core_string_hash, cf_x_core_string_mod, SHARD_COUNT);
  if (!shardset) {
    cf_x_core_trace_exit("x_case_shardset_create");
  }

  cf_x_case_shardset_add(shardset, "apple");
  cf_x_case_shardset_add(shardset, "bicycle");
  cf_x_case_shardset_add(shardset, "color");

  cf_x_case_shardset_iterate_start(shardset);
  while ((string = cf_x_case_shardset_iterate_next(shardset))) {
    printf("%s\n", string);
    if (0 == strcmp("bicycle", string)) {
      cf_x_case_shardset_iterate_remove(shardset);
    }
  }

  printf("\n");

  cf_x_case_shardset_iterate_start(shardset);
  while ((string = cf_x_case_shardset_iterate_next(shardset))) {
    printf("%s\n", string);
  }

  return 0;
}
