#include "cf/inferno/ca/cell.h"
#include "cf/x/core/standard.h"

cf_inferno_ca_t cf_inferno_ca_copy(cf_inferno_ca_t cell)
{
  cf_inferno_ca_t cf_inferno_ca_copy;

  cf_inferno_ca_copy.value = cell.value;
  cf_inferno_ca_copy.rule = cell.rule;

  return cf_inferno_ca_copy;
}

void cf_inferno_ca_init(cf_inferno_ca_t *cell, unsigned long value, unsigned long rule)
{
  assert(cell);

  cell->value = value;
  cell->rule = rule;
}
