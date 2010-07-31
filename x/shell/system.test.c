#include "cf/x/case/list.h"
#include "cf/x/core/constants.h"
#include "cf/x/core/period.h"
#include "cf/x/core/string.h"
#include "cf/x/core/tools.h"
#include "cf/x/shell/system.h"

static void handle_signal(int signal);

void handle_signal(int signal)
{

  switch (signal) {
    case SIGUSR1:
      break;
    case SIGUSR2:
      break;
    default:
      cf_x_core_stop_requested = cf_x_core_bool_true;
      break;
  }

}

int main(int argc, char *argv[])
{
  return 0;

  cf_x_core_shell_t *shell;
  cf_x_core_period_t *period;
  cf_x_case_list_t *input;
  char *command_string;

  signal(SIGINT, handle_signal);
  signal(SIGUSR1, handle_signal);
  signal(SIGUSR2, handle_signal);
  signal(SIGPIPE, SIG_IGN);

  shell = cf_x_core_shell_create();
  if (!shell) {
    cf_x_core_trace_exit("x_core_shell_create");
  }

  if (!cf_x_core_shell_start(shell)) {
    cf_x_core_trace_exit("x_core_shell_start");
  }

  period = cf_x_core_period_create(2);
  if (!period) {
    cf_x_core_trace_exit("x_core_period_create");
  }

  while (!cf_x_core_stop_requested) {
    input = cf_x_core_shell_take_input(shell);
    if (input) {
      cf_x_case_list_iterate_start(input);
      while ((command_string = cf_x_case_list_iterate_next(input))) {
        printf(":%s:\n", command_string);
      }
      cf_x_case_list_destroy(input);
    }

    if (cf_x_core_bool_false && cf_x_core_period_once(period)) {
      cf_x_core_shell_append_output(shell, "ok\n");
    }

    usleep(CF_X_CORE_STANDARD_SLEEP_MICROSECONDS);
  }

  cf_x_core_period_destroy(period);
  cf_x_core_shell_destroy(shell);

  return 0;
}
