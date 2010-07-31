#ifndef cf_x_shell_system_h
#define cf_x_shell_system_h

#include "cf/x/case/list.h"

struct cf_x_core_shell_t;
typedef struct cf_x_core_shell_t cf_x_core_shell_t;

void cf_x_core_shell_append_output(cf_x_core_shell_t *shell, char *output);

cf_x_core_shell_t *cf_x_core_shell_create();

void cf_x_core_shell_destroy(cf_x_core_shell_t *shell);

cf_x_core_bool_t cf_x_core_shell_start(cf_x_core_shell_t *shell);

cf_x_case_list_t *cf_x_core_shell_take_input(cf_x_core_shell_t *shell);

#endif
