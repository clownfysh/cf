#include "cf/inferno/search/system.h"
#include "cf/x/core/log.h"
#include "cf/x/config/options.h"
#include "cf/x/core/bitarray.h"
#include "cf/x/core/objects.h"
#include "cf/x/file/basic.h"
#include "cf/x/file/csv.h"
#include "cf/x/math/tools.h"

#define DEFAULT_SEARCH_ALGORITHM CF_INFERNO_SEARCH_ALGORITHM_BIOS
#define MAX_POINTS 64
#define MAX_OVERALL_TIME_MICROSECONDS 1000000 * 60 * 5
#define MAX_SEARCH_TIME_MICROSECONDS 1000000
#define POINT_RANGE 100
#define POINTS_FILENAME "stravel_points.csv"
#define SOLUTION_FILENAME "stravel_solution.csv"

#define RANDOM_POINT_COUNT cf_x_core_min(64, MAX_POINTS)

struct stravel_t {
  unsigned short point_count;
  double points[MAX_POINTS][2];
  double distances[MAX_POINTS][MAX_POINTS];
  unsigned short current_point;
  cf_x_core_log_t *log;
};
typedef struct stravel_t stravel_t;

struct comparable_point_t {
  unsigned short index;
  stravel_t *stravel;
};
typedef struct comparable_point_t comparable_point_t;

static void cache_distances(stravel_t *stravel);

static double calculate_point_distance(stravel_t *stravel,
    unsigned short point_a, unsigned short point_b);

static int compare_points(const void *comparable_point_a_void,
    const void *comparable_point_b_void);

static void create_random_points(stravel_t *stravel);

static unsigned short get_fork_point(stravel_t *stravel,
    unsigned short current_point, unsigned char fork,
    cf_x_core_bool_t visited[]);

static double get_point_distance_from_cache(stravel_t *stravel,
    unsigned short point_a, unsigned short point_b);

static cf_x_core_bool_t load_points_from_file(stravel_t *stravel,
    char *points_filename);

static cf_x_core_bool_t save_points_to_file(stravel_t *stravel,
    char *filename);

static cf_x_core_bool_t save_solution_to_file(stravel_t *stravel,
    cf_x_core_bitarray_t *solution, char *filename);

static cf_x_core_bool_t score_solution(void *context,
    cf_x_core_bitarray_t *solution, double *score);

static void cache_distances(stravel_t *stravel)
{
  unsigned short i;
  unsigned short j;

  for (i = 0; i < stravel->point_count; i++) {
    for (j = 0; j < stravel->point_count; j++) {
      stravel->distances[i][j] = calculate_point_distance(stravel, i, j);
    }
  }
}

double calculate_point_distance(stravel_t *stravel, unsigned short point_a,
    unsigned short point_b)
{
  double d;
  double x1;
  double y1;
  double x2;
  double y2;

  x1 = stravel->points[point_a][0];
  y1 = stravel->points[point_a][1];
  x2 = stravel->points[point_b][0];
  y2 = stravel->points[point_b][1];

  d = cf_x_math_tools_calculate_distance(x1, y1, x2, y2);

  return d;
}

int compare_points(const void *comparable_point_a_void,
    const void *comparable_point_b_void)
{
  assert(comparable_point_a_void);
  assert(comparable_point_b_void);
  const comparable_point_t *comparable_point_a;
  const comparable_point_t *comparable_point_b;
  unsigned short point_a;
  unsigned short point_b;
  int compare_result;
  double distance_to_a;
  double distance_to_b;
  stravel_t *stravel;

  comparable_point_a = (const comparable_point_t *) comparable_point_a_void;
  comparable_point_b = (const comparable_point_t *) comparable_point_b_void;

  point_a = comparable_point_a->index;
  point_b = comparable_point_b->index;
  stravel = comparable_point_a->stravel;

  distance_to_a
    = get_point_distance_from_cache(stravel, stravel->current_point, point_a);
  distance_to_b
    = get_point_distance_from_cache(stravel, stravel->current_point, point_b);

  if (distance_to_a < distance_to_b) {
    compare_result = -1;
  } else if (distance_to_b < distance_to_a) {
    compare_result = 1;
  } else {
    compare_result = 0;
  }

  return compare_result;
}

void create_random_points(stravel_t *stravel)
{
  unsigned short i;

  stravel->point_count = RANDOM_POINT_COUNT;

  cf_x_core_seed_random_with_time();
  for (i = 0; i < stravel->point_count; i++) {
    stravel->points[i][0] = ((random() % 1000000) / 1000000.0) * POINT_RANGE;
    stravel->points[i][1] = ((random() % 1000000) / 1000000.0) * POINT_RANGE;
  }
}

