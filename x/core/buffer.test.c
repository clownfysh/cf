#include "x/core/buffer.h"
#include "x/core/tools.h"

static char *initial_buffer = "happy";
static char *additional_buffer_0 = " day for fun on the beach";
static char *additional_buffer_1 = " with a white paper rabbyt";

int main(int argc, char *argv[])
{
  x_core_buffer_t *buffer;
  char *string;
  unsigned long string_size;

  buffer = x_core_buffer_create(initial_buffer, strlen(initial_buffer));
  if (!buffer) {
    x_core_trace_exit("x_core_buffer_create");
  }

  if (!x_core_buffer_append_string(buffer, additional_buffer_0,
          strlen(additional_buffer_0))) {
    x_core_trace_exit("x_core_buffer_append_string");
  }
  if (!x_core_buffer_append_string(buffer, additional_buffer_1,
          strlen(additional_buffer_1))) {
    x_core_trace_exit("x_core_buffer_append_string");
  }

  string = x_core_buffer_get_string(buffer, &string_size);
  printf("string_size:%lu\n", string_size);
  printf(":%s:\n", string);

  x_core_buffer_destroy(buffer);

  return 0;
}
