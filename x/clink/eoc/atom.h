#ifndef cf_x_clink_eoc_atom_h
#define cf_x_clink_eoc_atom_h

struct cf_x_clink_eoc_atom_t;
typedef struct cf_x_clink_eoc_atom_t cf_x_clink_eoc_atom_t;

cf_x_clink_eoc_atom_t *cf_x_clink_eoc_atom_create(unsigned long max_concepts,
    unsigned long max_links);

void cf_x_clink_eoc_atom_destroy(cf_x_clink_eoc_atom_t *atom);

unsigned char cf_x_clink_eoc_atom_get_direction(cf_x_clink_eoc_atom_t *atom);

unsigned char cf_x_clink_eoc_atom_get_face(cf_x_clink_eoc_atom_t *atom);

void *cf_x_clink_eoc_atom_get_linked_object(cf_x_clink_eoc_atom_t *atom,
    unsigned long concept_index, unsigned long object_index);

void *cf_x_clink_eoc_atom_get_object(cf_x_clink_eoc_atom_t *atom,
    unsigned long concept_index);

void cf_x_clink_eoc_atom_observe(cf_x_clink_eoc_atom_t *atom, unsigned char c);

#endif
