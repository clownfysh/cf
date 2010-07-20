#include "inferno/bios/actor.h"
#include "inferno/bios/declarations.h"
#include "inferno/bios/system.h"
#include "inferno/core/constants.h"
#include "inferno/core/goal.h"
#include "x/core/random.h"
#include "x/core/tools.h"

#define ANARCHY_MODULUS 4
#define DO_MOVE 1
#define DO_MEET 1
#define EFE_LENGTH 32  /*  2, 5, 32, ...  */

/*  1 2 1 1 1 1 3 4  */
#define GENE_INDEX_DISPLAY 1
#define GENE_INDEX_MOVE 2
#define GENE_INDEX_MEET 1
#define GENE_INDEX_MEET_WHO 1
#define GENE_INDEX_MEET_START 1
#define GENE_INDEX_MEET_LENGTH 1
#define GENE_INDEX_FSCORE 3
#define GENE_INDEX_HISTORY_BIT 4

#define MUTATION 1
#define MUTATION_INCIDENCE_PER 32
#define POSTER_WIDTH 4
#define POSTER_HEIGHT 4
#define USE_FSCORE 1

/*  I've got some range-oriented error with at least one of these right now...
    setting them higher than 1 tickles the error.  */
#define CA_OUT_ADDRESS_SPREAD_FACTOR 1
#define OBSERVE_LOCATION_SPREAD_DISTANCE 1

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

struct inferno_bios_actor_t {
  x_core_bitarray_t *solution;
  inferno_bios_system_t *system;
  double score;
  x_core_bool_t score_is_valid;
  void *box_cell;
};

char display_color(inferno_bios_actor_t *actor);

static unsigned long eight_from_eight(inferno_bios_actor_t *actor,
    unsigned long ulong_0, unsigned long ulong_1, unsigned long ulong_2,
    unsigned long ulong_3, unsigned long ulong_4, unsigned long ulong_5,
    unsigned long ulong_6, unsigned long ulong_7);

static unsigned long eight_from_sixteen(inferno_bios_actor_t *actor,
    unsigned long ulong_0, unsigned long ulong_1, unsigned long ulong_2,
    unsigned long ulong_3, unsigned long ulong_4, unsigned long ulong_5,
    unsigned long ulong_6, unsigned long ulong_7, unsigned long ulong_8,
    unsigned long ulong_9, unsigned long ulong_10, unsigned long ulong_11,
    unsigned long ulong_12, unsigned long ulong_13, unsigned long ulong_14,
    unsigned long ulong_15);

static unsigned long eight_from_twenty_four(inferno_bios_actor_t *actor,
    unsigned long ulong_0, unsigned long ulong_1, unsigned long ulong_2,
    unsigned long ulong_3, unsigned long ulong_4, unsigned long ulong_5,
    unsigned long ulong_6, unsigned long ulong_7, unsigned long ulong_8,
    unsigned long ulong_9, unsigned long ulong_10, unsigned long ulong_11,
    unsigned long ulong_12, unsigned long ulong_13, unsigned long ulong_14,
    unsigned long ulong_15, unsigned long ulong_16, unsigned long ulong_17,
    unsigned long ulong_18, unsigned long ulong_19, unsigned long ulong_20,
    unsigned long ulong_21, unsigned long ulong_22, unsigned long ulong_23);

static x_core_bool_t fitness_is_superior(double fitness_a, double fitness_b,
    inferno_core_goal_t goal);

static unsigned long get_position_index_for_observe_location
(inferno_bios_actor_t *actor, unsigned long observe_location);

static inferno_box_coordinate_t get_relative_position_from_index
(unsigned long index);

static double get_score(inferno_bios_actor_t *actor);

static void meet_actor(inferno_bios_actor_t *actor);

static void meet_actor_details(inferno_bios_actor_t *actor_a,
    inferno_bios_actor_t *actor_b);

static void move_actor(inferno_bios_actor_t *actor);

static void parse_display_gene(inferno_bios_actor_t *actor,
    unsigned long gene_start_address, display_gene_t *display_gene);

static void parse_meet_gene(inferno_bios_actor_t *actor,
    unsigned long gene_start_address, meet_gene_t *meet_gene);

