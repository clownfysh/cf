#include "cf/sage/core/constants.h"
#include "cf/sage/core/goal.h"
#include "cf/sage/eos/actor.h"
#include "cf/sage/eos/declarations.h"
#include "cf/sage/eos/system.h"
#include "cf/x/core/random.h"

#define ANARCHY_MODULUS 4
#define GENE_INDEX_DISPLAY 0
#define GENE_INDEX_MEET 1
#define GENE_INDEX_MOVE 2
#define GENE_INDEX_PERSONALITY 3
#define MUTATION 1
#define MUTATION_INCIDENCE_PER 32

struct display_gene_t {
  unsigned long red;
  unsigned long green;
  unsigned long blue;
};
typedef struct display_gene_t display_gene_t;

struct meet_gene_t {
  unsigned long address;
  unsigned long length;
};
typedef struct meet_gene_t meet_gene_t;

struct personality_gene_t {
  unsigned long extrovert;
  unsigned long racist;
};
typedef struct personality_gene_t personality_gene_t;

struct cf_sage_eos_actor_t {
  cf_x_core_bitarray_t *solution;
  cf_sage_eos_system_t *system;
  double score;
  cf_x_core_bool_t score_is_valid;
  void *box_cell;
};

static char display_color(cf_sage_eos_actor_t *actor);

static cf_x_core_bool_t fitness_is_superior(double fitness_a, double fitness_b,
    cf_sage_core_goal_t goal);

static double get_score(cf_sage_eos_actor_t *actor);

static void meet_actor(cf_sage_eos_actor_t *actor_a, cf_sage_eos_actor_t *actor_b);

static void move_actor(cf_sage_eos_actor_t *actor);

static void parse_display_gene(cf_sage_eos_actor_t *actor,
    unsigned long gene_start_address, display_gene_t *display_gene);

static void parse_meet_gene(cf_sage_eos_actor_t *actor,
    unsigned long gene_start_address, meet_gene_t *meet_gene);

static void parse_move_gene(cf_sage_eos_actor_t *actor,
    unsigned long gene_start_address, cf_sage_box_coordinate_t *move_gene);

static void parse_personality_gene(cf_sage_eos_actor_t *actor,
    unsigned long gene_start_address, personality_gene_t *personality_gene);

char display_color(cf_sage_eos_actor_t *actor)
{
  unsigned long display_gene_start_address;
  display_gene_t display_gene;
  char c;

  display_gene_start_address
    = cf_sage_core_get_gene_start_address(actor->solution, GENE_INDEX_DISPLAY);
  parse_display_gene(actor, display_gene_start_address, &display_gene);

  if ((display_gene.red > display_gene.blue)
      && (display_gene.red > display_gene.green)) {
    c = 'r';
  } else if ((display_gene.green > display_gene.red)
      && (display_gene.green > display_gene.blue)) {
    c = 'g';
  } else if ((display_gene.blue > display_gene.green)
      && (display_gene.blue > display_gene.red)) {
    c = 'b';
  } else {
    c = 'w';
  }

  return c;
}

cf_x_core_bool_t fitness_is_superior(double fitness_a, double fitness_b,
    cf_sage_core_goal_t goal)
{
  cf_x_core_bool_t superior;

  /*
    TODO: in general, what is the number optimal for society? :: is there a
    percentage, in general, of battles that should be arbitrarily decided, such
    that the optimal amount of diversion from the goal is kept in a culture?
    that diversion from the goal facilitiates diversity that aids in the goal
    :: is there some governing relationship to be found there?
  */
  if (0 == (rand() % ANARCHY_MODULUS)) {
    superior = cf_x_core_bool_true;
  } else {
    if (CF_SAGE_CORE_GOAL_MAXIMIZE_SCORE == goal) {
      if (fitness_a > fitness_b) {
        superior = cf_x_core_bool_true;
      } else {
        superior = cf_x_core_bool_false;
      }
    } else {
      if (fitness_a < fitness_b) {
        superior = cf_x_core_bool_true;
      } else {
        superior = cf_x_core_bool_false;
      }
    }
  }

  return superior;
}

double get_score(cf_sage_eos_actor_t *actor)
{
  assert(actor);
  void *context;
  cf_sage_core_score_solution_f score_solution;
  cf_x_audit_log_t *log;

  context = cf_sage_eos_system_get_context(actor->system);
  score_solution = cf_sage_eos_system_get_score_solution(actor->system);

  if (!actor->score_is_valid) {
    if (score_solution(context, actor->solution, &actor->score)) {
      actor->score_is_valid = cf_x_core_bool_true;
    } else {
      log = cf_sage_eos_system_get_log(actor->system);
      cf_x_audit_log_trace(log, "eos", "score_solution");
    }
  }

  return actor->score;
}

void cf_sage_eos_actor_act(cf_sage_eos_actor_t *actor)
{
  move_actor(actor);
}

