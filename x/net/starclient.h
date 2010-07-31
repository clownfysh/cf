#ifndef x_net_starclient_h
#define x_net_starclient_h

#include "x/audit/log.h"
#include "x/core/engine.h"
#include "x/net/client.h"
#include "x/net/starclient_stats.h"

struct x_net_starclient_t;
typedef struct x_net_starclient_t x_net_starclient_t;

x_core_bool_t x_net_starclient_connect(x_net_starclient_t *starclient);

x_net_starclient_t *x_net_starclient_create(x_case_list_t *star_arm_ips,
    unsigned short star_arm_port_min, unsigned short star_arm_port_max,
    char *node_server_exclude_ip, unsigned short node_server_exclude_min_port,
    unsigned short node_server_exclude_max_port, void *custom_client_context,
    x_audit_log_t *log);

void x_net_starclient_destroy(x_net_starclient_t *starclient);

x_net_client_t *x_net_starclient_get_client(x_net_starclient_t *starclient,
    int socket);

void x_net_starclient_get_stats(x_net_starclient_t *starclient,
    x_net_starclient_stats_t *starclient_stats);

void x_net_starclient_process_messages(x_net_starclient_t *starclient);

x_core_bool_t x_net_starclient_register_engine(x_net_starclient_t *starclient,
    x_net_engine_id_t engine_id, unsigned long message_type_count);

void x_net_starclient_register_message_handler(x_net_starclient_t *starclient,
    x_net_engine_id_t engine_id, unsigned long message_type,
    x_net_client_handle_message_f message_handler);

x_core_bool_t x_net_starclient_send_message_to_any_arm
(x_net_starclient_t *starclient, x_core_message_t *message);

x_core_bool_t x_net_starclient_send_message_to_all_arms
(x_net_starclient_t *starclient, x_core_message_t *message);

void x_net_starclient_set_unsent_messages_queue_size
(x_net_starclient_t *starclient, unsigned long queue_size);

x_core_bool_t x_net_starclient_star_available(x_net_starclient_t *starclient);

#endif
