#ifndef x_clink_eoc_atom_h
#define x_clink_eoc_atom_h

struct x_clink_eoc_atom_t;
typedef struct x_clink_eoc_atom_t x_clink_eoc_atom_t;

x_clink_eoc_atom_t *x_clink_eoc_atom_create(unsigned long max_concepts,
    unsigned long max_links);

void x_clink_eoc_atom_destroy(x_clink_eoc_atom_t *atom);

unsigned char x_clink_eoc_atom_get_direction(x_clink_eoc_atom_t *atom);

unsigned char x_clink_eoc_atom_get_face(x_clink_eoc_atom_t *atom);

void *x_clink_eoc_atom_get_linked_object(x_clink_eoc_atom_t *atom,
    unsigned long concept_index, unsigned long object_index);

void *x_clink_eoc_atom_get_object(x_clink_eoc_atom_t *atom,
    unsigned long concept_index);

void x_clink_eoc_atom_observe(x_clink_eoc_atom_t *atom, unsigned char c);

#endif
