#include "x/core/engine.h"
#include "x/core/message.h"
#include "x/core/string.h"
#include "x/core/tools.h"

struct x_core_message_t {
  int client_socket;
  x_core_message_encoding_t encoding;
  x_net_engine_id_t engine_id;
  unsigned long message_type;
  unsigned long data_size;
  char *data;
  unsigned long deserialize_position;
};

x_core_bool_t x_core_message_add_bool(x_core_message_t *message,
    void *object)
{
  return x_core_message_add_short(message, object);
}

x_core_bool_t x_core_message_add_double(x_core_message_t *message,
    void *object)
{
  x_core_bool_t success;
  char *double_string;
  int chars_written;
  double *double_pointer;
  char *new_data;

  double_pointer = object;
  chars_written = asprintf(&double_string, "%f", *double_pointer);
  if (chars_written >= 0) {
    new_data = realloc(message->data, message->data_size + chars_written + 1);
    if (new_data) {
      success = x_core_bool_true;
      message->data = new_data;
      memcpy(message->data + message->data_size, double_string,
          chars_written);
      *(message->data + message->data_size + chars_written) = '\0';
      message->data_size += (chars_written + 1);
    } else {
      success = x_core_bool_false;
      x_core_trace("realloc");
    }
    free(double_string);
  } else {
    success = x_core_bool_false;
    x_core_trace("asprintf");
  }

  return success;
}

x_core_bool_t x_core_message_add_long(x_core_message_t *message,
    void *object)
{
  x_core_bool_t success;
  long *long_pointer;
  long long_object_network;
  char *new_data;

  long_pointer = object;
  long_object_network = htonl(*long_pointer);
  new_data = realloc(message->data, message->data_size
      + sizeof long_object_network);
  if (new_data) {
    message->data = new_data;
    memcpy(message->data + message->data_size, &long_object_network,
        sizeof long_object_network);
    message->data_size += sizeof long_object_network;
    success = x_core_bool_true;
  } else {
    success = x_core_bool_false;
  }

  return success;
}

x_core_bool_t x_core_message_add_short(x_core_message_t *message,
    void *object)
{
  x_core_bool_t success;
  short *short_pointer;
  short short_object_network;
  char *new_data;

  short_pointer = object;
  short_object_network = htons(*short_pointer);
  new_data = realloc(message->data, message->data_size
      + sizeof short_object_network);
  if (new_data) {
    message->data = new_data;
    memcpy(message->data + message->data_size, &short_object_network,
        sizeof short_object_network);
    message->data_size += sizeof short_object_network;
    success = x_core_bool_true;
  } else {
    success = x_core_bool_false;
  }

  return success;
}

x_core_bool_t x_core_message_add_string(x_core_message_t *message,
    void *object)
{
  x_core_bool_t success;
  char *new_data;
  size_t string_size_witx_terminator;

  string_size_witx_terminator = strlen(object) + 1;
  new_data = realloc(message->data, message->data_size
      + string_size_witx_terminator);
  if (new_data) {
    message->data = new_data;
    memcpy(message->data + message->data_size, object,
        string_size_witx_terminator);
    message->data_size += string_size_witx_terminator;
    success = x_core_bool_true;
  } else {
    success = x_core_bool_false;
    x_core_trace("realloc");
  }

  return success;
}

void *x_core_message_copy(void *message_object)
{
  assert(message_object);
  x_core_message_t *message;
  x_core_message_t *message_copy;

  message = message_object;

  message_copy = malloc(sizeof *message_copy);
  if (message_copy) {
    message_copy->client_socket = message->client_socket;
    message_copy->encoding = message->encoding;
    message_copy->engine_id = message->engine_id;
    message_copy->message_type = message->message_type;
    message_copy->data_size = message->data_size;
    message_copy->deserialize_position = message->deserialize_position;
    message_copy->data = malloc(message->data_size);
    if (message_copy->data) {
      memcpy(message_copy->data, message->data, message->data_size);
    } else {
      x_core_trace("malloc");
    }
  } else {
    x_core_trace("malloc");
  }

  return message_copy;
}

x_core_message_t *x_core_message_create(int client_socket,
    x_net_engine_id_t engine_id, unsigned long message_type, char *data,
    unsigned long data_size)
{
  x_core_message_t *message;

  message = malloc(sizeof *message);
  if (message) {
    message->client_socket = client_socket;
    message->encoding = X_CORE_MESSAGE_ENCODING_PLAIN;
    message->engine_id = engine_id;
    message->message_type = message_type;
    message->deserialize_position = 0;
    message->data_size = data_size;
    if (data_size != 0) {
      message->data = malloc(data_size);
      if (message->data) {
        memcpy(message->data, data, data_size);
      } else {
        free(message);
        message = NULL;
      }
    } else {
      message->data = NULL;
    }
  } else {
    x_core_trace("malloc");
  }

  return message;
}

