#ifndef cf_oh_form_h
#define cf_oh_form_h

enum cf_oh_form_t {
  cf_oh_form_fluid = 0,
  cf_oh_form_description = 1,
};
typedef enum cf_oh_form_t cf_oh_form_t;

#define CF_OH_FORM_COUNT 2

const char *cf_oh_form[CF_OH_FORM_COUNT][2];

#endif
