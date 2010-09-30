#include "cf/x/core/message.h"
#include "cf/x/core/string.h"
#include "cf/x/core/tools.h"

cf_x_core_bool_t cf_x_core_string_add_to_message(void *string_object,
    cf_x_core_message_t *message)
{
  return cf_x_core_message_add_string(message, string_object);
}

int cf_x_core_string_compare(void *string_object_a, void *string_object_b)
{
  const char *string_a;
  const char *string_b;
  int compare;

  string_a = string_object_a;
  string_b = string_object_b;
  compare = strcmp(string_a, string_b);

  return compare;
}

cf_x_core_bool_t cf_x_core_string_compare_equal(void *string_a_object,
    void *string_b_object)
{
  return (0 == cf_x_core_string_compare(string_a_object, string_b_object));
}

void *cf_x_core_string_copy(void *string_object)
{
  assert(string_object);
  /*  char *string;  */
  char *string_copy;

  /*  string = string_object;  */
  string_copy = strdup(string_object);
  if (!string_copy) {
    cf_x_core_trace("strdup");
  }

  return string_copy;
}

void *cf_x_core_string_create_from_message(cf_x_core_message_t *message)
{
  return cf_x_core_message_take_string(message);
}

void cf_x_core_string_destroy(void *string_object)
{
  assert(string_object);
  free(string_object);
}

char *cf_x_core_string_get_as_string(void *string_object)
{
  assert(string_object);
  /*  char *string;  */
  char *string_copy;

  /*  string = string_object;  */
  string_copy = strdup(string_object);
  if (!string_copy) {
    cf_x_core_trace("strdup");
  }

  return string_copy;
}

unsigned long cf_x_core_string_hash(void *string_object)
{
  return cf_x_core_hash(string_object);
}

void cf_x_core_string_init_iobject(cf_x_core_iobject_t *iobject)
{
  assert(iobject);
  cf_x_core_iobject_init(iobject, cf_x_core_string_compare,
      cf_x_core_string_compare_equal, cf_x_core_string_copy,
      cf_x_core_string_destroy, cf_x_core_string_get_as_string,
      cf_x_core_string_mod);
}

unsigned long cf_x_core_string_mod(void *string_object, unsigned long divisor)
{
  assert(string_object);
  char *string = string_object;
  unsigned long dividend = 0;
  unsigned char i;
  char c;
  unsigned long place_value = 1;

  for (i = 0; i < 4; i++) {
    c = *(string + i);
    if (c) {
      dividend += (c * place_value);
    } else {
      break;
    }
    place_value *= 256;
  }

  return dividend % divisor;
}

void cf_x_core_string_print(void *string_object)
{
  char *string;

  string = string_object;
  printf("%s", string);
}

cf_x_core_string_t cf_x_core_string_substring(cf_x_core_string_t string,
    unsigned long start, unsigned long length)
{
  assert(string);
  cf_x_core_string_t substring;
  unsigned long string_length;

  string_length = strlen(string);
  if ((start + length) > string_length) {
    length = string_length - start;
  }
  substring = malloc(length + 1);
  if (substring) {
    memcpy(substring, string + start, length);
    *(substring + length) = '\0';
  } else {
    cf_x_core_trace("malloc");
  }

  return substring;
}
