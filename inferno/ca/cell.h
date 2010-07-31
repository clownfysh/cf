#ifndef cf_inferno_ca_cell_h
#define cf_inferno_ca_cell_h

#define CF_INFERNO_CA_NO_RULE 0

struct cf_inferno_ca_t {
  unsigned long value;
  unsigned long rule;
};
typedef struct cf_inferno_ca_t cf_inferno_ca_t;

cf_inferno_ca_t cf_inferno_ca_copy(cf_inferno_ca_t cell);

void cf_inferno_ca_init(cf_inferno_ca_t *cell, unsigned long value, unsigned long rule);

#endif