unsigned short get_fork_point(stravel_t *stravel, unsigned short current_point,
    unsigned char fork, cf_x_core_bool_t visited[])
{
  comparable_point_t closest_points[stravel->point_count];
  unsigned short i;
  unsigned short j;
  unsigned short next_point;

  j = 0;
  for (i = 0; i < stravel->point_count; i++) {
    if (!visited[i]) {
      closest_points[j].index = i;
      closest_points[j].stravel = stravel;
      j++;
    }
  }

  stravel->current_point = current_point;
  qsort(closest_points, j, sizeof(comparable_point_t), compare_points);

  if (fork < j) {
    next_point = closest_points[fork].index;
  } else {
    next_point = closest_points[j - 1].index;
  }

  return next_point;
}

double get_point_distance_from_cache(stravel_t *stravel,
    unsigned short point_a, unsigned short point_b)
{
  return stravel->distances[point_a][point_b];
}

cf_x_core_bool_t load_points_from_file(stravel_t *stravel,
    char *points_filename)
{
  cf_x_file_csv_t *csv;
  cf_x_core_bool_t success;
  unsigned short each_point;
  double x;
  double y;

  csv = cf_x_file_csv_create_extended(points_filename, 0, MAX_POINTS - 1);
  if (csv) {
    success = cf_x_core_bool_true;
    stravel->point_count = cf_x_file_csv_get_object_count(csv);
    cf_x_core_log_enter(stravel->log, "stvl", "loading %lu points from %s",
        stravel->point_count, points_filename);
    for (each_point = 0; each_point < stravel->point_count; each_point++) {
      x = cf_x_file_csv_get_value_by_name_as_double(csv, each_point, "x");
      y = cf_x_file_csv_get_value_by_name_as_double(csv, each_point, "y");
      stravel->points[each_point][0] = x;
      stravel->points[each_point][1] = y;
    }
    cf_x_file_csv_destroy(csv);
  } else {
    success = cf_x_core_bool_false;
    cf_x_core_log_trace(stravel->log, "stvl", "x_file_csv_create");
  }

  return success;
}

cf_x_core_bool_t save_points_to_file(stravel_t *stravel, char *filename)
{
  assert(stravel);
  cf_x_file_basic_t *file;
  cf_x_core_bool_t success;
  unsigned short each_point;
  double x;
  double y;
  char string[64];

  file = cf_x_file_basic_create
    (filename, CF_X_FILE_MODE_TRUNCATE_OR_CREATE_FOR_WRITE);
  if (file) {
    success = cf_x_core_bool_true;

    if (!cf_x_file_basic_write_string(file, "x,y\n")) {
      success = cf_x_core_bool_false;
      cf_x_core_log_trace(stravel->log, "stvl", "x_file_basic_write_string");
    }

    for (each_point = 0; each_point < stravel->point_count; each_point++) {
      x = stravel->points[each_point][0];
      y = stravel->points[each_point][1];
      if (sprintf(string, "%.5f,%.5f\n", x, y) >= 0) {  /*  TODO  */
        if (!cf_x_file_basic_write_string(file, string)) {
          success = cf_x_core_bool_false;
          cf_x_core_log_trace
            (stravel->log, "stvl", "x_file_basic_write_string");
          break;
        }
      } else {
        success = cf_x_core_bool_false;
        cf_x_core_log_trace(stravel->log, "stvl", "sprintf");
        break;
      }
    }
    cf_x_file_basic_destroy(file);
  } else {
    success = cf_x_core_bool_false;
    cf_x_core_log_trace(stravel->log, "stvl", "x_file_basic_create");
  }

  return success;
}

