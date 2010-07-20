#ifndef inferno_computer_system_h
#define inferno_computer_system_h

#include "inferno/computer/output.h"

struct inferno_computer_system_t;
typedef struct inferno_computer_system_t inferno_computer_system_t;

typedef void (*inferno_computer_logic_f)(unsigned long input,
    inferno_computer_output_t *output);

inferno_computer_output_t *inferno_computer_system_compute(inferno_computer_system_t *system,
    unsigned long input);

inferno_computer_system_t *inferno_computer_system_create(unsigned long order,
    inferno_computer_logic_f logic);

inferno_computer_system_t *inferno_computer_system_create_from_file(char *filename);

void inferno_computer_system_destroy(void *system_object);

char *inferno_computer_system_get_as_string(void *system_object);

void inferno_computer_system_print(inferno_computer_system_t *system);

x_core_bool_t inferno_computer_system_save_as_file(inferno_computer_system_t *system,
    char *filename);

#endif