int cf_sage_eos_actor_compare_maximize(void *actor_a_void, void *actor_b_void)
{
  assert(actor_a_void);
  assert(actor_b_void);
  cf_sage_eos_actor_t **actor_a;
  cf_sage_eos_actor_t **actor_b;
  double actor_a_score;
  double actor_b_score;
  int compare_result;

  actor_a = (cf_sage_eos_actor_t **) actor_a_void;
  actor_b = (cf_sage_eos_actor_t **) actor_b_void;

  actor_a_score = get_score((cf_sage_eos_actor_t *) *actor_a);
  actor_b_score = get_score((cf_sage_eos_actor_t *) *actor_b);
  if (actor_a_score > actor_b_score) {
    compare_result = -1;
  } else if (actor_a_score < actor_b_score) {
    compare_result = 1;
  } else {
    compare_result = 0;
  }

  return compare_result;
}

int cf_sage_eos_actor_compare_minimize(void *actor_a_void, void *actor_b_void)
{
  assert(actor_a_void);
  assert(actor_b_void);
  cf_sage_eos_actor_t **actor_a;
  cf_sage_eos_actor_t **actor_b;
  double actor_a_score;
  double actor_b_score;
  int compare_result;

  actor_a = (cf_sage_eos_actor_t **) actor_a_void;
  actor_b = (cf_sage_eos_actor_t **) actor_b_void;

  actor_a_score = get_score((cf_sage_eos_actor_t *) *actor_a);
  actor_b_score = get_score((cf_sage_eos_actor_t *) *actor_b);
  if (actor_a_score < actor_b_score) {
    compare_result = -1;
  } else if (actor_a_score > actor_b_score) {
    compare_result = 1;
  } else {
    compare_result = 0;
  }

  return compare_result;
}

void *cf_sage_eos_actor_copy(void *actor_void)
{
  cf_x_core_trace_exit("TODO: implement");
  return NULL;
}

void *cf_sage_eos_actor_create(void *system_void, cf_x_core_bitarray_t *solution)
{
  assert(system_void);
  assert(solution);
  cf_sage_eos_actor_t *actor;
  cf_sage_eos_system_t *system;
  cf_x_audit_log_t *log;

  system = system_void;

  actor = malloc(sizeof *actor);
  if (actor) {
    actor->system = system;
    actor->score_is_valid = cf_x_core_bool_false;
    actor->solution = cf_x_core_bitarray_copy(solution);
    if (!actor->solution) {
      log = cf_sage_eos_system_get_log(system);
      cf_x_audit_log_trace(log, "eos", "x_core_bitarray_copy");
      free(actor);
      actor = NULL;
    }
  } else {
    cf_x_core_trace_exit("malloc");
  }

  return actor;
}

void *cf_sage_eos_actor_create_random(void *system_void)
{
  assert(system_void);
  cf_x_core_bitarray_t *bitarray;
  cf_sage_eos_actor_t *actor;
  cf_x_audit_log_t *log;
  cf_sage_eos_system_t *system;

  system = system_void;

  bitarray = cf_x_core_bitarray_create_random(CF_SAGE_CORE_SOLUTION_BIT_COUNT);
  if (bitarray) {
    actor = cf_sage_eos_actor_create(system, bitarray);
    if (!actor) {
      log = cf_sage_eos_system_get_log(system);
      cf_x_audit_log_trace(log, "eos", "sage_eos_actor_create");
      cf_x_core_bitarray_destroy(bitarray);
    }
    cf_x_core_bitarray_destroy(bitarray);
  } else {
    actor = NULL;
    log = cf_sage_eos_system_get_log(system);
    cf_x_audit_log_trace(log, "eos", "x_core_bitarray_create_random");
  }

  return actor;
}

void cf_sage_eos_actor_destroy(void *actor_void)
{
  assert(actor_void);
  cf_sage_eos_actor_t *actor;

  actor = actor_void;

  cf_x_core_bitarray_destroy(actor->solution);
  free(actor);
}

void *cf_sage_eos_actor_get_box_cell(void *actor_void)
{
  assert(actor_void);
  cf_sage_eos_actor_t *actor;

  actor = actor_void;

  return actor->box_cell;
}

cf_x_core_bitarray_t *cf_sage_eos_actor_get_solution(void *actor_void)
{
  assert(actor_void);
  cf_sage_eos_actor_t *actor;

  actor = actor_void;

  return actor->solution;
}

void cf_sage_eos_actor_init_actorey(cf_sage_core_actorey_t *actorey)
{
  cf_sage_core_actorey_init(actorey,
      cf_sage_eos_actor_create, cf_sage_eos_actor_create_random,
      cf_sage_eos_actor_compare_minimize, cf_sage_eos_actor_copy,
      cf_sage_eos_actor_destroy, cf_sage_eos_actor_get_box_cell,
      cf_sage_eos_actor_set_box_cell);
}

void cf_sage_eos_actor_set_box_cell(void *actor_void, void *box_cell)
{
  assert(actor_void);
  cf_sage_eos_actor_t *actor;

  actor = actor_void;

  actor->box_cell = box_cell;
}

