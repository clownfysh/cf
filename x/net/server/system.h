#ifndef cf_x_net_server_system_h
#define cf_x_net_server_system_h

#include "cf/x/core/log.h"
#include "cf/x/config/system.h"
#include "cf/x/case/list.h"
#include "cf/x/core/engine.h"
#include "cf/x/core/message.h"
#include "cf/x/core/imessage.h"
#include "cf/x/core/types.h"
#include "cf/x/net/engine/enginey.h"
#include "cf/x/net/ip_address.h"
#include "cf/x/net/post/ipost.h"
#include "cf/x/net/server/stats.h"

#define CF_X_NET_SERVER_SYSTEM_MAX_ENGINES 64
#define CF_X_NET_SERVER_SYSTEM_NO_MAINTAIN_FUNCTION NULL
#define CF_X_NET_SERVER_SYSTEM_NO_CONFIG_SYSTEM NULL
#define CF_X_NET_SERVER_SYSTEM_DONT_CLOSE_UNRESPONSIVE_CLIENTS 0
#define CF_X_NET_MESSAGE_TYPE_COUNT_NONE 0

cf_x_net_server_system_t *cf_x_net_server_system_create(const char *name, unsigned short min_port,
    unsigned short max_port, unsigned short max_threads,
    cf_x_core_imessage_t *messagey, cf_x_net_post_ipost_t *postey,
    cf_x_net_engine_get_name_f get_engine_name, cf_x_config_system_t *config_system,
    cf_x_core_log_t *log);

void cf_x_net_server_system_destroy(cf_x_net_server_system_t *server);

cf_x_core_bool_t cf_x_net_server_system_engine_run_thread_quiesce_requested
(cf_x_net_server_system_t *server, cf_x_net_engine_id_t engine_id,
    unsigned short thread_index);

cf_x_net_server_system_handle_message_f cf_x_net_server_system_get_handler_for_message
(cf_x_net_server_system_t *server, void *message_object);

cf_x_config_system_t *cf_x_net_server_system_get_config_system(cf_x_net_server_system_t *server);

void cf_x_net_server_system_get_stats(cf_x_net_server_system_t *server,
    cf_x_net_server_stats_t *server_stats);

void cf_x_net_server_system_print_stats(cf_x_net_server_system_t *server);

void cf_x_net_server_system_process_messages(cf_x_net_server_system_t *server,
    cf_x_net_engine_id_t engine_id, unsigned short thread_index);

cf_x_core_bool_t cf_x_net_server_system_register_engine(cf_x_net_server_system_t *server,
    cf_x_net_engine_id_t engine_id, void *custom_server_context,
    cf_x_net_engine_enginey_t *enginey, unsigned short min_run_threads,
    unsigned short max_run_threads, cf_x_net_maintain_t maintain_schedule,
    unsigned long message_type_count);

void cf_x_net_server_system_register_message_handler(cf_x_net_server_system_t *server,
    cf_x_net_engine_id_t engine_id, unsigned long message_type,
    cf_x_net_server_system_handle_message_f message_handler);

cf_x_core_bool_t cf_x_net_server_system_send_message(cf_x_net_server_system_t *server,
    void *message_object);

void cf_x_net_server_system_set_unresponsive_client_time_seconds
(cf_x_net_server_system_t *server, unsigned long unresponsive_client_time_seconds);

cf_x_core_bool_t cf_x_net_server_system_start(cf_x_net_server_system_t *server);

cf_x_case_list_t *cf_x_net_server_system_take_unprocessed_messages
(cf_x_net_server_system_t *server);

cf_x_case_list_t *cf_x_net_server_system_take_unsent_messages(cf_x_net_server_system_t *server);

#endif
