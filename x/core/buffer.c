#include "x/core/buffer.h"
#include "x/core/tools.h"

#define INITIAL_BUFFER_GROW_SIZE 1024
#define GROW_FACTOR 8

struct x_core_buffer_t {
  char *string;
  unsigned long string_size;
  unsigned long string_allocated_size;
  unsigned long grow_size;
};

x_core_bool_t x_core_buffer_append_char(x_core_buffer_t *buffer, char c)
{
  assert(buffer);
  x_core_bool_t success;
  char string[2];

  *string = c;
  *(string + 1) = '\0';
  success = x_core_buffer_append_string(buffer, buffer->string, 1);

  return success;
}

x_core_bool_t x_core_buffer_append_string(x_core_buffer_t *buffer,
    char *string, unsigned long string_size)
{
  assert(buffer);
  assert(string);
  x_core_bool_t success;
  unsigned long available_size;
  unsigned long new_allocated_size;
  unsigned long new_string_size;
  char *new_string;
  x_core_bool_t have_the_needed_space;

  if (buffer->string_allocated_size > 0) {
    available_size = (buffer->string_allocated_size - 1) - buffer->string_size;
  } else {
    available_size = 0;
  }
  if (string_size > available_size) {
    buffer->grow_size = string_size * GROW_FACTOR;
    new_allocated_size = buffer->string_allocated_size + buffer->grow_size;
    new_string = realloc(buffer->string, new_allocated_size);
    if (new_string) {
      buffer->string = new_string;
      buffer->string_allocated_size = new_allocated_size;
      have_the_needed_space = x_core_bool_true;
    } else {
      x_core_trace("realloc");
      have_the_needed_space = x_core_bool_false;
    }
  } else {
    have_the_needed_space = x_core_bool_true;
  }

  if (have_the_needed_space) {
    success = x_core_bool_true;
    memcpy(buffer->string + buffer->string_size, string, string_size);
    new_string_size = buffer->string_size + string_size;
    *(buffer->string + new_string_size) = '\0';
    buffer->string_size += string_size;
  } else {
    success = x_core_bool_false;
  }

  return success;
}

void x_core_buffer_clear(x_core_buffer_t *buffer)
{
  assert(buffer);

  if (buffer->string) {
    free(buffer->string);
  }
  buffer->string_size = 0;
  buffer->string_allocated_size = 0;
}

x_core_buffer_t *x_core_buffer_create(char *string, unsigned long string_size)
{
  assert(string);
  x_core_buffer_t *buffer;

  buffer = malloc(sizeof *buffer);
  if (buffer) {
    buffer->string_size = string_size;
    if (string_size > 0) {
      buffer->string = malloc(string_size + 1);
      if (buffer->string) {
        memcpy(buffer->string, string, string_size + 1);
        buffer->string_allocated_size = string_size + 1;
        buffer->grow_size = string_size * GROW_FACTOR;
      } else {
        x_core_trace("malloc");
      }
    } else {
      buffer->string = NULL;
      buffer->string_allocated_size = 0;
      buffer->grow_size = INITIAL_BUFFER_GROW_SIZE;
    }
  } else {
    x_core_trace("malloc");
  }

  return buffer;
}

void x_core_buffer_destroy(x_core_buffer_t *buffer)
{
  assert(buffer);

  if (buffer->string) {
    free(buffer->string);
  }
}

char *x_core_buffer_get_string(x_core_buffer_t *buffer,
    unsigned long *string_size)
{
  assert(buffer);
  assert(string_size);
  *string_size = buffer->string_size;
  return buffer->string;
}

char *x_core_buffer_take_string(x_core_buffer_t *buffer,
    unsigned long *string_size)
{
  assert(buffer);
  assert(string_size);
  char *string;

  string = buffer->string;
  *string_size = buffer->string_size;
  if (buffer->string) {
    buffer->string = NULL;
    buffer->string_size = 0;
    buffer->string_allocated_size = 0;
  }

  return string;
}
