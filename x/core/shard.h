#ifndef cf_x_core_shard_h
#define cf_x_core_shard_h

#include "cf/x/core/object.h"

struct cf_x_core_shard_t {
  unsigned long id;
  void *object;
};
typedef struct cf_x_core_shard_t cf_x_core_shard_t;

void cf_x_core_shard_init(cf_x_core_shard_t *shard, unsigned long id, void *object);

#endif
