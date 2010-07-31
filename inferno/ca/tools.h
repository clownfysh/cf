#ifndef inferno_ca_tools_h
#define inferno_ca_tools_h

#include "x/case/array.h"
#include "x/core/bitarray.h"

typedef unsigned long (*inferno_ca_select_initial_rule_f)();
typedef unsigned long (*inferno_ca_select_initial_value_f)();

x_case_array_t *inferno_ca_create_initial_state(unsigned long cell_count,
    unsigned long time_steps, inferno_ca_select_initial_rule_f select_initial_rule,
    inferno_ca_select_initial_value_f select_initial_value);

x_case_array_t *inferno_ca_create_initial_state_from_bitarray
(x_core_bitarray_t *bitarray);

x_case_array_t *inferno_ca_create_initial_state_salt_and_pepper_binary
(unsigned long cell_count, unsigned long time_steps);

x_case_array_t *inferno_ca_create_initial_state_single_cell_binary
(unsigned long cell_count, unsigned long time_steps);

x_case_array_t *inferno_ca_create_initial_state_single_cell_k3
(unsigned long cell_count);

unsigned long inferno_ca_select_rule_0();

unsigned long inferno_ca_select_value_0();

unsigned long inferno_ca_select_value_salt_and_pepper();

#endif
