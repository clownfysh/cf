#ifndef x_net_client_h
#define x_net_client_h

#include "x/audit/log.h"
#include "x/case/list.h"
#include "x/core/message.h"
#include "x/core/messagey.h"
#include "x/net/postey.h"

struct x_net_client_t;
typedef struct x_net_client_t x_net_client_t;

typedef void (*x_net_client_handle_message_f)(void *custom_client_context,
    x_core_message_t *message);

void *x_net_client_copy(void *client_object);

x_net_client_t *x_net_client_create(const char *server_ip_address,
    unsigned short server_min_port, unsigned short server_max_port,
    x_net_engine_get_name_f get_engine_name, void *custom_client_context,
    x_audit_log_t *log);

void x_net_client_destroy(void *client_object);

char *x_net_client_get_as_string(void *client_object);

char *x_net_client_get_server_ip_address(x_net_client_t *client);

unsigned short x_net_client_get_server_port(x_net_client_t *client);

int x_net_client_get_socket(x_net_client_t *client);

x_core_bool_t x_net_client_is_connected_to_server(x_net_client_t *client);

void x_net_client_print(void *client_object);

void x_net_client_process_messages(x_net_client_t *client);

x_core_bool_t x_net_client_register_engine(x_net_client_t *client,
    x_net_engine_id_t engine_id, unsigned long message_type_count);

void x_net_client_register_message_handler(x_net_client_t *client,
    x_net_engine_id_t engine_id, unsigned long message_type,
    x_net_client_handle_message_f message_handler);

x_core_bool_t x_net_client_send_message(x_net_client_t *client,
    x_core_message_t *message);

x_case_list_t *x_net_client_take_unsent_messages(x_net_client_t *client);

#endif
