#ifndef cf_x_core_days_hours_minutes_seconds_h
#define cf_x_core_days_hours_minutes_seconds_h

struct cf_x_core_days_hours_minutes_seconds_t {
  unsigned short days;
  unsigned short hours;
  unsigned short minutes;
  unsigned short seconds;
};
typedef struct cf_x_core_days_hours_minutes_seconds_t
cf_x_core_days_hours_minutes_seconds_t;

char *create_string_from_days_hours_minutes_seconds
(cf_x_core_days_hours_minutes_seconds_t *days_hours_minutes_seconds);

#endif
