#include "x/core/standard.h"
#include "x/core/shard.h"
#include "x/core/types.h"

void x_core_shard_init(x_core_shard_t *shard, unsigned long id,
    void *object)
{
  assert(shard);
  shard->id = id;
  shard->object = object;
}
