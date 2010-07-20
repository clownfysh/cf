#include "x/core/tools.h"
#include "x/format/json.h"

struct x_format_json_t {
};

x_format_json_t *x_format_json_create()
{
  x_format_json_t *json;

  json = malloc(sizeof *json);
  if (json) {

  } else {
    x_core_trace("malloc");
  }

  return json;
}

void x_format_json_destroy(x_format_json_t *json)
{
  assert(json);
  free(json);
}
