#include "inferno/ca/eca.h"
#include "inferno/ca/k3.h"
#include "inferno/core/constants.h"
#include "x/core/bit.h"
#include "x/core/constants.h"
#include "x/core/tools.h"

struct k3_cell_t {
  x_core_bit_t a;
  x_core_bit_t b;
  x_core_bit_t c;
};
typedef struct k3_cell_t k3_cell_t;

struct k3_context_t {
  x_core_bit_t rule[24];
  k3_cell_t map[8];
};
typedef struct k3_context_t k3_context_t;

inferno_ca_t inferno_ca_k3_calculate_new_cell_state(inferno_ca_system_t *system,
    unsigned long cell_index)
{
  assert(system);
  unsigned long neighbor_0_index;
  unsigned long neighbor_1_index;
  unsigned long neighbor_2_index;
  inferno_ca_t *neighbor_0_cell;
  inferno_ca_t *neighbor_1_cell;
  inferno_ca_t *neighbor_2_cell;
  unsigned long new_cell_value;
  inferno_ca_t new_cell_state;
  x_core_bit_t in_a;
  x_core_bit_t in_b;
  x_core_bit_t in_c;
  x_core_bit_t link_number;
  x_core_bit_t out_a;
  x_core_bit_t out_b;
  x_core_bit_t out_c;
  k3_context_t *context;

  context = inferno_ca_system_get_context(system);

  neighbor_0_index
    = inferno_ca_eca_get_relative_cell_index(system, cell_index, 0);
  neighbor_1_index
    = inferno_ca_eca_get_relative_cell_index(system, cell_index, 1);
  neighbor_2_index
    = inferno_ca_eca_get_relative_cell_index(system, cell_index, 2);

  neighbor_0_cell = inferno_ca_system_get_current_cell(system, neighbor_0_index);
  neighbor_1_cell = inferno_ca_system_get_current_cell(system, neighbor_1_index);
  neighbor_2_cell = inferno_ca_system_get_current_cell(system, neighbor_2_index);

  in_a = neighbor_0_cell->value / 4;
  in_b = (neighbor_1_cell->value / 2) % 2;
  in_c = neighbor_2_cell->value % 2;

  link_number = (in_a) + (in_b * 2) + (in_c * 4);
  out_a = context->map[link_number].a;
  out_b = context->map[link_number].b;
  out_c = context->map[link_number].c;

  new_cell_value = (out_a) + (out_b * 2) + (out_c * 4);

  new_cell_state.value = new_cell_value;
  inferno_ca_init(&new_cell_state, new_cell_value, INFERNO_CA_NO_RULE);

  return new_cell_state;
}

void *inferno_ca_k3_create_context(void *parameter_object)
{
  k3_context_t *context;
  unsigned long rule_number;
  unsigned long value;
  unsigned long place_value;
  unsigned short eacx_bit;
  unsigned short eacx_link;
  unsigned long div;

  context = malloc(sizeof *context);
  if (context) {
    rule_number = *((unsigned long *) parameter_object);

    value = rule_number;
    place_value = 8388608;  /*  2^23  */
    for (eacx_bit = 0; eacx_bit < 24; eacx_bit++) {
      div = value / place_value;
      *(context->rule + eacx_bit) = div;
      value = value % place_value;
      place_value /= 2;
      if (0 == place_value) {
        break;
      }
    }

    for (eacx_link = 0; eacx_link < 8; eacx_link++) {
      (*(context->map + eacx_link)).a = *(context->rule + (eacx_link * 3) + 0);
      (*(context->map + eacx_link)).b = *(context->rule + (eacx_link * 3) + 1);
      (*(context->map + eacx_link)).c = *(context->rule + (eacx_link * 3) + 2);
    }
  } else {
    x_core_trace("malloc");
  }

  return context;
}

void inferno_ca_k3_destroy_context(void *context_object)
{
  free(context_object);
}

void inferno_ca_k3_get_cell_color(inferno_ca_t *cell, x_core_color_t *color)
{
  x_core_bit_t a;
  x_core_bit_t b;
  x_core_bit_t c;

  a = cell->value % 2;
  b = (cell->value / 2) % 2;
  c = cell->value / 4;

  if (0 == a) {
    color->red = X_CORE_MAX_COLOR;
  } else {
    color->red = 0;
  }
  if (0 == b) {
    color->green = X_CORE_MAX_COLOR;
  } else {
    color->green = 0;
  }
  if (0 == c) {
    color->blue = X_CORE_MAX_COLOR;
  } else {
    color->blue = 0;
  }
}

unsigned long inferno_ca_k3_get_relative_cell_index(inferno_ca_system_t *system,
    unsigned long cell_index, unsigned long relationship)
{
  return inferno_ca_eca_get_relative_cell_index(system, cell_index, relationship);
}

void inferno_ca_k3_init_systemey(inferno_ca_systemey_t *systemey, void *name_object)
{
  inferno_ca_systemey_init(systemey, name_object,
      inferno_ca_k3_calculate_new_cell_state, inferno_ca_k3_create_context,
      inferno_ca_k3_destroy_context, INFERNO_CA_NO_END_TIME_STEP_FUNCTION,
      inferno_ca_k3_get_cell_color, inferno_ca_k3_get_relative_cell_index,
      INFERNO_CA_NO_START_TIME_STEP_FUNCTION);
}
