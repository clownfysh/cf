#include "inferno/ca/cell.h"
#include "h/core/standard.h"

inferno_ca_t inferno_ca_copy(inferno_ca_t cell)
{
  inferno_ca_t inferno_ca_copy;

  inferno_ca_copy.value = cell.value;
  inferno_ca_copy.rule = cell.rule;

  return inferno_ca_copy;
}

void inferno_ca_init(inferno_ca_t *cell, unsigned long value, unsigned long rule)
{
  assert(cell);

  cell->value = value;
  cell->rule = rule;
}
