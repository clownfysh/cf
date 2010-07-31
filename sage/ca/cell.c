#include "cf/sage/ca/cell.h"
#include "cf/x/core/standard.h"

cf_sage_ca_t cf_sage_ca_copy(cf_sage_ca_t cell)
{
  cf_sage_ca_t cf_sage_ca_copy;

  cf_sage_ca_copy.value = cell.value;
  cf_sage_ca_copy.rule = cell.rule;

  return cf_sage_ca_copy;
}

void cf_sage_ca_init(cf_sage_ca_t *cell, unsigned long value, unsigned long rule)
{
  assert(cell);

  cell->value = value;
  cell->rule = rule;
}
