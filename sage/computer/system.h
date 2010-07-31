#ifndef cf_sage_computer_system_h
#define cf_sage_computer_system_h

#include "cf/sage/computer/output.h"

struct cf_sage_computer_system_t;
typedef struct cf_sage_computer_system_t cf_sage_computer_system_t;

typedef void (*cf_sage_computer_logic_f)(unsigned long input,
    cf_sage_computer_output_t *output);

cf_sage_computer_output_t *cf_sage_computer_system_compute(cf_sage_computer_system_t *system,
    unsigned long input);

cf_sage_computer_system_t *cf_sage_computer_system_create(unsigned long order,
    cf_sage_computer_logic_f logic);

cf_sage_computer_system_t *cf_sage_computer_system_create_from_file(char *filename);

void cf_sage_computer_system_destroy(void *system_object);

char *cf_sage_computer_system_get_as_string(void *system_object);

void cf_sage_computer_system_print(cf_sage_computer_system_t *system);

cf_x_core_bool_t cf_sage_computer_system_save_as_file(cf_sage_computer_system_t *system,
    char *filename);

#endif