char display_color(inferno_bios_actor_t *actor)
{
  unsigned long display_gene_start_address;
  display_gene_t display_gene;
  char c;

  display_gene_start_address
    = inferno_core_get_gene_start_address(actor->solution, GENE_INDEX_DISPLAY);
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

unsigned long eight_from_eight(inferno_bios_actor_t *actor, unsigned long ulong_0,
    unsigned long ulong_1, unsigned long ulong_2, unsigned long ulong_3,
    unsigned long ulong_4, unsigned long ulong_5, unsigned long ulong_6,
    unsigned long ulong_7)
{
  unsigned long cells[8][EFE_LENGTH];
  unsigned long eacx_x;
  unsigned long eacx_y;
  unsigned long ca_in_0;
  unsigned long ca_in_1;
  unsigned long ca_in_2;
  unsigned long ca_index_0;
  unsigned long ca_index_1;
  unsigned long ca_index_2;
  unsigned long ca_out_address;
  unsigned long ca_out;
  unsigned long result;
  unsigned long result_bit_0;
  unsigned long result_bit_1;
  unsigned long result_bit_2;

  cells[0][0] = ulong_0;
  cells[1][0] = ulong_1;
  cells[2][0] = ulong_2;
  cells[3][0] = ulong_3;
  cells[4][0] = ulong_4;
  cells[5][0] = ulong_5;
  cells[6][0] = ulong_6;
  cells[7][0] = ulong_7;

  for (eacx_y = 1; eacx_y < EFE_LENGTH; eacx_y++) {
    for (eacx_x = 0; eacx_x < 8; eacx_x++) {

      /*  it might make a difference if these indexes, before being
          wrapped, had a constant added to them so that they weren't at
          the beginning of the genome (like how with gene indexes I've
          been starting them at 2 so they're offset from the start of
          the gene)...the thinking behind this was something like: for
          some reason I thought maybe the code had, as a byproduct of
          some algorithm, given a preference for gene cutting/selection/
          copying during the meet...that I had given some preference
          that would make cutting/selecting/copying less likely at the
          beginning of the gene, so having these things farther longo the
          gene would make them more likely to be changeable.  but I'm
          not sure there even is such a preference.  */
      ca_index_0 = x_core_wrap_index(eacx_x - 1, 8);
      ca_index_1 = eacx_x;
      ca_index_2 = x_core_wrap_index(eacx_x + 1, 8);

      ca_in_0 = cells[ca_index_0][eacx_y - 1];
      ca_in_1 = cells[ca_index_1][eacx_y - 1];
      ca_in_2 = cells[ca_index_2][eacx_y - 1];
      ca_out_address = CA_OUT_ADDRESS_SPREAD_FACTOR * (
        (4 * ca_in_0) + (2 * ca_in_1) + (1 * ca_in_2)
        );
      ca_out = x_core_bitarray_get_bit(actor->solution, ca_out_address);
      cells[eacx_x][eacx_y] = ca_out;
    }
  }

  result_bit_0 = cells[0][EFE_LENGTH - 1];
  result_bit_1 = cells[1][EFE_LENGTH - 1];
  result_bit_2 = cells[2][EFE_LENGTH - 1];
  result = (4 * result_bit_0) + (2 * result_bit_1) + result_bit_2;

  return result;
}

unsigned long eight_from_sixteen(inferno_bios_actor_t *actor, unsigned long ulong_0,
    unsigned long ulong_1, unsigned long ulong_2, unsigned long ulong_3,
    unsigned long ulong_4, unsigned long ulong_5, unsigned long ulong_6,
    unsigned long ulong_7, unsigned long ulong_8, unsigned long ulong_9,
    unsigned long ulong_10, unsigned long ulong_11, unsigned long ulong_12,
    unsigned long ulong_13, unsigned long ulong_14, unsigned long ulong_15)
{
  unsigned long cells[8][EFE_LENGTH];
  unsigned long eacx_x;
  unsigned long eacx_y;
  unsigned long ca_in_0;
  unsigned long ca_in_1;
  unsigned long ca_in_2;
  unsigned long ca_in_3;
  unsigned long ca_in_4;
  unsigned long ca_in_5;
  unsigned long ca_index_0;
  unsigned long ca_index_1;
  unsigned long ca_index_2;
  unsigned long ca_out_address;
  unsigned long ca_out;
  unsigned long result;
  unsigned long result_bit_0;
  unsigned long result_bit_1;
  unsigned long result_bit_2;

  cells[0][0] = ulong_0;
  cells[1][0] = ulong_1;
  cells[2][0] = ulong_2;
  cells[3][0] = ulong_3;
  cells[4][0] = ulong_4;
  cells[5][0] = ulong_5;
  cells[6][0] = ulong_6;
  cells[7][0] = ulong_7;

  cells[0][1] = ulong_8;
  cells[1][1] = ulong_9;
  cells[2][1] = ulong_10;
  cells[3][1] = ulong_11;
  cells[4][1] = ulong_12;
  cells[5][1] = ulong_13;
  cells[6][1] = ulong_14;
  cells[7][1] = ulong_15;

  for (eacx_y = 2; eacx_y < EFE_LENGTH; eacx_y++) {
    for (eacx_x = 0; eacx_x < 8; eacx_x++) {
      ca_index_0 = x_core_wrap_index(eacx_x - 1, 8);
      ca_index_1 = eacx_x;
      ca_index_2 = x_core_wrap_index(eacx_x + 1, 8);

      ca_in_0 = cells[ca_index_0][eacx_y - 1];
      ca_in_1 = cells[ca_index_1][eacx_y - 1];
      ca_in_2 = cells[ca_index_2][eacx_y - 1];

      ca_in_3 = cells[ca_index_0][eacx_y - 2];
      ca_in_4 = cells[ca_index_1][eacx_y - 2];
      ca_in_5 = cells[ca_index_2][eacx_y - 2];

      ca_out_address = CA_OUT_ADDRESS_SPREAD_FACTOR * (
        (32 * ca_in_5) + (16 * ca_in_4) + (8 * ca_in_3)
        + (4 * ca_in_2) + (2 * ca_in_1) + (1 * ca_in_0)
        );

      ca_out = x_core_bitarray_get_bit(actor->solution, ca_out_address);
      cells[eacx_x][eacx_y] = ca_out;
    }
  }

  result_bit_0 = cells[0][EFE_LENGTH - 1];
  result_bit_1 = cells[1][EFE_LENGTH - 1];
  result_bit_2 = cells[2][EFE_LENGTH - 1];
  result = (4 * result_bit_0) + (2 * result_bit_1) + result_bit_2;

  return result;
}

unsigned long eight_from_twenty_four(inferno_bios_actor_t *actor,
    unsigned long ulong_0, unsigned long ulong_1, unsigned long ulong_2,
    unsigned long ulong_3, unsigned long ulong_4, unsigned long ulong_5,
    unsigned long ulong_6, unsigned long ulong_7, unsigned long ulong_8,
    unsigned long ulong_9, unsigned long ulong_10, unsigned long ulong_11,
    unsigned long ulong_12, unsigned long ulong_13, unsigned long ulong_14,
    unsigned long ulong_15, unsigned long ulong_16, unsigned long ulong_17,
    unsigned long ulong_18, unsigned long ulong_19, unsigned long ulong_20,
    unsigned long ulong_21, unsigned long ulong_22, unsigned long ulong_23)
{
  unsigned long cells[8][EFE_LENGTH];
  unsigned long eacx_x;
  unsigned long eacx_y;
  unsigned long ca_in_0;
  unsigned long ca_in_1;
  unsigned long ca_in_2;
  unsigned long ca_in_3;
  unsigned long ca_in_4;
  unsigned long ca_in_5;
  unsigned long ca_in_6;
  unsigned long ca_in_7;
  unsigned long ca_in_8;
  unsigned long ca_index_0;
  unsigned long ca_index_1;
  unsigned long ca_index_2;
  unsigned long ca_out_address;
  unsigned long ca_out;
  unsigned long result;
  unsigned long result_bit_0;
  unsigned long result_bit_1;
  unsigned long result_bit_2;

  cells[0][0] = ulong_0;
  cells[1][0] = ulong_1;
  cells[2][0] = ulong_2;
  cells[3][0] = ulong_3;
  cells[4][0] = ulong_4;
  cells[5][0] = ulong_5;
  cells[6][0] = ulong_6;
  cells[7][0] = ulong_7;

  cells[0][1] = ulong_8;
  cells[1][1] = ulong_9;
  cells[2][1] = ulong_10;
  cells[3][1] = ulong_11;
  cells[4][1] = ulong_12;
  cells[5][1] = ulong_13;
  cells[6][1] = ulong_14;
  cells[7][1] = ulong_15;

  cells[0][2] = ulong_16;
  cells[1][2] = ulong_17;
  cells[2][2] = ulong_18;
  cells[3][2] = ulong_19;
  cells[4][2] = ulong_20;
  cells[5][2] = ulong_21;
  cells[6][2] = ulong_22;
  cells[7][2] = ulong_23;

  for (eacx_y = 3; eacx_y < EFE_LENGTH; eacx_y++) {
    for (eacx_x = 0; eacx_x < 8; eacx_x++) {
      ca_index_0 = x_core_wrap_index(eacx_x - 1, 8);
      ca_index_1 = eacx_x;
      ca_index_2 = x_core_wrap_index(eacx_x + 1, 8);

      ca_in_0 = cells[ca_index_0][eacx_y - 1];
      ca_in_1 = cells[ca_index_1][eacx_y - 1];
      ca_in_2 = cells[ca_index_2][eacx_y - 1];

      ca_in_3 = cells[ca_index_0][eacx_y - 2];
      ca_in_4 = cells[ca_index_1][eacx_y - 2];
      ca_in_5 = cells[ca_index_2][eacx_y - 2];

      ca_in_6 = cells[ca_index_0][eacx_y - 3];
      ca_in_7 = cells[ca_index_1][eacx_y - 3];
      ca_in_8 = cells[ca_index_2][eacx_y - 3];

      ca_out_address = CA_OUT_ADDRESS_SPREAD_FACTOR * (
        (256 * ca_in_8) + (128 * ca_in_7) + (64 * ca_in_6)
        + (32 * ca_in_5) + (16 * ca_in_4) + (8 * ca_in_3)
        + (4 * ca_in_2) + (2 * ca_in_1) + (1 * ca_in_0)
        );

      ca_out = x_core_bitarray_get_bit(actor->solution, ca_out_address);
      cells[eacx_x][eacx_y] = ca_out;
    }
  }

  result_bit_0 = cells[0][EFE_LENGTH - 1];
  result_bit_1 = cells[1][EFE_LENGTH - 1];
  result_bit_2 = cells[2][EFE_LENGTH - 1];
  result = (4 * result_bit_0) + (2 * result_bit_1) + result_bit_2;

  return result;
}

x_core_bool_t fitness_is_superior(double fitness_a, double fitness_b,
    inferno_core_goal_t goal)
{
  x_core_bool_t superior;

  /*
    TODO: in general, what is the number optimal for society? :: is there a
    percentage, in general, of battles that should be arbitrarily decided, such
    that the optimal amount of diversion from the goal is kept in a culture?
    that diversion from the goal facilitiates diversity that aids in the goal
    :: is there some governing relationship to be found there?
  */
  if (0 == (random() % ANARCHY_MODULUS)) {
    superior = x_core_bool_true;
  } else {
    if (INFERNO_CORE_GOAL_MAXIMIZE_SCORE == goal) {
      if (fitness_a > fitness_b) {
        superior = x_core_bool_true;
      } else {
        superior = x_core_bool_false;
      }
    } else {
      if (fitness_a < fitness_b) {
        superior = x_core_bool_true;
      } else {
        superior = x_core_bool_false;
      }
    }
  }

  return superior;
}

unsigned long get_position_index_for_observe_location(inferno_bios_actor_t *actor,
    unsigned long observe_location)
{
  unsigned long observed_0;
  unsigned long observed_1;
  unsigned long observed_2;
  unsigned long observed_3;
  unsigned long observed_4;
  unsigned long observed_5;
  unsigned long observed_6;
  unsigned long observed_7;

  unsigned long observed_8;
  unsigned long observed_9;
  unsigned long observed_10;
  unsigned long observed_11;
  unsigned long observed_12;
  unsigned long observed_13;
  unsigned long observed_14;
  unsigned long observed_15;

  unsigned long observed_16;
  unsigned long observed_17;
  unsigned long observed_18;
  unsigned long observed_19;
  unsigned long observed_20;
  unsigned long observed_21;
  unsigned long observed_22;
  unsigned long observed_23;

  inferno_bios_actor_t *actor_0;
  inferno_bios_actor_t *actor_1;
  inferno_bios_actor_t *actor_2;
  inferno_bios_actor_t *actor_3;
  inferno_bios_actor_t *actor_4;
  inferno_bios_actor_t *actor_5;
  inferno_bios_actor_t *actor_6;
  inferno_bios_actor_t *actor_7;

  inferno_box_coordinate_t relation;
  unsigned long new_position_index;
  inferno_box_system_t *box;

  box = inferno_bios_system_get_box(actor->system);

  inferno_box_coordinate_init_witx_xyz(&relation, 0, 0, 1);
  actor_0 = inferno_box_system_find_relative(box, actor, &relation);

  inferno_box_coordinate_init_witx_xyz(&relation, 1, 0, 1);
  actor_1 = inferno_box_system_find_relative(box, actor, &relation);

  inferno_box_coordinate_init_witx_xyz(&relation, 2, 0, 1);
  actor_2 = inferno_box_system_find_relative(box, actor, &relation);

  inferno_box_coordinate_init_witx_xyz(&relation, 2, 1, 1);
  actor_3 = inferno_box_system_find_relative(box, actor, &relation);

  inferno_box_coordinate_init_witx_xyz(&relation, 2, 2, 1);
  actor_4 = inferno_box_system_find_relative(box, actor, &relation);

  inferno_box_coordinate_init_witx_xyz(&relation, 1, 2, 1);
  actor_5 = inferno_box_system_find_relative(box, actor, &relation);

  inferno_box_coordinate_init_witx_xyz(&relation, 0, 2, 1);
  actor_6 = inferno_box_system_find_relative(box, actor, &relation);

  inferno_box_coordinate_init_witx_xyz(&relation, 0, 1, 1);
  actor_7 = inferno_box_system_find_relative(box, actor, &relation);

  observed_0 = x_core_bitarray_get_bit(actor_0->solution, observe_location);
  observed_1 = x_core_bitarray_get_bit(actor_1->solution, observe_location);
  observed_2 = x_core_bitarray_get_bit(actor_2->solution, observe_location);
  observed_3 = x_core_bitarray_get_bit(actor_3->solution, observe_location);
  observed_4 = x_core_bitarray_get_bit(actor_4->solution, observe_location);
  observed_5 = x_core_bitarray_get_bit(actor_5->solution, observe_location);
  observed_6 = x_core_bitarray_get_bit(actor_6->solution, observe_location);
  observed_7 = x_core_bitarray_get_bit(actor_7->solution, observe_location);

  observed_8 = x_core_bitarray_get_bit(actor_0->solution,
      observe_location + OBSERVE_LOCATION_SPREAD_DISTANCE);
  observed_9 = x_core_bitarray_get_bit(actor_1->solution,
      observe_location + OBSERVE_LOCATION_SPREAD_DISTANCE);
  observed_10 = x_core_bitarray_get_bit(actor_2->solution,
      observe_location + OBSERVE_LOCATION_SPREAD_DISTANCE);
  observed_11 = x_core_bitarray_get_bit(actor_3->solution,
      observe_location + OBSERVE_LOCATION_SPREAD_DISTANCE);
  observed_12 = x_core_bitarray_get_bit(actor_4->solution,
      observe_location + OBSERVE_LOCATION_SPREAD_DISTANCE);
  observed_13 = x_core_bitarray_get_bit(actor_5->solution,
      observe_location + OBSERVE_LOCATION_SPREAD_DISTANCE);
  observed_14 = x_core_bitarray_get_bit(actor_6->solution,
      observe_location + OBSERVE_LOCATION_SPREAD_DISTANCE);
  observed_15 = x_core_bitarray_get_bit(actor_7->solution,
      observe_location + OBSERVE_LOCATION_SPREAD_DISTANCE);

  observed_16 = x_core_bitarray_get_bit(actor_0->solution,
      observe_location + (2 * OBSERVE_LOCATION_SPREAD_DISTANCE));
  observed_17 = x_core_bitarray_get_bit(actor_1->solution,
      observe_location + (2 * OBSERVE_LOCATION_SPREAD_DISTANCE));
  observed_18 = x_core_bitarray_get_bit(actor_2->solution,
      observe_location + (2 * OBSERVE_LOCATION_SPREAD_DISTANCE));
  observed_19 = x_core_bitarray_get_bit(actor_3->solution,
      observe_location + (2 * OBSERVE_LOCATION_SPREAD_DISTANCE));
  observed_20 = x_core_bitarray_get_bit(actor_4->solution,
      observe_location + (2 * OBSERVE_LOCATION_SPREAD_DISTANCE));
  observed_21 = x_core_bitarray_get_bit(actor_5->solution,
      observe_location + (2 * OBSERVE_LOCATION_SPREAD_DISTANCE));
  observed_22 = x_core_bitarray_get_bit(actor_6->solution,
      observe_location + (2 * OBSERVE_LOCATION_SPREAD_DISTANCE));
  observed_23 = x_core_bitarray_get_bit(actor_7->solution,
      observe_location + (2 * OBSERVE_LOCATION_SPREAD_DISTANCE));

  if (x_core_bool_false) {
    new_position_index = eight_from_eight(actor, observed_0, observed_1,
        observed_2, observed_3, observed_4, observed_5, observed_6,
        observed_7);
    new_position_index = eight_from_sixteen(actor, observed_0, observed_1,
        observed_2, observed_3, observed_4, observed_5, observed_6,
        observed_7, observed_8, observed_9, observed_10, observed_11,
        observed_12, observed_13, observed_14, observed_15);
  }
  new_position_index = eight_from_twenty_four(actor, observed_0,
      observed_1, observed_2, observed_3, observed_4, observed_5,
      observed_6, observed_7, observed_8, observed_9, observed_10,
      observed_11, observed_12, observed_13, observed_14, observed_15,
      observed_16, observed_17, observed_18, observed_19, observed_20,
      observed_21, observed_22, observed_23);

  return new_position_index;
}

inferno_box_coordinate_t get_relative_position_from_index(unsigned long index)
{
  assert(index < 8);
  inferno_box_coordinate_t position;

  position.x = 1;
  position.y = 1;
  position.z = 1;

  switch (index) {
    case 0:
      position.x = 0;
      position.y = 0;
      break;
    case 1:
      position.x = 1;
      position.y = 0;
      break;
    case 2:
      position.x = 2;
      position.y = 0;
      break;
    case 3:
      position.x = 2;
      position.y = 1;
      break;
    case 4:
      position.x = 2;
      position.y = 2;
      break;
    case 5:
      position.x = 1;
      position.y = 2;
      break;
    case 6:
      position.x = 0;
      position.y = 2;
      break;
    case 7:
      position.x = 0;
      position.y = 1;
      break;
  }

  return position;
}

double get_score(inferno_bios_actor_t *actor)
{
  assert(actor);
  inferno_core_score_solution_f score_solution;
  void *context;
  x_audit_log_t *log;

  if (!actor->score_is_valid) {
    context = inferno_bios_system_get_context(actor->system);
    score_solution = inferno_bios_system_get_score_solution(actor->system);
    if (score_solution(context, actor->solution, &actor->score)) {
      actor->score_is_valid = x_core_bool_true;
    } else {
      log = inferno_bios_system_get_log(actor->system);
      x_audit_log_trace(log, "bios", "score_solution");
    }
  }

  return actor->score;
}

void inferno_bios_actor_act(inferno_bios_actor_t *actor)
{
  if (DO_MOVE) {
    move_actor(actor);
  }
  if (DO_MEET) {
    meet_actor(actor);
  }
}

void *inferno_bios_actor_copy(void *actor_void)
{
  x_core_trace_exit("TODO: implement");
  return NULL;
}

void *inferno_bios_actor_create(void *system_void, x_core_bitarray_t *solution)
{
  assert(system_void);
  assert(solution);
  inferno_bios_actor_t *actor;
  inferno_bios_system_t *system;

  system = system_void;

  actor = malloc(sizeof *actor);
  if (actor) {
    actor->system = system;
    actor->score_is_valid = x_core_bool_false;
    actor->solution = x_core_bitarray_copy(solution);
    if (!actor->solution) {
      x_core_trace("inferno_core_birarray_copy");
      free(actor);
      actor = NULL;
    }
  } else {
    x_core_trace_exit("malloc");
  }

  return actor;
}

void *inferno_bios_actor_create_random(void *system_void)
{
  assert(system_void);
  x_core_bitarray_t *bitarray;
  inferno_bios_actor_t *actor;
  x_audit_log_t *log;
  inferno_bios_system_t *system;

  system = system_void;

  bitarray = x_core_bitarray_create_random(INFERNO_CORE_SOLUTION_BIT_COUNT);
  if (bitarray) {
    actor = inferno_bios_actor_create(system, bitarray);
    if (!actor) {
      log = inferno_bios_system_get_log(system);
      x_audit_log_trace(log, "bios", "inferno_bios_actor_create");
    }
    x_core_bitarray_destroy(bitarray);
  } else {
    actor = NULL;
    log = inferno_bios_system_get_log(system);
    x_audit_log_trace(log, "bios", "x_core_bitarray_create_random");
  }

  return actor;
}

int inferno_bios_actor_compare_maximize(void *actor_a_void, void *actor_b_void)
{
  assert(actor_a_void);
  assert(actor_b_void);
  inferno_bios_actor_t **actor_a;
  inferno_bios_actor_t **actor_b;
  double actor_a_score;
  double actor_b_score;
  long compare_result;

  actor_a = (inferno_bios_actor_t **) actor_a_void;
  actor_b = (inferno_bios_actor_t **) actor_b_void;

  actor_a_score = get_score((inferno_bios_actor_t *) *actor_a);
  actor_b_score = get_score((inferno_bios_actor_t *) *actor_b);
  if (actor_a_score > actor_b_score) {
    compare_result = -1;
  } else if (actor_a_score < actor_b_score) {
    compare_result = 1;
  } else {
    compare_result = 0;
  }

  return compare_result;
}

int inferno_bios_actor_compare_minimize(void *actor_a_void, void *actor_b_void)
{
  assert(actor_a_void);
  assert(actor_b_void);
  inferno_bios_actor_t **actor_a;
  inferno_bios_actor_t **actor_b;
  double actor_a_score;
  double actor_b_score;
  long compare_result;

  actor_a = (inferno_bios_actor_t **) actor_a_void;
  actor_b = (inferno_bios_actor_t **) actor_b_void;

  actor_a_score = get_score((inferno_bios_actor_t *) *actor_a);
  actor_b_score = get_score((inferno_bios_actor_t *) *actor_b);
  if (actor_a_score < actor_b_score) {
    compare_result = -1;
  } else if (actor_a_score > actor_b_score) {
    compare_result = 1;
  } else {
    compare_result = 0;
  }

  return compare_result;
}

void inferno_bios_actor_destroy(void *actor_void)
{
  assert(actor_void);
  inferno_bios_actor_t *actor;

  actor = actor_void;
  x_core_bitarray_destroy(actor->solution);

  free(actor);
}

void *inferno_bios_actor_get_box_cell(void *actor_void)
{
  assert(actor_void);
  inferno_bios_actor_t *actor;

  actor = actor_void;

  return actor->box_cell;
}

x_core_bitarray_t *inferno_bios_actor_get_solution(void *actor_void)
{
  assert(actor_void);
  inferno_bios_actor_t *actor;

  actor = actor_void;

  return actor->solution;
}

void inferno_bios_actor_init_actorey(inferno_core_actorey_t *actorey)
{
  inferno_core_actorey_init(actorey,
      inferno_bios_actor_create, inferno_bios_actor_create_random,
      inferno_bios_actor_compare_minimize, inferno_bios_actor_copy,
      inferno_bios_actor_destroy, inferno_bios_actor_get_box_cell,
      inferno_bios_actor_set_box_cell);
}

void inferno_bios_actor_set_box_cell(void *actor_void, void *box_cell)
{
  assert(actor_void);
  inferno_bios_actor_t *actor;

  actor = actor_void;

  actor->box_cell = box_cell;
}

void meet_actor(inferno_bios_actor_t *actor)
{
  unsigned long exchange_position_index;
  inferno_box_coordinate_t exchange_position_relative;
  inferno_bios_actor_t *actor_to_exchange_with;
  unsigned long fscore_this;
  unsigned long fscore_other;
  double fitness_this;
  double fitness_other;
  inferno_box_system_t *box;
  inferno_core_goal_t goal;

  box = inferno_bios_system_get_box(actor->system);

  exchange_position_index = get_position_index_for_observe_location(actor,
      inferno_core_get_gene_start_address(actor->solution, GENE_INDEX_MEET_WHO));
  exchange_position_relative
    = get_relative_position_from_index(exchange_position_index);
  actor_to_exchange_with = inferno_box_system_find_relative
    (box, actor, &exchange_position_relative);

  fitness_this = get_score(actor);
  fitness_other = get_score(actor_to_exchange_with);
  goal = inferno_bios_system_get_goal(actor->system);
  if (fitness_is_superior(fitness_this, fitness_other, goal)) {
    fscore_this = 1;
    fscore_other = 0;
  } else {
    fscore_this = 0;
    fscore_other = 1;
  }

  if (fscore_this >= fscore_other) {
    meet_actor_details(actor, actor_to_exchange_with);
  } else {
    meet_actor_details(actor_to_exchange_with, actor);
  }
}

void meet_actor_details(inferno_bios_actor_t *actor_a, inferno_bios_actor_t *actor_b)
{
  assert(actor_a);
  assert(actor_b);
  meet_gene_t meet_gene;
  unsigned long meet_gene_start_address;
  unsigned long eacx_gene;
  inferno_box_system_t *box;
  x_core_bit_t bit;

  meet_gene_start_address
    = inferno_core_get_gene_start_address(actor_a->solution, GENE_INDEX_MEET);
  parse_meet_gene(actor_a, meet_gene_start_address, &meet_gene);

  actor_b->score_is_valid = x_core_bool_false;

  for (eacx_gene = meet_gene.address;
       eacx_gene < (meet_gene.address + meet_gene.length);
       eacx_gene++) {
    bit = x_core_bitarray_get_bit(actor_a->solution, eacx_gene);
    x_core_bitarray_set_bit(actor_b->solution, eacx_gene, bit);
    if (MUTATION) {
      if (0 == x_core_random_unsigned_long(MUTATION_INCIDENCE_PER)) {
        x_core_bitarray_set_bit(actor_b->solution, eacx_gene,
            x_core_random_unsigned_long(2));
      }
    }
  }

  box = inferno_bios_system_get_box(actor_a->system);
  inferno_box_system_swap(box, actor_a, actor_b);
}

void move_actor(inferno_bios_actor_t *actor)
{
  assert(actor);
  unsigned long new_position_index;
  inferno_box_coordinate_t new_position_relative;
  inferno_box_system_t *box;
  inferno_bios_actor_t *actor_to_switcinferno_with;
  unsigned long move_gene_start_address;

  box = inferno_bios_system_get_box(actor->system);

  move_gene_start_address
    = inferno_core_get_gene_start_address(actor->solution, GENE_INDEX_MOVE);
  new_position_index = get_position_index_for_observe_location(actor,
      move_gene_start_address);
  new_position_relative = get_relative_position_from_index(new_position_index);

  actor_to_switcinferno_with = inferno_box_system_find_relative(box, actor,
      &new_position_relative);
  inferno_box_system_swap(box, actor, actor_to_switcinferno_with);
}

void parse_display_gene(inferno_bios_actor_t *actor,
    unsigned long gene_start_address, display_gene_t *display_gene)
{
  display_gene->red = x_core_bitarray_get_unsigned_char(actor->solution,
      gene_start_address + 0);
  display_gene->green = x_core_bitarray_get_unsigned_char(actor->solution,
      gene_start_address + 8);
  display_gene->blue = x_core_bitarray_get_unsigned_char(actor->solution,
      gene_start_address + 16);
}

/*
  TODO: change this to use the actor's built in "random" number generator
*/
void parse_meet_gene(inferno_bios_actor_t *actor, unsigned long gene_start_address,
    meet_gene_t *meet_gene)
{
  meet_gene->address = x_core_bitarray_get_unsigned_char(actor->solution,
      gene_start_address + 0);
  meet_gene->length = x_core_bitarray_get_unsigned_char(actor->solution,
      gene_start_address + 8);
}
