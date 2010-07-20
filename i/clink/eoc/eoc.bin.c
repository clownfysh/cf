#include "i/clink/eoc/atom.h"
#include "i/tools.h"
#include <curses.h>
#include <time.h>
#include <unistd.h>

#define FRAME_SLEEP_MICROSECONDS 10000
#define MAX_CONCEPTS 9
#define MAX_LINKS 9
#define WORLD_WIDTH 80 /* 157 */
#define WORLD_HEIGHT 40 /* 58 */

#define SPARKS_PER_FRAME ((WORLD_WIDTH * WORLD_HEIGHT) / 2)

struct i_clink_eoc_t {
  i_clink_eoc_atom_t *world[WORLD_WIDTH][WORLD_HEIGHT];
};
typedef struct i_clink_eoc_t i_clink_eoc_t;

static i_clink_eoc_t *create();

static void destroy(i_clink_eoc_t *eoc);

static i_clink_eoc_atom_t *get_target_atom(i_clink_eoc_t *eoc,
    unsigned long atom_x, unsigned long atom_y, unsigned long *target_atom_x,
    unsigned long *target_atom_y);

static void swap(i_clink_eoc_t *eoc, unsigned long x, unsigned long y,
    unsigned long target_x, unsigned long target_y);

int main(int argc, char *argv[]);

i_clink_eoc_t *create()
{
  i_clink_eoc_t *eoc;
  unsigned long x;
  unsigned long y;

  eoc = malloc(sizeof *eoc);
  if (eoc) {
    for (x = 0; x < WORLD_WIDTH; x++) {
      for (y = 0; y < WORLD_HEIGHT; y++) {
        eoc->world[x][y] = i_clink_eoc_atom_create(MAX_CONCEPTS, MAX_LINKS);
        if (!eoc->world[x][y]) {
          i_trace("i_clink_eoc_atom_create");
          exit(1);
        }
      }
    }
  } else {
    i_trace("malloc");
  }

  return eoc;
}

void destroy(i_clink_eoc_t *eoc)
{
  unsigned long x;
  unsigned long y;

  for (x = 0; x < WORLD_WIDTH; x++) {
    for (y = 0; y < WORLD_HEIGHT; y++) {
      i_clink_eoc_atom_destroy(eoc->world[x][y]);
    }
  }
}

i_clink_eoc_atom_t *get_target_atom(i_clink_eoc_t *eoc, unsigned long atom_x,
    unsigned long atom_y, unsigned long *target_atom_x,
    unsigned long *target_atom_y)
{
  assert(eoc);
  assert(target_atom_x);
  assert(target_atom_y);
  i_clink_eoc_atom_t *observer = eoc->world[atom_x][atom_y];
  i_clink_eoc_atom_t *target;
  unsigned char direction;

  direction = i_clink_eoc_atom_get_direction(observer);
  *target_atom_x = i_wrap_index(atom_x + ((direction % 3) - 1), WORLD_WIDTH);
  *target_atom_y
    = i_wrap_index(atom_y + ((direction / 3) - 1), WORLD_HEIGHT);
  target = eoc->world[*target_atom_x][*target_atom_y];

  return target;
}

void swap(i_clink_eoc_t *eoc, unsigned long x, unsigned long y,
    unsigned long target_x, unsigned long target_y)
{
  assert(eoc);
  i_clink_eoc_atom_t *temporary;

  temporary = eoc->world[target_x][target_y];
  eoc->world[target_x][target_y] = eoc->world[x][y];
  eoc->world[x][y] = temporary;
}

int main(int argc, char *argv[])
{
  i_clink_eoc_t *eoc;
  unsigned long x;
  unsigned long y;
  unsigned long a;
  unsigned long b;
  unsigned long target_x;
  unsigned long target_y;
  i_clink_eoc_atom_t *target;
  unsigned char target_face;
  unsigned char neighbor_face;
  unsigned char face;
  unsigned long i;
  unsigned long j;

  initscr();
  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
  init_pair(4, COLOR_BLUE, COLOR_BLACK);
  init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(6, COLOR_CYAN, COLOR_BLACK);
  init_pair(7, COLOR_WHITE, COLOR_BLACK);

  srandom(time(NULL));

  eoc = create();
  if (eoc) {
    while (i_bool_true) {
      for (i = 0; i < SPARKS_PER_FRAME; i++) {
        x = random() % WORLD_WIDTH;
        y = random() % WORLD_HEIGHT;

        for (j = 0; j < 16; j++) {
          a = i_wrap_index(x + ((random() % 3) - 1), WORLD_WIDTH);
          b = i_wrap_index(y + ((random() % 3) - 1), WORLD_HEIGHT);
          neighbor_face = i_clink_eoc_atom_get_face(eoc->world[a][b]);
          i_clink_eoc_atom_observe(eoc->world[x][y], neighbor_face);
        }

        target = get_target_atom(eoc, x, y, &target_x, &target_y);
        target_face = i_clink_eoc_atom_get_face(target);
        face = i_clink_eoc_atom_get_face(eoc->world[x][y]);
        if (face > target_face) {
          swap(eoc, x, y, target_x, target_y);
        }

      }

      for (x = 0; x < WORLD_WIDTH; x++) {
        for (y = 0; y < WORLD_HEIGHT; y++) {
          face = i_clink_eoc_atom_get_face(eoc->world[x][y]);
          mvaddch(y, x, (face + 42) | COLOR_PAIR((face % 7) + 1));
        }
      }
      refresh();
      usleep(FRAME_SLEEP_MICROSECONDS);
    }
    destroy(eoc);
  } else {
    i_trace("create");
  }

  endwin();

  return 0;
}
