#ifndef cf_inferno_ca_tools_h
#define cf_inferno_ca_tools_h

#include "cf/x/case/array.h"
#include "cf/x/core/bitarray.h"

typedef unsigned long (*cf_inferno_ca_select_initial_rule_f)();
typedef unsigned long (*cf_inferno_ca_select_initial_value_f)();

cf_x_case_array_t *cf_inferno_ca_create_initial_state(unsigned long cell_count,
    unsigned long time_steps, cf_inferno_ca_select_initial_rule_f select_initial_rule,
    cf_inferno_ca_select_initial_value_f select_initial_value);

cf_x_case_array_t *cf_inferno_ca_create_initial_state_from_bitarray
(cf_x_core_bitarray_t *bitarray);

cf_x_case_array_t *cf_inferno_ca_create_initial_state_salt_and_pepper_binary
(unsigned long cell_count, unsigned long time_steps);

cf_x_case_array_t *cf_inferno_ca_create_initial_state_single_cell_binary
(unsigned long cell_count, unsigned long time_steps);

cf_x_case_array_t *cf_inferno_ca_create_initial_state_single_cell_k3
(unsigned long cell_count);

unsigned long cf_inferno_ca_select_rule_0();

unsigned long cf_inferno_ca_select_value_0();

unsigned long cf_inferno_ca_select_value_salt_and_pepper();

#endif
