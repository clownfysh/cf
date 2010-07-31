#ifndef cf_x_net_star_node_system_h
#define cf_x_net_star_node_system_h

#include "cf/x/core/message.h"
#include "cf/x/net/server/system.h"
#include "cf/x/net/star/client/system.h"

struct cf_x_net_star_node_system_t;
typedef struct cf_x_net_star_node_system_t cf_x_net_star_node_system_t;

cf_x_net_star_node_system_t *cf_x_net_star_node_system_create(char *node_ip, unsigned short node_min_port,
    unsigned short node_max_port, unsigned short node_max_threads,
    cf_x_case_list_t *peer_node_ips, unsigned short peer_node_port_min,
    unsigned short peer_node_port_max, cf_x_core_log_t *log);

void cf_x_net_star_node_system_destroy(cf_x_net_star_node_system_t *node);

cf_x_net_server_system_t *cf_x_net_star_node_system_get_server(cf_x_net_star_node_system_t *node);

void cf_x_net_star_node_system_get_server_stats(cf_x_net_star_node_system_t *node,
    cf_x_net_server_stats_t *server_stats);

cf_x_net_star_client_system_t *cf_x_net_star_node_system_get_starclient(cf_x_net_star_node_system_t *node);

cf_x_core_bool_t cf_x_net_star_node_system_register_engine(cf_x_net_star_node_system_t *node,
    cf_x_net_engine_id_t engine_id, void *custom_server_context,
    cf_x_net_engine_enginey_t *enginey, unsigned short min_run_threads,
    unsigned short max_run_threads, cf_x_net_maintain_t maintain_schedule,
    unsigned long message_type_count);

void cf_x_net_star_node_system_register_message_handler(cf_x_net_star_node_system_t *node,
    cf_x_net_engine_id_t engine_id, unsigned long message_type,
    cf_x_net_server_system_handle_message_f message_handler);

cf_x_core_bool_t cf_x_net_star_node_system_send_message_to_all_peers(cf_x_net_star_node_system_t *node,
    cf_x_core_message_t *message);

cf_x_core_bool_t cf_x_net_star_node_system_send_message_to_any_peer(cf_x_net_star_node_system_t *node,
    cf_x_core_message_t *message);

cf_x_core_bool_t cf_x_net_star_node_system_send_message_to_client(cf_x_net_star_node_system_t *node,
    cf_x_core_message_t *message);

void cf_x_net_star_node_system_set_server_unresponsive_client_time_seconds
(cf_x_net_star_node_system_t *node, unsigned long unresponsive_client_time_seconds);

cf_x_core_bool_t cf_x_net_star_node_system_start(cf_x_net_star_node_system_t *node);

#endif