cf_x_core_bool_t save_solution_to_file(stravel_t *stravel,
    cf_x_core_bitarray_t *solution, char *filename)
{
  assert(stravel);
  assert(filename);
  cf_x_core_bool_t visited[MAX_POINTS];
  unsigned short each_point;
  unsigned char bit_0;
  unsigned char bit_1;
  unsigned short fork;
  unsigned short current_point;
  unsigned short next_point;
  cf_x_file_basic_t *file;
  cf_x_core_bool_t success;
  double x;
  double y;
  char string[64];

  for (each_point = 0; each_point < stravel->point_count; each_point++) {
    visited[each_point] = cf_x_core_bool_false;
  }

  file = cf_x_file_basic_create
    (filename, CF_X_FILE_MODE_TRUNCATE_OR_CREATE_FOR_WRITE);
  if (file) {
    success = cf_x_core_bool_true;

    if (!cf_x_file_basic_write_string(file, "x,y\n")) {
      success = cf_x_core_bool_false;
      cf_x_core_log_trace(stravel->log, "stvl", "x_file_basic_write_string");
    }

    current_point = 0;
    x = stravel->points[current_point][0];
    y = stravel->points[current_point][1];
    if (sprintf(string, "%.5f,%.5f\n", x, y) >= 0) {  /*  TODO  */
      if (!cf_x_file_basic_write_string(file, string)) {
        success = cf_x_core_bool_false;
        cf_x_core_log_trace(stravel->log, "stvl", "x_file_basic_write_string");
      }
    } else {
      success = cf_x_core_bool_false;
      cf_x_core_log_trace(stravel->log, "stvl", "sprintf");
    }
    visited[current_point] = cf_x_core_bool_true;

    for (each_point = 0; each_point < (stravel->point_count - 1);
         each_point++) {
      bit_0 = cf_x_core_bitarray_get_bit(solution, (current_point * 2) + 0);
      bit_1 = cf_x_core_bitarray_get_bit(solution, (current_point * 2) + 1);
      fork = (bit_0 * 1) + (bit_1 * 2);
      next_point = get_fork_point(stravel, current_point, fork, visited);
      x = stravel->points[next_point][0];
      y = stravel->points[next_point][1];
      if (sprintf(string, "%.5f,%.5f\n", x, y) >= 0) {  /*  TODO  */
        if (!cf_x_file_basic_write_string(file, string)) {
          success = cf_x_core_bool_false;
          cf_x_core_log_trace
            (stravel->log, "stvl", "x_file_basic_write_string");
          break;
        }
      } else {
        success = cf_x_core_bool_false;
        cf_x_core_log_trace(stravel->log, "stvl", "sprintf");
        break;
      }

      visited[next_point] = cf_x_core_bool_true;
      current_point = next_point;
    }
    cf_x_file_basic_destroy(file);
  } else {
    success = cf_x_core_bool_false;
    cf_x_core_log_trace(stravel->log, "stvl", "x_file_basic_create");
  }

  return success;
}

cf_x_core_bool_t score_solution(void *context, cf_x_core_bitarray_t *solution,
    double *score)
{
  assert(score);
  assert(solution);
  assert(score);
  cf_x_core_bool_t visited[MAX_POINTS];
  unsigned short i;
  unsigned bit_0;
  unsigned bit_1;
  unsigned short fork;
  unsigned short current_point;
  unsigned short next_point;
  double distance;
  stravel_t *stravel;

  stravel = context;

  *score = 0.0;

  for (i = 0; i < stravel->point_count; i++) {
    visited[i] = cf_x_core_bool_false;
  }

  current_point = 0;
  visited[0] = cf_x_core_bool_true;
  for (i = 0; i < (stravel->point_count - 1); i++) {
    bit_0 = cf_x_core_bitarray_get_bit(solution, (current_point * 2) + 0);
    bit_1 = cf_x_core_bitarray_get_bit(solution, (current_point * 2) + 1);
    fork = (bit_0 * 1) + (bit_1 * 2);
    next_point = get_fork_point(stravel, current_point, fork, visited);
    visited[next_point] = cf_x_core_bool_true;
    distance
      = get_point_distance_from_cache(stravel, current_point, next_point);
    *score += distance;
    current_point = next_point;
  }

  return cf_x_core_bool_true;
}

