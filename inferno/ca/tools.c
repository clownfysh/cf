#include "inferno/ca/state.h"
#include "inferno/ca/tools.h"
#include "x/core/tools.h"

x_container_array_t *inferno_ca_create_initial_state(unsigned long cell_count,
    unsigned long time_steps, inferno_ca_select_initial_rule_f select_initial_rule,
    inferno_ca_select_initial_value_f select_initial_value)
{
  x_container_array_t *initial_state;
  unsigned long eacx_time_step;
  unsigned long eacx_cell;
  inferno_ca_state_t *cell_state;
  inferno_ca_t *cells;

  initial_state = x_container_array_create(time_steps, inferno_ca_state_compare,
      inferno_ca_state_copy, inferno_ca_state_destroy);
  if (initial_state) {
    cells = malloc(sizeof(inferno_ca_t) * cell_count);
    if (cells) {
      for (eacx_time_step = 0; eacx_time_step < time_steps; eacx_time_step++) {
        for (eacx_cell = 0; eacx_cell < cell_count; eacx_cell++) {
          (*(cells + eacx_cell)).value = select_initial_value();
          (*(cells + eacx_cell)).rule = select_initial_rule();
        }
        cell_state = inferno_ca_state_create_from_cells(cells, cell_count);
        if (cell_state) {
          x_container_array_add(initial_state, eacx_time_step, cell_state);
        } else {
          x_core_trace("inferno_ca_state_create_from_cells");
        }
      }
      free(cells);
    } else {
      x_core_trace("malloc");
    }
  } else {
    x_core_trace("x_container_array_create");
  }

  return initial_state;
}

x_container_array_t *inferno_ca_create_initial_state_from_bitarray
(x_core_bitarray_t *bitarray)
{
  assert(bitarray);
  x_container_array_t *initial_state;
  unsigned long eacx_cell;
  inferno_ca_state_t *cell_state;
  inferno_ca_t *cells;
  x_core_bit_t bit;
  unsigned long cell_count;

  cell_count = x_core_bitarray_get_size(bitarray);

  initial_state = x_container_array_create(1, inferno_ca_state_compare,
      inferno_ca_state_copy, inferno_ca_state_destroy);
  if (initial_state) {
    cells = malloc(sizeof(inferno_ca_t) * cell_count);
    if (cells) {
      for (eacx_cell = 0; eacx_cell < cell_count; eacx_cell++) {
        bit = x_core_bitarray_get_bit(bitarray, eacx_cell);
        (*(cells + eacx_cell)).value = bit;
        (*(cells + eacx_cell)).rule = 0;
      }
      cell_state = inferno_ca_state_create_from_cells(cells, cell_count);
      if (cell_state) {
        x_container_array_add(initial_state, 0, cell_state);
      } else {
        x_core_trace("inferno_ca_state_create_from_cells");
      }
      free(cells);
    } else {
      x_core_trace("malloc");
    }
  } else {
    x_core_trace("x_container_array_create");
  }

  return initial_state;
}

x_container_array_t *inferno_ca_create_initial_state_salt_and_pepper_binary
(unsigned long cell_count, unsigned long time_steps)
{
  return inferno_ca_create_initial_state(cell_count, time_steps,
      inferno_ca_select_rule_0, inferno_ca_select_value_salt_and_pepper);
}

x_container_array_t *inferno_ca_create_initial_state_single_cell_binary
(unsigned long cell_count, unsigned long time_steps)
{
  x_container_array_t *initial_state;
  unsigned long single_cell_index;
  inferno_ca_state_t *cell_state;
  unsigned long eacx_time_step;

  initial_state = inferno_ca_create_initial_state(cell_count, time_steps,
      inferno_ca_select_rule_0, inferno_ca_select_value_0);
  if (initial_state) {
    single_cell_index = cell_count / 2;
    for (eacx_time_step = 0; eacx_time_step < time_steps; eacx_time_step++) {
      cell_state = x_container_array_find(initial_state, eacx_time_step);
      if (cell_state) {
        inferno_ca_state_set_cell_value(cell_state, single_cell_index, 1);
      } else {
        x_core_trace("x_container_array_find");
      }
    }
  } else {
    x_core_trace("inferno_ca_create_initial_state");
  }

  return initial_state;
}

x_container_array_t *inferno_ca_create_initial_state_single_cell_k3
(unsigned long cell_count)
{
  x_container_array_t *initial_state;
  unsigned long single_cell_index;
  inferno_ca_state_t *cell_state;

  initial_state = inferno_ca_create_initial_state(cell_count, 1,
      inferno_ca_select_rule_0, inferno_ca_select_value_0);
  if (initial_state) {
    single_cell_index = cell_count / 2;
    cell_state = x_container_array_find_last(initial_state);
    inferno_ca_state_set_cell_value(cell_state, single_cell_index, 7);
  } else {
    x_core_trace("inferno_ca_create_initial_state");
  }

  return initial_state;
}

unsigned long inferno_ca_select_rule_0()
{
  return 0;
}

unsigned long inferno_ca_select_value_0()
{
  return 0;
}

unsigned long inferno_ca_select_value_salt_and_pepper()
{
  return x_core_coin_toss();
}
