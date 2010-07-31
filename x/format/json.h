#ifndef cf_x_format_json_h
#define cf_x_format_json_h

struct cf_x_format_json_t;
typedef struct cf_x_format_json_t cf_x_format_json_t;

cf_x_format_json_t *cf_x_format_json_create();

void cf_x_format_json_destroy(cf_x_format_json_t *json);

#endif
