#ifndef i_clink_eoc_atom_h
#define i_clink_eoc_atom_h

struct i_clink_eoc_atom_t;
typedef struct i_clink_eoc_atom_t i_clink_eoc_atom_t;

i_clink_eoc_atom_t *i_clink_eoc_atom_create(unsigned long max_concepts,
    unsigned long max_links);

void i_clink_eoc_atom_destroy(i_clink_eoc_atom_t *atom);

unsigned char i_clink_eoc_atom_get_direction(i_clink_eoc_atom_t *atom);

unsigned char i_clink_eoc_atom_get_face(i_clink_eoc_atom_t *atom);

void *i_clink_eoc_atom_get_linked_object(i_clink_eoc_atom_t *atom,
    unsigned long concept_index, unsigned long object_index);

void *i_clink_eoc_atom_get_object(i_clink_eoc_atom_t *atom,
    unsigned long concept_index);

void i_clink_eoc_atom_observe(i_clink_eoc_atom_t *atom, unsigned char c);

#endif
