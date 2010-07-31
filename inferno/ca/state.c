#include "cf/inferno/ca/state.h"
#include "cf/x/core/tools.h"

struct cf_inferno_ca_state_t {
  cf_inferno_ca_t *cells;
  unsigned long cell_count;
};

int cf_inferno_ca_state_compare(void *state_object_a, void *state_object_b)
{
  cf_x_core_trace_exit("TODO: implement");
  return 0;
}

void *cf_inferno_ca_state_copy(void *state_object)
{
  assert(state_object);
  cf_inferno_ca_state_t *state;
  cf_inferno_ca_state_t *state_copy;
  unsigned long cells_memory_size;

  state = state_object;
  cells_memory_size = sizeof(cf_inferno_ca_t) * state->cell_count;

  state_copy = malloc(sizeof *state_copy);
  if (state_copy) {
    state_copy->cell_count = state->cell_count;
    state_copy->cells = malloc(cells_memory_size);
    if (state_copy->cells) {
      memcpy(state_copy->cells, state->cells, cells_memory_size);
    } else {
      cf_x_core_trace("malloc");
    }
  } else {
    cf_x_core_trace("malloc");
  }

  return state_copy;
}

cf_inferno_ca_state_t *cf_inferno_ca_state_create(unsigned long cell_count)
{
  cf_inferno_ca_state_t *state;

  state = malloc(sizeof *state);
  if (state) {
    state->cell_count = cell_count;
    state->cells = malloc(sizeof(cf_inferno_ca_t) * cell_count);
    if (!state->cells) {
      cf_x_core_trace("malloc");
    }
  } else {
    cf_x_core_trace("malloc");
  }

  return state;
}

cf_inferno_ca_state_t *cf_inferno_ca_state_create_from_cells(cf_inferno_ca_t *cells,
    unsigned long cell_count)
{
  cf_inferno_ca_state_t *state;
  unsigned long cells_memory_size;

  cells_memory_size = sizeof(cf_inferno_ca_state_t) * cell_count;

  state = malloc(sizeof *state);
  if (state) {
    state->cell_count = cell_count;
    state->cells = malloc(cells_memory_size);
    if (state->cells) {
      memcpy(state->cells, cells, cells_memory_size);
    } else {
      cf_x_core_trace("malloc");
    }
  } else {
    cf_x_core_trace("malloc");
  }

  return state;
}

void cf_inferno_ca_state_destroy(void *state_object)
{
  assert(state_object);
  cf_inferno_ca_state_t *state;

  state = state_object;

  free(state->cells);
  free(state);
}

char *cf_inferno_ca_state_get_as_string(void *state_object)
{
  assert(state_object);
  cf_inferno_ca_state_t *state;
  unsigned long each_cell;
  char *string;
  char c;
  cf_inferno_ca_t *cell;
  unsigned long cf_inferno_ca_value;
  unsigned long cell_count;

  state = state_object;
  cell_count = state->cell_count;

  string = malloc(cell_count + 1);
  if (string) {
    for (each_cell = 0; each_cell < cell_count; each_cell++) {
      cell = &(*(state->cells + each_cell));
      cf_inferno_ca_value = cell->value;
      if (cf_inferno_ca_value < 10) {
        c = 48 + cf_inferno_ca_value;
      } else {
        c = 120;
      }
      *(string + each_cell) = c;
    }
    *(string + cell_count) = '\0';
  } else {
    cf_x_core_trace("malloc");
  }

  return string;
}

cf_inferno_ca_t *cf_inferno_ca_state_get_cell(cf_inferno_ca_state_t *state,
    unsigned long cell_index)
{
  return state->cells + cell_index;
}

unsigned long cf_inferno_ca_state_get_cell_count(cf_inferno_ca_state_t *state)
{
  return state->cell_count;
}

unsigned long cf_inferno_ca_state_get_cell_rule(cf_inferno_ca_state_t *state,
    unsigned long cell_index)
{
  return (*(state->cells + cell_index)).rule;
}

unsigned long cf_inferno_ca_state_get_cell_value(cf_inferno_ca_state_t *state,
    unsigned long cell_index)
{
  return (*(state->cells + cell_index)).value;
}

void cf_inferno_ca_state_print(cf_inferno_ca_state_t *state)
{
  cf_x_core_print(state, cf_inferno_ca_state_get_as_string);
}

void cf_inferno_ca_state_set_cell(cf_inferno_ca_state_t *state, unsigned long cell_index,
    cf_inferno_ca_t *cell)
{
  *(state->cells + cell_index) = cf_inferno_ca_copy(*cell);
}

void cf_inferno_ca_state_set_cell_rule(cf_inferno_ca_state_t *state,
    unsigned long cell_index, unsigned long cf_inferno_ca_rule)
{
  (*(state->cells + cell_index)).rule = cf_inferno_ca_rule;
}

void cf_inferno_ca_state_set_cell_value(cf_inferno_ca_state_t *state,
    unsigned long cell_index, unsigned long cf_inferno_ca_value)
{
  (*(state->cells + cell_index)).value = cf_inferno_ca_value;
}