void x_core_message_destroy(void *message_object)
{
  assert(message_object);
  x_core_message_t *message;

  message = message_object;

  if (message->data) {
    free(message->data);
  }
  free(message);

}

void x_core_message_free(void *message_object)
{
  x_core_message_t *message;

  message = message_object;
  x_core_message_destroy(message);
}

int x_core_message_get_client_socket(void *message_object)
{
  assert(message_object);
  x_core_message_t *message;

  message = message_object;

  return message->client_socket;
}

char *x_core_message_get_data(x_core_message_t *message)
{
  return message->data;
}

unsigned long x_core_message_get_data_get_size(x_core_message_t *message)
{
  return message->data_size;
}

x_core_message_encoding_t x_core_message_get_encoding
(x_core_message_t *message)
{
  return message->encoding;
}

x_net_engine_id_t x_core_message_get_engine_id(void *message_object)
{
  assert(message_object);
  x_core_message_t *message;

  message = message_object;

  return message->engine_id;
}

unsigned long x_core_message_get_type(void *message_object)
{
  assert(message_object);
  x_core_message_t *message;

  message = message_object;

  return message->message_type;
}

void *x_core_message_take_bool(x_core_message_t *message)
{
  return x_core_message_take_short(message);
}

x_core_bool_t x_core_message_take_bool_value(x_core_message_t *message)
{
  return x_core_message_take_short_value(message);
}

void *x_core_message_take_double(x_core_message_t *message)
{
  double *double_pointer;
  char *double_string;

  double_pointer = malloc(sizeof *double_pointer);
  if (double_pointer) {
    double_string = x_core_message_take_string(message);
    if (double_string) {
      *double_pointer = atof(double_string);
      free(double_string);
    } else {
      *double_pointer = 0.0;
      x_core_trace("x_core_message_take_string");
    }
  } else {
    x_core_trace("malloc");
  }

  return double_pointer;
}

double x_core_message_take_double_value(x_core_message_t *message)
{
  double double_object;
  char *double_string;

  double_string = x_core_message_take_string(message);
  if (double_string) {
    double_object = atof(double_string);
    free(double_string);
  } else {
    x_core_trace("x_core_message_take_string");
    double_object = 0.0;
  }

  return double_object;
}

void *x_core_message_take_long(x_core_message_t *message)
{
  long *long_pointer;
  long long_object_network;

  memcpy(&long_object_network, message->data + message->deserialize_position,
      sizeof long_object_network);
  message->deserialize_position += sizeof long_object_network;
  long_pointer = malloc(sizeof *long_pointer);
  if (long_pointer) {
    *long_pointer = ntohl(long_object_network);
  } else {
    x_core_trace("malloc");
  }

  return long_pointer;
}

long x_core_message_take_long_value(x_core_message_t *message)
{
  long long_object;
  long long_object_network;

  if ((message->deserialize_position + sizeof long_object_network)
      <= message->data_size) {
    memcpy(&long_object_network,
        message->data + message->deserialize_position,
        sizeof long_object_network);
    message->deserialize_position += sizeof long_object_network;
    long_object = ntohl(long_object_network);
  } else {
    x_core_trace("tried to read a long past the end of the message");
    long_object = 0;
  }

  return long_object;
}

void *x_core_message_take_short(x_core_message_t *message)
{
  short *short_pointer;
  short short_object_network;

  memcpy(&short_object_network, message->data + message->deserialize_position,
      sizeof short_object_network);
  message->deserialize_position += sizeof short_object_network;
  short_pointer = malloc(sizeof *short_pointer);
  if (short_pointer) {
    *short_pointer = ntohs(short_object_network);
  } else {
    x_core_trace("malloc");
  }

  return short_pointer;
}

short x_core_message_take_short_value(x_core_message_t *message)
{
  short short_object;
  short short_object_network;

  if ((message->deserialize_position + sizeof short_object_network)
      <= message->data_size) {
    memcpy(&short_object_network,
        message->data + message->deserialize_position,
        sizeof short_object_network);
    message->deserialize_position += sizeof short_object_network;
    short_object = ntohs(short_object_network);
  } else {
    x_core_trace("tried to read a short past the end of the message");
    short_object = 0;
  }

  return short_object;
}

void *x_core_message_take_string(x_core_message_t *message)
{
  char *string;
  size_t string_size_witx_terminator;

  string_size_witx_terminator
    = strlen(message->data + message->deserialize_position) + 1;
  string = malloc(string_size_witx_terminator);
  if (string) {
    memcpy(string, message->data + message->deserialize_position,
        string_size_witx_terminator);
    message->deserialize_position += string_size_witx_terminator;
  } else {
    x_core_trace("malloc");
  }

  return string;
}
