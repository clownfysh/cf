#include "x/case/list.h"
#include "x/core/constants.h"
#include "x/core/period.h"
#include "x/core/string.h"
#include "x/core/tools.h"
#include "x/shell/system.h"

static void handle_signal(int signal);

void handle_signal(int signal)
{

  switch (signal) {
    case SIGUSR1:
      break;
    case SIGUSR2:
      break;
    default:
      x_core_stop_requested = x_core_bool_true;
      break;
  }

}

int main(int argc, char *argv[])
{
  return 0;

  x_core_shell_t *shell;
  x_core_period_t *period;
  x_case_list_t *input;
  char *command_string;

  signal(SIGINT, handle_signal);
  signal(SIGUSR1, handle_signal);
  signal(SIGUSR2, handle_signal);
  signal(SIGPIPE, SIG_IGN);

  shell = x_core_shell_create();
  if (!shell) {
    x_core_trace_exit("x_core_shell_create");
  }

  if (!x_core_shell_start(shell)) {
    x_core_trace_exit("x_core_shell_start");
  }

  period = x_core_period_create(2);
  if (!period) {
    x_core_trace_exit("x_core_period_create");
  }

  while (!x_core_stop_requested) {
    input = x_core_shell_take_input(shell);
    if (input) {
      x_case_list_iterate_start(input);
      while ((command_string = x_case_list_iterate_next(input))) {
        printf(":%s:\n", command_string);
      }
      x_case_list_destroy(input);
    }

    if (x_core_bool_false && x_core_period_once(period)) {
      x_core_shell_append_output(shell, "ok\n");
    }

    usleep(X_CORE_STANDARD_SLEEP_MICROSECONDS);
  }

  x_core_period_destroy(period);
  x_core_shell_destroy(shell);

  return 0;
}
