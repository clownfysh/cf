#ifndef x_core_directory_h
#define x_core_directory_h

#include "x/core/standard.h"
#include "x/core/bool.h"

struct x_core_directory_t;
typedef struct x_core_directory_t x_core_directory_t;

x_core_directory_t *x_core_directory_create();

void x_core_directory_destroy(x_core_directory_t *x_core_directory);

x_core_bool_t x_core_directory_exists(char *directory_name);

x_core_bool_t x_core_directory_make(char *directory_name, mode_t mode);

#endif