void meet_actor(cf_sage_eos_actor_t *actor_a, cf_sage_eos_actor_t *actor_b)
{
  meet_gene_t meet_gene;
  unsigned long meet_gene_start_address;
  unsigned long each_gene;
  cf_x_core_bit_t bit;
  cf_sage_box_system_t *box;

  meet_gene_start_address
    = cf_sage_core_get_gene_start_address(actor_a->solution, GENE_INDEX_MEET);
  parse_meet_gene(actor_a, meet_gene_start_address, &meet_gene);

  actor_b->score_is_valid = cf_x_core_bool_false;

  for (each_gene = meet_gene.address;
       each_gene < (meet_gene.address + meet_gene.length);
       each_gene++) {
    bit = cf_x_core_bitarray_get_bit(actor_a->solution, each_gene);
    cf_x_core_bitarray_set_bit(actor_b->solution, each_gene, bit);
    if (MUTATION) {
      if (0 == cf_x_core_random_unsigned_long(MUTATION_INCIDENCE_PER)) {
        cf_x_core_bitarray_set_bit(actor_b->solution, each_gene,
            cf_x_core_random_unsigned_long(2));
      }
    }
  }

  box = cf_sage_eos_system_get_box(actor_a->system);
  cf_sage_box_system_swap(box, actor_a, actor_b);
}

void move_actor(cf_sage_eos_actor_t *actor)
{
  cf_sage_box_coordinate_t move_gene;
  personality_gene_t personality_gene;
  unsigned long move_gene_start_address;
  unsigned long personality_gene_start_address;
  char color_of_possible_friend;
  cf_sage_eos_actor_t *target_actor;
  double actor_score;
  double target_actor_score;
  cf_sage_box_system_t *box;
  cf_sage_box_coordinate_t coordinate;
  cf_sage_core_goal_t goal;

  box = cf_sage_eos_system_get_box(actor->system);

  move_gene_start_address
    = cf_sage_core_get_gene_start_address(actor->solution, GENE_INDEX_MOVE);
  parse_move_gene(actor, move_gene_start_address, &move_gene);

  personality_gene_start_address = cf_sage_core_get_gene_start_address
    (actor->solution, GENE_INDEX_PERSONALITY);
  parse_personality_gene(actor, personality_gene_start_address,
      &personality_gene);

  target_actor = cf_sage_box_system_find_relative(box, actor, &move_gene);

  if (personality_gene.extrovert) {
    color_of_possible_friend = display_color(target_actor);
    if ((!personality_gene.racist)
        || (display_color(actor) == color_of_possible_friend)) {
      actor_score = get_score(actor);
      target_actor_score = get_score(target_actor);
      goal = cf_sage_eos_system_get_goal(actor->system);
      if (fitness_is_superior(actor_score, target_actor_score, goal)) {
        meet_actor(actor, target_actor);
      }
    }
  } else {
    cf_sage_box_coordinate_init_witx_xyz(&coordinate, 2 - move_gene.x,
        2 - move_gene.y, 2 - move_gene.z);
    target_actor = cf_sage_box_system_find_relative(box, actor, &coordinate);
    if (!target_actor) {
      cf_sage_box_system_move_relative(box, actor, &coordinate);
    }
  }
}

void parse_display_gene(cf_sage_eos_actor_t *actor,
    unsigned long gene_start_address, display_gene_t *display_gene)
{
  display_gene->red = cf_x_core_bitarray_get_unsigned_char(actor->solution,
      gene_start_address + 0);
  display_gene->green = cf_x_core_bitarray_get_unsigned_char(actor->solution,
      gene_start_address + 8);
  display_gene->blue = cf_x_core_bitarray_get_unsigned_char(actor->solution,
      gene_start_address + 16);
}

void parse_meet_gene(cf_sage_eos_actor_t *actor, unsigned long gene_start_address,
    meet_gene_t *meet_gene)
{
  meet_gene->address = cf_x_core_bitarray_get_unsigned_char(actor->solution,
      gene_start_address + 0);
  meet_gene->length = cf_x_core_bitarray_get_unsigned_char(actor->solution,
      gene_start_address + 8);
}

void parse_move_gene(cf_sage_eos_actor_t *actor, unsigned long gene_start_address,
    cf_sage_box_coordinate_t *move_gene)
{
  assert(actor);
  assert(move_gene);

  move_gene->x = cf_x_core_bitarray_get_unsigned_char
    (actor->solution, gene_start_address + 0) % 3;
  move_gene->y = cf_x_core_bitarray_get_unsigned_char
    (actor->solution, gene_start_address + 8) % 3;
  move_gene->z = 1;

}

void parse_personality_gene(cf_sage_eos_actor_t *actor,
    unsigned long gene_start_address, personality_gene_t *personality_gene)
{
  personality_gene->extrovert = cf_x_core_bitarray_get_bit
    (actor->solution, gene_start_address + 0);
  personality_gene->racist = cf_x_core_bitarray_get_bit
    (actor->solution, gene_start_address + 1);
}
