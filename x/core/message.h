#ifndef cf_x_core_message_h
#define cf_x_core_message_h

#include "cf/x/core/engine.h"
#include "cf/x/core/object.h"

enum cf_x_core_message_encoding_t {
  CF_X_CORE_MESSAGE_ENCODING_UNKNOWN = 0,
  CF_X_CORE_MESSAGE_ENCODING_PLAIN = 1,
};
typedef enum cf_x_core_message_encoding_t cf_x_core_message_encoding_t;

typedef cf_x_core_bool_t cf_x_core_message_add_to_message_f(void *object,
    cf_x_core_message_t *message);

typedef void *cf_x_core_message_create_from_message_f(cf_x_core_message_t *message);

cf_x_core_bool_t cf_x_core_message_add_bool(cf_x_core_message_t *message, void *object);

cf_x_core_bool_t cf_x_core_message_add_double(cf_x_core_message_t *message,
    void *object);

cf_x_core_bool_t cf_x_core_message_add_long(cf_x_core_message_t *message, void *object);

cf_x_core_bool_t cf_x_core_message_add_short(cf_x_core_message_t *message,
    void *object);

cf_x_core_bool_t cf_x_core_message_add_string(cf_x_core_message_t *message,
    void *object);

void *cf_x_core_message_copy(void *message_object);

cf_x_core_message_t *cf_x_core_message_create(int client_socket,
    cf_x_net_engine_id_t engine_id, unsigned long message_type, char *data,
    unsigned long data_size);

void cf_x_core_message_destroy(void *message_object);

int cf_x_core_message_get_client_socket(void *message_object);

char *cf_x_core_message_get_data(cf_x_core_message_t *message);

unsigned long cf_x_core_message_get_data_get_size(cf_x_core_message_t *message);

cf_x_core_message_encoding_t cf_x_core_message_get_encoding
(cf_x_core_message_t *message);

cf_x_net_engine_id_t cf_x_core_message_get_engine_id(void *message_object);

unsigned long cf_x_core_message_get_type(void *message_object);

void *cf_x_core_message_take_bool(cf_x_core_message_t *message);

cf_x_core_bool_t cf_x_core_message_take_bool_value(cf_x_core_message_t *message);

void *cf_x_core_message_take_double(cf_x_core_message_t *message);

double cf_x_core_message_take_double_value(cf_x_core_message_t *message);

void *cf_x_core_message_take_long(cf_x_core_message_t *message);

long cf_x_core_message_take_long_value(cf_x_core_message_t *message);

void *cf_x_core_message_take_short(cf_x_core_message_t *message);

short cf_x_core_message_take_short_value(cf_x_core_message_t *message);

void *cf_x_core_message_take_string(cf_x_core_message_t *message);

#endif
