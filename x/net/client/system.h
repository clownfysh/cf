#ifndef cf_x_net_client_system_h
#define cf_x_net_client_system_h

#include "cf/x/core/log.h"
#include "cf/x/case/list.h"
#include "cf/x/core/message.h"
#include "cf/x/core/imessage.h"
#include "cf/x/net/post/ipost.h"

struct cf_x_net_client_system_t;
typedef struct cf_x_net_client_system_t cf_x_net_client_system_t;

typedef void (*cf_x_net_client_system_handle_message_f)(void *custom_client_context,
    cf_x_core_message_t *message);

void *cf_x_net_client_system_copy(void *client_object);

cf_x_net_client_system_t *cf_x_net_client_system_create(const char *server_ip_address,
    unsigned short server_min_port, unsigned short server_max_port,
    cf_x_net_engine_get_name_f get_engine_name, void *custom_client_context,
    cf_x_core_log_t *log);

void cf_x_net_client_system_destroy(void *client_object);

char *cf_x_net_client_system_get_as_string(void *client_object);

char *cf_x_net_client_system_get_server_ip_address(cf_x_net_client_system_t *client);

unsigned short cf_x_net_client_system_get_server_port(cf_x_net_client_system_t *client);

int cf_x_net_client_system_get_socket(cf_x_net_client_system_t *client);

cf_x_core_bool_t cf_x_net_client_system_is_connected_to_server(cf_x_net_client_system_t *client);

void cf_x_net_client_system_print(void *client_object);

void cf_x_net_client_system_process_messages(cf_x_net_client_system_t *client);

cf_x_core_bool_t cf_x_net_client_system_register_engine(cf_x_net_client_system_t *client,
    cf_x_net_engine_id_t engine_id, unsigned long message_type_count);

void cf_x_net_client_system_register_message_handler(cf_x_net_client_system_t *client,
    cf_x_net_engine_id_t engine_id, unsigned long message_type,
    cf_x_net_client_system_handle_message_f message_handler);

cf_x_core_bool_t cf_x_net_client_system_send_message(cf_x_net_client_system_t *client,
    cf_x_core_message_t *message);

cf_x_case_list_t *cf_x_net_client_system_take_unsent_messages(cf_x_net_client_system_t *client);

#endif