int main(int argc, char *argv[])
{
  cf_x_config_options_t *options;
  cf_x_core_objects_t objects;
  cf_x_core_log_t *log;
  char *points_filename;
  cf_inferno_search_system_t *system;
  cf_x_case_array_t *initial_solutions;
  /*  unsigned short initial_solution_count;  */
  cf_x_case_array_t *solutions;
  unsigned short solution_count;
  cf_x_core_bool_t still_searching;
  unsigned short i;
  cf_x_core_bitarray_t *solution;
  double score;
  double best_score;
  stravel_t stravel;
  double goal_distance;
  double sqrt_point_count;
  struct timeval start_time;
  char *search_algorithm_string;
  cf_inferno_search_algorithm_t search_algorithm;

  initial_solutions = NULL;
  /*  initial_solution_count = 0;  */

  cf_x_core_objects_init(&objects);

  log = cf_x_core_log_create(stdout);
  if (!log) {
    cf_x_core_trace_exit("x_audit_log_create");
  }
  stravel.log = log;

  options = cf_x_config_options_create(argc, argv, &objects);
  if (!options) {
    cf_x_core_log_trace_exit(log, "stvl", "x_config_options_create");
  }

  if (cf_x_config_options_find_as_string(options, "points_filename",
          &points_filename, "")) {
    if (!load_points_from_file(&stravel, points_filename)) {
      cf_x_core_log_trace_exit(log, "stvl", "load_points_from_file");
    }
  } else {
    cf_x_core_log_enter
      (log, "stvl", "creating %lu random points", RANDOM_POINT_COUNT);
    create_random_points(&stravel);
  }

  if (cf_x_config_options_find_as_string(options, "search_algorithm",
          &search_algorithm_string, "")) {
    if (!cf_inferno_search_algorithm_get_from_string
        (search_algorithm_string, &search_algorithm)) {
      search_algorithm = DEFAULT_SEARCH_ALGORITHM;
    }
  } else {
    search_algorithm = DEFAULT_SEARCH_ALGORITHM;
  }
  search_algorithm_string
    = cf_inferno_search_algorithm_get_string(search_algorithm);
  cf_x_core_log_enter(log, "stvl", "search algorithm: %s",
      search_algorithm_string);

  cf_x_core_log_enter(log, "stvl", "saving points to stravel_points.csv");
  if (!save_points_to_file(&stravel, POINTS_FILENAME)) {
    cf_x_core_log_trace_exit(log, "stvl", "save_points_to_file");
  }

  sqrt_point_count = sqrt(stravel.point_count);
  goal_distance = ((POINT_RANGE * sqrt_point_count) + POINT_RANGE) * 1.0;

  cf_x_core_log_enter(log, "stvl", "caching point distances");
  cache_distances(&stravel);

  cf_x_core_log_enter(log, "stvl", "creating search system");
  system = cf_inferno_search_system_create(score_solution,
      CF_INFERNO_CORE_GOAL_MINIMIZE_SCORE, &stravel, initial_solutions,
      search_algorithm, log);
  if (system) {
    cf_x_core_log_enter(log, "stvl", "goal distance: %.2f", goal_distance);
    gettimeofday(&start_time, NULL);
    still_searching = cf_x_core_bool_true;
    while (still_searching) {
      cf_inferno_search_system_search(system, MAX_SEARCH_TIME_MICROSECONDS);
      solutions = cf_inferno_search_system_get_solutions_copy(system, 1);
      if (solutions) {
        solution = cf_x_case_array_find(solutions, 0);
        if (!score_solution(&stravel, solution, &best_score)) {
          cf_x_core_log_trace_exit(log, "stvl", "score_solution");
        }
        solution_count = cf_x_case_array_get_size(solutions);
        for (i = 0; i < solution_count; i++) {
          solution = cf_x_case_array_find(solutions, i);
          if (!score_solution(&stravel, solution, &score)) {
            cf_x_core_log_trace_exit(log, "stvl", "score_solution");
          }
          if (cf_x_core_bool_true) {
            cf_x_core_log_enter(log, "stvl", "distance: %.2f", score);
          }
          fflush(stdout);
          if (score <= goal_distance) {
            still_searching = cf_x_core_bool_false;
            cf_x_core_log_enter
              (log, "stvl", "reached goal distance, stopping search");
          }
        }
        cf_x_case_array_destroy(solutions);
      } else {
        cf_x_core_log_trace_exit
          (log, "stvl", "inferno_search_system_get_solutions_copy");
        break;
      }
      if (!cf_x_core_time_is_remaining_microseconds
          (&start_time, MAX_OVERALL_TIME_MICROSECONDS)) {
        still_searching = cf_x_core_bool_false;
        cf_x_core_log_enter
          (log, "stvl", "max search time elapsed, stopping search");
      }
    }

    solutions = cf_inferno_search_system_get_solutions_copy(system, 1);
    if (solutions) {
      solution = cf_x_case_array_find(solutions, 0);
      cf_x_core_log_enter
        (log, "stvl", "saving solution to %s", SOLUTION_FILENAME);
      if (!save_solution_to_file(&stravel, solution, SOLUTION_FILENAME)) {
        cf_x_core_log_trace_exit(log, "stvl", "save_solution_to_file");
      }
      cf_x_case_array_destroy(solutions);
    } else {
      cf_x_core_log_trace_exit
        (log, "stvl", "inferno_search_system_get_solutions_copy");
    }

    cf_inferno_search_system_destroy(system);
  } else {
    cf_x_core_log_trace_exit(log, "stvl", "inferno_search_system_create");
  }

  cf_x_config_options_destroy(options);
  cf_x_core_log_destroy(log);

  return 0;
}
