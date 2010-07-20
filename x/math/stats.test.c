#include "x/audit/log.h"
#include "x/config/tools.h"
#include "x/math/stats.h"
#include "x/math/tools.h"

static int test_prf(void);

struct _math_stats_test_case
{
  char *name;
  int (*test_function)(void);
};
typedef struct _math_stats_test_case math_stats_test_case;

x_audit_log_t *the_log = NULL;

math_stats_test_case test_cases[] =
{
  { "precision, recall, f-measure", test_prf },
  { NULL, NULL}
};

int main(int argc, char *argv[])
{
  x_config_disable_if_running_batcx_tests(argc, argv);

  int result = 0;
  int line = 0;
  unsigned long ii = 0;

  the_log = x_audit_log_create(stdout);

  for (ii = 0; test_cases[ii].name != NULL; ii++) {
    if ((line = test_cases[ii].test_function()) != 0) {
      x_audit_log_enter(the_log, "math", "... Test <%s> FAILED (here: %d)",
          test_cases[ii].name, line);
      result++;
    } else {
      x_audit_log_enter(the_log, "math", "... Test <%s> passed",
          test_cases[ii].name);
    }
  }

  if (result != 0) {
    x_audit_log_enter(the_log, "math", "Unit Test FAILED");
  }

  x_audit_log_destroy(the_log);
  return result;
}

static int test_prf(void)
{
  if (x_math_stats_get_precision(0, 0) != 0) {
    return __LINE__;
  }

  if (x_math_stats_get_recall(0, 0) != 0) {
    return __LINE__;
  }

  if (x_math_stats_get_f_measure(0, 0, 0) != 0) {
    return __LINE__;
  }


  if (x_math_stats_get_precision(10, 0) != 1.0) {
    return __LINE__;
  }

  if (x_math_stats_get_precision(10, 10) != .5) {
    return __LINE__;
  }


  if (x_math_stats_get_recall(10, 0) != 1.0) {
    return __LINE__;
  }

  if (x_math_stats_get_recall(10, 10) != .5) {
    return __LINE__;
  }


  if (x_math_stats_get_f_measure(.5, .5, 1.0) != .5) {
    return __LINE__;
  }

  printf("%f\n", x_math_stats_get_f_measure(.75, .5, 1.0));
  if (x_math_tools_equal_double
      (x_math_stats_get_f_measure(.75, .5, 1.0), 0.6)) {
    return __LINE__;
  }

  if (!x_math_tools_equal_double
      (x_math_stats_get_f_measure(.3, .9, 3.0), 0.75)) {
    return __LINE__;
  }

  return 0;
}
