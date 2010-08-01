#ifndef cf_x_core_dayhour_h
#define cf_x_core_dayhour_h

struct cf_x_core_dayhour_t {
  unsigned short days;
  unsigned short hours;
  unsigned short minutes;
  unsigned short seconds;
};
typedef struct cf_x_core_dayhour_t cf_x_core_dayhour_t;

char *create_string_from_dayhour(cf_x_core_dayhour_t *dayhour);

#endif
