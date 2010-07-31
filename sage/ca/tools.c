#include "cf/sage/ca/state.h"
#include "cf/sage/ca/tools.h"
#include "cf/x/core/tools.h"

cf_x_case_array_t *cf_sage_ca_create_initial_state(unsigned long cell_count,
    unsigned long time_steps, cf_sage_ca_select_initial_rule_f select_initial_rule,
    cf_sage_ca_select_initial_value_f select_initial_value)
{
  cf_x_case_array_t *initial_state;
  unsigned long each_time_step;
  unsigned long each_cell;
  cf_sage_ca_state_t *cell_state;
  cf_sage_ca_t *cells;

  initial_state = cf_x_case_array_create(time_steps, cf_sage_ca_state_compare,
      cf_sage_ca_state_copy, cf_sage_ca_state_destroy);
  if (initial_state) {
    cells = malloc(sizeof(cf_sage_ca_t) * cell_count);
    if (cells) {
      for (each_time_step = 0; each_time_step < time_steps; each_time_step++) {
        for (each_cell = 0; each_cell < cell_count; each_cell++) {
          (*(cells + each_cell)).value = select_initial_value();
          (*(cells + each_cell)).rule = select_initial_rule();
        }
        cell_state = cf_sage_ca_state_create_from_cells(cells, cell_count);
        if (cell_state) {
          cf_x_case_array_add(initial_state, each_time_step, cell_state);
        } else {
          cf_x_core_trace("sage_ca_state_create_from_cells");
        }
      }
      free(cells);
    } else {
      cf_x_core_trace("malloc");
    }
  } else {
    cf_x_core_trace("x_case_array_create");
  }

  return initial_state;
}

cf_x_case_array_t *cf_sage_ca_create_initial_state_from_bitarray
(cf_x_core_bitarray_t *bitarray)
{
  assert(bitarray);
  cf_x_case_array_t *initial_state;
  unsigned long each_cell;
  cf_sage_ca_state_t *cell_state;
  cf_sage_ca_t *cells;
  cf_x_core_bit_t bit;
  unsigned long cell_count;

  cell_count = cf_x_core_bitarray_get_size(bitarray);

  initial_state = cf_x_case_array_create(1, cf_sage_ca_state_compare,
      cf_sage_ca_state_copy, cf_sage_ca_state_destroy);
  if (initial_state) {
    cells = malloc(sizeof(cf_sage_ca_t) * cell_count);
    if (cells) {
      for (each_cell = 0; each_cell < cell_count; each_cell++) {
        bit = cf_x_core_bitarray_get_bit(bitarray, each_cell);
        (*(cells + each_cell)).value = bit;
        (*(cells + each_cell)).rule = 0;
      }
      cell_state = cf_sage_ca_state_create_from_cells(cells, cell_count);
      if (cell_state) {
        cf_x_case_array_add(initial_state, 0, cell_state);
      } else {
        cf_x_core_trace("sage_ca_state_create_from_cells");
      }
      free(cells);
    } else {
      cf_x_core_trace("malloc");
    }
  } else {
    cf_x_core_trace("x_case_array_create");
  }

  return initial_state;
}

cf_x_case_array_t *cf_sage_ca_create_initial_state_salt_and_pepper_binary
(unsigned long cell_count, unsigned long time_steps)
{
  return cf_sage_ca_create_initial_state(cell_count, time_steps,
      cf_sage_ca_select_rule_0, cf_sage_ca_select_value_salt_and_pepper);
}

cf_x_case_array_t *cf_sage_ca_create_initial_state_single_cell_binary
(unsigned long cell_count, unsigned long time_steps)
{
  cf_x_case_array_t *initial_state;
  unsigned long single_cell_index;
  cf_sage_ca_state_t *cell_state;
  unsigned long each_time_step;

  initial_state = cf_sage_ca_create_initial_state(cell_count, time_steps,
      cf_sage_ca_select_rule_0, cf_sage_ca_select_value_0);
  if (initial_state) {
    single_cell_index = cell_count / 2;
    for (each_time_step = 0; each_time_step < time_steps; each_time_step++) {
      cell_state = cf_x_case_array_find(initial_state, each_time_step);
      if (cell_state) {
        cf_sage_ca_state_set_cell_value(cell_state, single_cell_index, 1);
      } else {
        cf_x_core_trace("x_case_array_find");
      }
    }
  } else {
    cf_x_core_trace("sage_ca_create_initial_state");
  }

  return initial_state;
}

cf_x_case_array_t *cf_sage_ca_create_initial_state_single_cell_k3
(unsigned long cell_count)
{
  cf_x_case_array_t *initial_state;
  unsigned long single_cell_index;
  cf_sage_ca_state_t *cell_state;

  initial_state = cf_sage_ca_create_initial_state(cell_count, 1,
      cf_sage_ca_select_rule_0, cf_sage_ca_select_value_0);
  if (initial_state) {
    single_cell_index = cell_count / 2;
    cell_state = cf_x_case_array_find_last(initial_state);
    cf_sage_ca_state_set_cell_value(cell_state, single_cell_index, 7);
  } else {
    cf_x_core_trace("sage_ca_create_initial_state");
  }

  return initial_state;
}

unsigned long cf_sage_ca_select_rule_0()
{
  return 0;
}

unsigned long cf_sage_ca_select_value_0()
{
  return 0;
}

unsigned long cf_sage_ca_select_value_salt_and_pepper()
{
  return cf_x_core_toss_coin();
}
