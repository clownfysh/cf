#include "cf/x/core/shard.h"
#include "cf/x/core/standard.h"

void cf_x_core_shard_init(cf_x_core_shard_t *shard, unsigned long id,
    void *object)
{
  assert(shard);
  shard->id = id;
  shard->object = object;
}
