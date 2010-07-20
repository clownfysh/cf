#ifndef x_core_message_h
#define x_core_message_h

#include "x/core/engine.h"
#include "x/core/object.h"
#include "x/core/types.h"

enum x_core_message_encoding_t {
  X_CORE_MESSAGE_ENCODING_UNKNOWN = 0,
  X_CORE_MESSAGE_ENCODING_PLAIN = 1,
};
typedef enum x_core_message_encoding_t x_core_message_encoding_t;

typedef x_core_bool_t x_core_message_add_to_message_f(void *object,
    x_core_message_t *message);

typedef void *x_core_message_create_from_message_f(x_core_message_t *message);

x_core_bool_t x_core_message_add_bool(x_core_message_t *message, void *object);

x_core_bool_t x_core_message_add_double(x_core_message_t *message,
    void *object);

x_core_bool_t x_core_message_add_long(x_core_message_t *message, void *object);

x_core_bool_t x_core_message_add_short(x_core_message_t *message,
    void *object);

x_core_bool_t x_core_message_add_string(x_core_message_t *message,
    void *object);

void *x_core_message_copy(void *message_object);

x_core_message_t *x_core_message_create(int client_socket,
    x_net_engine_id_t engine_id, unsigned long message_type, char *data,
    unsigned long data_size);

void x_core_message_destroy(void *message_object);

int x_core_message_get_client_socket(void *message_object);

char *x_core_message_get_data(x_core_message_t *message);

unsigned long x_core_message_get_data_get_size(x_core_message_t *message);

x_core_message_encoding_t x_core_message_get_encoding
(x_core_message_t *message);

x_net_engine_id_t x_core_message_get_engine_id(void *message_object);

unsigned long x_core_message_get_type(void *message_object);

void *x_core_message_take_bool(x_core_message_t *message);

x_core_bool_t x_core_message_take_bool_value(x_core_message_t *message);

void *x_core_message_take_double(x_core_message_t *message);

double x_core_message_take_double_value(x_core_message_t *message);

void *x_core_message_take_long(x_core_message_t *message);

long x_core_message_take_long_value(x_core_message_t *message);

void *x_core_message_take_short(x_core_message_t *message);

short x_core_message_take_short_value(x_core_message_t *message);

void *x_core_message_take_string(x_core_message_t *message);

#endif
