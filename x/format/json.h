#ifndef x_format_json_h
#define x_format_json_h

struct x_format_json_t;
typedef struct x_format_json_t x_format_json_t;

x_format_json_t *x_format_json_create();

void x_format_json_destroy(x_format_json_t *json);

#endif
