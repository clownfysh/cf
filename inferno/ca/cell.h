#ifndef inferno_ca_cell_h
#define inferno_ca_cell_h

#define INFERNO_CA_NO_RULE 0

struct inferno_ca_t {
  unsigned long value;
  unsigned long rule;
};
typedef struct inferno_ca_t inferno_ca_t;

inferno_ca_t inferno_ca_copy(inferno_ca_t cell);

void inferno_ca_init(inferno_ca_t *cell, unsigned long value, unsigned long rule);

#endif
