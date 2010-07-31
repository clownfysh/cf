#include "cf/x/core/content.h"

static char *content_names[CF_X_CORE_CONTENT_TYPE_COUNT] = {
  "void",
  "image/gif",
  "text/css",
  "text/html",
  "text/javascript"
};

char *cf_x_core_content_get_name(cf_x_core_content_t content_type)
{
  char *content_name;

  if (content_type < CF_X_CORE_CONTENT_TYPE_COUNT) {
    content_name = content_names[content_type];
  } else {
    content_name = "[unknown]";
  }

  return content_name;
}
