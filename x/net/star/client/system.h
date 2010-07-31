#ifndef cf_x_net_starclient_h
#define cf_x_net_starclient_h

#include "cf/x/core/log.h"
#include "cf/x/core/engine.h"
#include "cf/x/net/client/system.h"
#include "cf/x/net/star/client/stats.h"

struct cf_x_net_starclient_t;
typedef struct cf_x_net_starclient_t cf_x_net_starclient_t;

cf_x_core_bool_t cf_x_net_starclient_connect(cf_x_net_starclient_t *starclient);

cf_x_net_starclient_t *cf_x_net_starclient_create(cf_x_case_list_t *star_arm_ips,
    unsigned short star_arm_port_min, unsigned short star_arm_port_max,
    char *node_server_exclude_ip, unsigned short node_server_exclude_min_port,
    unsigned short node_server_exclude_max_port, void *custom_client_context,
    cf_x_audit_log_t *log);

void cf_x_net_starclient_destroy(cf_x_net_starclient_t *starclient);

cf_x_net_client_system_t *cf_x_net_starclient_get_client(cf_x_net_starclient_t *starclient,
    int socket);

void cf_x_net_starclient_get_stats(cf_x_net_starclient_t *starclient,
    cf_x_net_starclient_stats_t *starclient_stats);

void cf_x_net_starclient_process_messages(cf_x_net_starclient_t *starclient);

cf_x_core_bool_t cf_x_net_starclient_register_engine(cf_x_net_starclient_t *starclient,
    cf_x_net_engine_id_t engine_id, unsigned long message_type_count);

void cf_x_net_starclient_register_message_handler(cf_x_net_starclient_t *starclient,
    cf_x_net_engine_id_t engine_id, unsigned long message_type,
    cf_x_net_client_system_handle_message_f message_handler);

cf_x_core_bool_t cf_x_net_starclient_send_message_to_any_arm
(cf_x_net_starclient_t *starclient, cf_x_core_message_t *message);

cf_x_core_bool_t cf_x_net_starclient_send_message_to_all_arms
(cf_x_net_starclient_t *starclient, cf_x_core_message_t *message);

void cf_x_net_starclient_set_unsent_messages_queue_size
(cf_x_net_starclient_t *starclient, unsigned long queue_size);

cf_x_core_bool_t cf_x_net_starclient_star_available(cf_x_net_starclient_t *starclient);

#endif
