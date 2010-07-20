#include "inferno/ca/state.h"
#include "x/core/tools.h"

struct inferno_ca_state_t {
  inferno_ca_t *cells;
  unsigned long cell_count;
};

int inferno_ca_state_compare(void *state_object_a, void *state_object_b)
{
  x_core_trace_exit("TODO: implement");
  return 0;
}

void *inferno_ca_state_copy(void *state_object)
{
  assert(state_object);
  inferno_ca_state_t *state;
  inferno_ca_state_t *state_copy;
  unsigned long cells_memory_size;

  state = state_object;
  cells_memory_size = sizeof(inferno_ca_t) * state->cell_count;

  state_copy = malloc(sizeof *state_copy);
  if (state_copy) {
    state_copy->cell_count = state->cell_count;
    state_copy->cells = malloc(cells_memory_size);
    if (state_copy->cells) {
      memcpy(state_copy->cells, state->cells, cells_memory_size);
    } else {
      x_core_trace("malloc");
    }
  } else {
    x_core_trace("malloc");
  }

  return state_copy;
}

inferno_ca_state_t *inferno_ca_state_create(unsigned long cell_count)
{
  inferno_ca_state_t *state;

  state = malloc(sizeof *state);
  if (state) {
    state->cell_count = cell_count;
    state->cells = malloc(sizeof(inferno_ca_t) * cell_count);
    if (!state->cells) {
      x_core_trace("malloc");
    }
  } else {
    x_core_trace("malloc");
  }

  return state;
}

inferno_ca_state_t *inferno_ca_state_create_from_cells(inferno_ca_t *cells,
    unsigned long cell_count)
{
  inferno_ca_state_t *state;
  unsigned long cells_memory_size;

  cells_memory_size = sizeof(inferno_ca_state_t) * cell_count;

  state = malloc(sizeof *state);
  if (state) {
    state->cell_count = cell_count;
    state->cells = malloc(cells_memory_size);
    if (state->cells) {
      memcpy(state->cells, cells, cells_memory_size);
    } else {
      x_core_trace("malloc");
    }
  } else {
    x_core_trace("malloc");
  }

  return state;
}

void inferno_ca_state_destroy(void *state_object)
{
  assert(state_object);
  inferno_ca_state_t *state;

  state = state_object;

  free(state->cells);
  free(state);
}

char *inferno_ca_state_get_as_string(void *state_object)
{
  assert(state_object);
  inferno_ca_state_t *state;
  unsigned long eacx_cell;
  char *string;
  char c;
  inferno_ca_t *cell;
  unsigned long inferno_ca_value;
  unsigned long cell_count;

  state = state_object;
  cell_count = state->cell_count;

  string = malloc(cell_count + 1);
  if (string) {
    for (eacx_cell = 0; eacx_cell < cell_count; eacx_cell++) {
      cell = &(*(state->cells + eacx_cell));
      inferno_ca_value = cell->value;
      if (inferno_ca_value < 10) {
        c = 48 + inferno_ca_value;
      } else {
        c = 120;
      }
      *(string + eacx_cell) = c;
    }
    *(string + cell_count) = '\0';
  } else {
    x_core_trace("malloc");
  }

  return string;
}

inferno_ca_t *inferno_ca_state_get_cell(inferno_ca_state_t *state,
    unsigned long cell_index)
{
  return state->cells + cell_index;
}

unsigned long inferno_ca_state_get_cell_count(inferno_ca_state_t *state)
{
  return state->cell_count;
}

unsigned long inferno_ca_state_get_cell_rule(inferno_ca_state_t *state,
    unsigned long cell_index)
{
  return (*(state->cells + cell_index)).rule;
}

unsigned long inferno_ca_state_get_cell_value(inferno_ca_state_t *state,
    unsigned long cell_index)
{
  return (*(state->cells + cell_index)).value;
}

void inferno_ca_state_print(inferno_ca_state_t *state)
{
  x_core_print(state, inferno_ca_state_get_as_string);
}

void inferno_ca_state_set_cell(inferno_ca_state_t *state, unsigned long cell_index,
    inferno_ca_t *cell)
{
  *(state->cells + cell_index) = inferno_ca_copy(*cell);
}

void inferno_ca_state_set_cell_rule(inferno_ca_state_t *state,
    unsigned long cell_index, unsigned long inferno_ca_rule)
{
  (*(state->cells + cell_index)).rule = inferno_ca_rule;
}

void inferno_ca_state_set_cell_value(inferno_ca_state_t *state,
    unsigned long cell_index, unsigned long inferno_ca_value)
{
  (*(state->cells + cell_index)).value = inferno_ca_value;
}
