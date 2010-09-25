#ifndef cf_oh_iform_h
#define cf_oh_iform_h

#include "cf/oh/form.h"

typedef cf_oh_form_t (*cf_oh_iform_get_form_f)(void *form_void);

struct cf_oh_iform_t {
  cf_oh_iform_get_form_f get_form;
};
typedef struct cf_oh_iform_t cf_oh_iform_t;

#endif
