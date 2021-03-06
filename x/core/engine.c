#include "cf/x/core/engine.h"

static char *engine_names[CF_X_NET_ENGINE_TYPE_COUNT] = {
  "void",
  "test",
  "ping",
  "shell",
  "hyper",
};

char *cf_x_net_engine_get_name(unsigned long engine_id,
    cf_x_net_engine_get_name_f get_engine_name)
{
  char *engine_name;

  if (engine_id < CF_X_NET_ENGINE_TYPE_COUNT) {
    engine_name = engine_names[engine_id];
  } else {
    if (get_engine_name) {
      engine_name = get_engine_name(engine_id);
    } else {
      engine_name = "[unknown]";
    }
  }

  return engine_name;
}
