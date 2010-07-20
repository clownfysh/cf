#ifndef x_net_server_h
#define x_net_server_h

#include "x/audit/log.h"
#include "x/config/system.h"
#include "x/container/list.h"
#include "x/core/engine.h"
#include "x/core/message.h"
#include "x/core/messagey.h"
#include "x/core/types.h"
#include "x/net/enginey.h"
#include "x/net/ip_address.h"
#include "x/net/postey.h"
#include "x/net/server_stats.h"

#define X_NET_SERVER_MAX_ENGINES 64
#define X_NET_SERVER_NO_MAINTAIN_FUNCTION NULL
#define X_NET_SERVER_NO_CONFIG_SYSTEM NULL
#define X_NET_SERVER_DONT_CLOSE_UNRESPONSIVE_CLIENTS 0
#define X_NET_MESSAGE_TYPE_COUNT_NONE 0

x_net_server_t *x_net_server_create(const char *name, unsigned short min_port,
    unsigned short max_port, unsigned short max_threads,
    x_core_messagey_t *messagey, x_net_postey_t *postey,
    x_net_engine_get_name_f get_engine_name, x_config_system_t *config_system,
    x_audit_log_t *log);

void x_net_server_destroy(x_net_server_t *server);

x_core_bool_t x_net_server_engine_run_thread_quiesce_requested
(x_net_server_t *server, x_net_engine_id_t engine_id,
    unsigned short thread_index);

x_net_server_handle_message_f x_net_server_get_handler_for_message
(x_net_server_t *server, void *message_object);

x_config_system_t *x_net_server_get_config_system(x_net_server_t *server);

void x_net_server_get_stats(x_net_server_t *server,
    x_net_server_stats_t *server_stats);

void x_net_server_print_stats(x_net_server_t *server);

void x_net_server_process_messages(x_net_server_t *server,
    x_net_engine_id_t engine_id, unsigned short thread_index);

x_core_bool_t x_net_server_register_engine(x_net_server_t *server,
    x_net_engine_id_t engine_id, void *custom_server_context,
    x_net_enginey_t *enginey, unsigned short min_run_threads,
    unsigned short max_run_threads, x_net_maintain_t maintain_schedule,
    unsigned long message_type_count);

void x_net_server_register_message_handler(x_net_server_t *server,
    x_net_engine_id_t engine_id, unsigned long message_type,
    x_net_server_handle_message_f message_handler);

x_core_bool_t x_net_server_send_message(x_net_server_t *server,
    void *message_object);

void x_net_server_set_unresponsive_client_time_seconds
(x_net_server_t *server, unsigned long unresponsive_client_time_seconds);

x_core_bool_t x_net_server_start(x_net_server_t *server);

x_container_list_t *x_net_server_take_unprocessed_messages
(x_net_server_t *server);

x_container_list_t *x_net_server_take_unsent_messages(x_net_server_t *server);

#endif
