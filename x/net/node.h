#ifndef x_net_node_h
#define x_net_node_h

#include "x/core/message.h"
#include "x/net/server.h"
#include "x/net/starclient.h"

struct x_net_node_t;
typedef struct x_net_node_t x_net_node_t;

x_net_node_t *x_net_node_create(char *node_ip, unsigned short node_min_port,
    unsigned short node_max_port, unsigned short node_max_threads,
    x_container_list_t *peer_node_ips, unsigned short peer_node_port_min,
    unsigned short peer_node_port_max, x_audit_log_t *log);

void x_net_node_destroy(x_net_node_t *node);

x_net_server_t *x_net_node_get_server(x_net_node_t *node);

void x_net_node_get_server_stats(x_net_node_t *node,
    x_net_server_stats_t *server_stats);

x_net_starclient_t *x_net_node_get_starclient(x_net_node_t *node);

x_core_bool_t x_net_node_register_engine(x_net_node_t *node,
    x_net_engine_id_t engine_id, void *custom_server_context,
    x_net_enginey_t *enginey, unsigned short min_run_threads,
    unsigned short max_run_threads, x_net_maintain_t maintain_schedule,
    unsigned long message_type_count);

void x_net_node_register_message_handler(x_net_node_t *node,
    x_net_engine_id_t engine_id, unsigned long message_type,
    x_net_server_handle_message_f message_handler);

x_core_bool_t x_net_node_send_message_to_all_peers(x_net_node_t *node,
    x_core_message_t *message);

x_core_bool_t x_net_node_send_message_to_any_peer(x_net_node_t *node,
    x_core_message_t *message);

x_core_bool_t x_net_node_send_message_to_client(x_net_node_t *node,
    x_core_message_t *message);

void x_net_node_set_server_unresponsive_client_time_seconds
(x_net_node_t *node, unsigned long unresponsive_client_time_seconds);

x_core_bool_t x_net_node_start(x_net_node_t *node);

#endif
