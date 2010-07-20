#ifndef x_core_days_hours_minutes_seconds_h
#define x_core_days_hours_minutes_seconds_h

struct x_core_days_hours_minutes_seconds_t {
  unsigned short days;
  unsigned short hours;
  unsigned short minutes;
  unsigned short seconds;
};
typedef struct x_core_days_hours_minutes_seconds_t
x_core_days_hours_minutes_seconds_t;

char *create_string_from_days_hours_minutes_seconds
(x_core_days_hours_minutes_seconds_t *days_hours_minutes_seconds);

#endif
