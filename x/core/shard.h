#ifndef x_core_shard_h
#define x_core_shard_h

#include "x/core/object.h"

struct x_core_shard_t {
  unsigned long id;
  void *object;
};
typedef struct x_core_shard_t x_core_shard_t;

void x_core_shard_init(x_core_shard_t *shard, unsigned long id, void *object);

#endif
