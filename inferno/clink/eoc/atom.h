#ifndef cf_inferno_clink_eoc_atom_h
#define cf_inferno_clink_eoc_atom_h

struct cf_inferno_clink_eoc_atom_t;
typedef struct cf_inferno_clink_eoc_atom_t cf_inferno_clink_eoc_atom_t;

cf_inferno_clink_eoc_atom_t *cf_inferno_clink_eoc_atom_create(unsigned long max_concepts,
    unsigned long max_links);

void cf_inferno_clink_eoc_atom_destroy(cf_inferno_clink_eoc_atom_t *atom);

unsigned char cf_inferno_clink_eoc_atom_get_direction(cf_inferno_clink_eoc_atom_t *atom);

unsigned char cf_inferno_clink_eoc_atom_get_face(cf_inferno_clink_eoc_atom_t *atom);

void *cf_inferno_clink_eoc_atom_get_linked_object(cf_inferno_clink_eoc_atom_t *atom,
    unsigned long concept_index, unsigned long object_index);

void *cf_inferno_clink_eoc_atom_get_object(cf_inferno_clink_eoc_atom_t *atom,
    unsigned long concept_index);

void cf_inferno_clink_eoc_atom_observe(cf_inferno_clink_eoc_atom_t *atom, unsigned char c);

#endif
