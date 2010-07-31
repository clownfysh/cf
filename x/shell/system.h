#ifndef x_shell_system_h
#define x_shell_system_h

#include "x/case/list.h"

struct x_core_shell_t;
typedef struct x_core_shell_t x_core_shell_t;

void x_core_shell_append_output(x_core_shell_t *shell, char *output);

x_core_shell_t *x_core_shell_create();

void x_core_shell_destroy(x_core_shell_t *shell);

x_core_bool_t x_core_shell_start(x_core_shell_t *shell);

x_case_list_t *x_core_shell_take_input(x_core_shell_t *shell);

#endif
