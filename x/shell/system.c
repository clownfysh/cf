#include "x/core/buffer.h"
#include "x/core/constants.h"
#include "x/core/string.h"
#include "x/core/tools.h"
#include "x/shell/system.h"

#define INITIAL_IN_PROGRESS_INPUT ""

struct x_core_shell_t {
  x_case_list_t *new_input;
  pthread_mutex_t new_input_mutex;

  x_core_buffer_t *in_progress_input;
  pthread_t shell_thread;
};

static void *shell_thread(void *shell_object);

void x_core_shell_append_output(x_core_shell_t *shell, char *output)
{
  assert(shell);
  assert(output);

  printf("%s", output);
}

x_core_shell_t *x_core_shell_create()
{
  x_core_shell_t *shell;
  x_core_bool_t so_far_so_good;

  shell = malloc(sizeof *shell);
  if (shell) {
    so_far_so_good = x_core_bool_true;
  } else {
    x_core_trace("malloc");
    so_far_so_good = x_core_bool_false;
  }

  if (so_far_so_good) {
    shell->new_input = x_case_list_create(x_core_string_compare,
        x_core_string_copy, X_CORE_NO_DESTROY_FUNCTION);
    if (!shell->new_input) {
      so_far_so_good = x_core_bool_false;
    }
  }

  if (so_far_so_good) {
    if (0 != pthread_mutex_init(&shell->new_input_mutex, NULL)) {
      so_far_so_good = x_core_bool_false;
    }
  }

  if (so_far_so_good) {
    shell->in_progress_input = x_core_buffer_create(INITIAL_IN_PROGRESS_INPUT,
        strlen(INITIAL_IN_PROGRESS_INPUT));
    if (!shell->in_progress_input) {
      so_far_so_good = x_core_bool_false;
      x_core_trace("x_core_buffer_create");
    }
  }

  if (!so_far_so_good && shell) {
    if (shell->new_input) {
      x_case_list_destroy(shell->new_input);
    }
    if (shell->in_progress_input) {
      x_core_buffer_destroy(shell->in_progress_input);
    }
    free(shell);
    shell = NULL;
  }

  return shell;
}

void x_core_shell_destroy(x_core_shell_t *shell)
{
  assert(shell);

  pthread_join(shell->shell_thread, NULL);
  x_case_list_destroy(shell->new_input);
  pthread_mutex_destroy(&shell->new_input_mutex);
  x_core_buffer_destroy(shell->in_progress_input);
  free(shell);

  x_core_terminal_block();
}

x_core_bool_t x_core_shell_start(x_core_shell_t *shell)
{
  assert(shell);
  x_core_bool_t success;

  x_core_terminal_nonblock();

  if (0 == pthread_create(&shell->shell_thread, NULL, shell_thread, shell)) {
    success = x_core_bool_true;
  } else {
    success = x_core_bool_false;
  }

  return success;
}

x_case_list_t *x_core_shell_take_input(x_core_shell_t *shell)
{
  assert(shell);
  x_case_list_t *new_input;
  char *input_string;

  new_input = x_case_list_create(x_core_string_compare,
      x_core_string_copy, x_core_string_destroy);
  if (new_input) {

    pthread_mutex_lock(&shell->new_input_mutex);
    {
      x_case_list_iterate_start(shell->new_input);
      while ((input_string
              = x_case_list_iterate_next(shell->new_input))) {
        if (x_case_list_add_last(new_input, input_string)) {
          x_case_list_iterate_remove(shell->new_input);
        } else {
          x_core_trace("x_case_list_add_last");
        }
      }
    }
    pthread_mutex_unlock(&shell->new_input_mutex);

  } else {
    x_core_trace("x_case_list_create");
  }

  return new_input;
}

void *shell_thread(void *shell_object)
{
  assert(shell_object);
  x_core_shell_t *shell;
  int c;
  char *input_string;
  unsigned long input_string_size;

  shell = shell_object;

  while (!x_core_stop_requested) {
    if (x_core_key_hit()) {
      c = getchar();
      if ('\n' == c) {
        input_string = x_core_buffer_take_string(shell->in_progress_input,
            &input_string_size);
        if (input_string) {

          pthread_mutex_lock(&shell->new_input_mutex);
          {
            if (!x_case_list_add_last(shell->new_input, input_string)) {
              x_core_trace("x_case_list_add_last");
            }
          }
          pthread_mutex_unlock(&shell->new_input_mutex);

        }
      } else {
        x_core_buffer_append_char(shell->in_progress_input, c);
      }
    }
    usleep(X_CORE_STANDARD_SLEEP_MICROSECONDS);
  }

  return NULL;
}
