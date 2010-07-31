#ifndef cf_sage_ca_cell_h
#define cf_sage_ca_cell_h

#define CF_SAGE_CA_NO_RULE 0

struct cf_sage_ca_t {
  unsigned long value;
  unsigned long rule;
};
typedef struct cf_sage_ca_t cf_sage_ca_t;

cf_sage_ca_t cf_sage_ca_copy(cf_sage_ca_t cell);

void cf_sage_ca_init(cf_sage_ca_t *cell, unsigned long value, unsigned long rule);

#endif
