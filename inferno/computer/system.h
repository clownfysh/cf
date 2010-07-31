#ifndef cf_inferno_computer_system_h
#define cf_inferno_computer_system_h

#include "cf/inferno/computer/output.h"

struct cf_inferno_computer_system_t;
typedef struct cf_inferno_computer_system_t cf_inferno_computer_system_t;

typedef void (*cf_inferno_computer_logic_f)(unsigned long input,
    cf_inferno_computer_output_t *output);

cf_inferno_computer_output_t *cf_inferno_computer_system_compute(cf_inferno_computer_system_t *system,
    unsigned long input);

cf_inferno_computer_system_t *cf_inferno_computer_system_create(unsigned long order,
    cf_inferno_computer_logic_f logic);

cf_inferno_computer_system_t *cf_inferno_computer_system_create_from_file(char *filename);

void cf_inferno_computer_system_destroy(void *system_object);

char *cf_inferno_computer_system_get_as_string(void *system_object);

void cf_inferno_computer_system_print(cf_inferno_computer_system_t *system);

cf_x_core_bool_t cf_inferno_computer_system_save_as_file(cf_inferno_computer_system_t *system,
    char *filename);

#endif
