#include "cf/x/core/tools.h"
#include "cf/x/format/json.h"

struct cf_x_format_json_t {
};

cf_x_format_json_t *cf_x_format_json_create()
{
  cf_x_format_json_t *json;

  json = malloc(sizeof *json);
  if (json) {

  } else {
    cf_x_core_trace("malloc");
  }

  return json;
}

void cf_x_format_json_destroy(cf_x_format_json_t *json)
{
  assert(json);
  free(json);
}
