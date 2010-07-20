#include "i/circle/system.h"

#define PERIOD 1048576

struct i_circle_system_t {
  unsigned char *bits;
  unsigned long bits_position;
  unsigned char *chars;
  unsigned long chars_position;
  unsigned long *longs;
  unsigned long longs_position;
};

i_circle_system_t *i_circle_system_create()
{
  i_circle_system_t *system;
  unsigned long i;

  system = malloc(sizeof *system);
  if (system) {
    system->bits = malloc((sizeof *system->bits) * PERIOD);
    if (system->bits) {
      memset(system->bits, 0, (sizeof *system->bits) * PERIOD);
      system->chars = malloc((sizeof *system->chars) * PERIOD);
      if (system->chars) {
        memset(system->chars, 0, (sizeof *system->chars) * PERIOD);
        system->longs = malloc((sizeof *system->longs) * PERIOD);
        if (system->longs) {
          memset(system->longs, 0, (sizeof *system->longs) * PERIOD);
          for (i = 0; i < PERIOD; i++) {
            *(system->bits + i) = random() % 2;
            *(system->chars + i) = random() % 256;
            *(system->longs + i) = random();
          }
          system->bits_position = PERIOD;
          system->chars_position = PERIOD;
          system->longs_position = PERIOD;
        } else {
          i_trace("malloc");
          free(system->chars);
          free(system->bits);
          free(system);
          system = NULL;
        }
      } else {
        i_trace("malloc");
        free(system->bits);
        free(system);
        system = NULL;
      }
    } else {
      i_trace("malloc");
      free(system);
      system = NULL;
    }
  } else {
    i_trace("malloc");
  }

  return system;
}

void i_circle_system_destroy(i_circle_system_t *system)
{
  assert(system);
  free(system->bits);
  free(system->chars);
  free(system->longs);
  free(system);
}

i_bit_t i_circle_system_get_bit(i_circle_system_t *system)
{
  assert(system);

  if (system->bits_position > 0) {
    system->bits_position--;
  } else {
    system->bits_position = PERIOD - 1;
  }

  return *(system->bits + system->bits_position);
}

i_bool_t i_circle_system_get_bool(i_circle_system_t *system)
{
  assert(system);

  if (system->bits_position > 0) {
    system->bits_position--;
  } else {
    system->bits_position = PERIOD - 1;
  }

  return *(system->bits + system->bits_position);
}

unsigned char i_circle_system_get_char(i_circle_system_t *system)
{
  assert(system);

  if (system->chars_position > 0) {
    system->chars_position--;
  } else {
    system->chars_position = PERIOD - 1;
  }

  return *(system->chars + system->chars_position);
}

unsigned long i_circle_system_get_long(i_circle_system_t *system)
{
  assert(system);

  if (system->longs_position > 0) {
    system->longs_position--;
  } else {
    system->longs_position = PERIOD - 1;
  }

  return *(system->longs + system->longs_position);
}
