#include "cf/x/case/list.h"
#include "cf/x/case/set.h"
#include "cf/x/core/basic/short.h"
#include "cf/x/core/constants.h"
#include "cf/x/core/engine.h"
#include "cf/x/core/message.h"
#include "cf/x/sync/period.h"
#include "cf/x/core/time.h"
#include "cf/x/core/tools.h"
#include "cf/x/net/client/socket.h"
#include "cf/x/net/engine/thread.h"
#include "cf/x/net/engine/enginey.h"
#include "cf/x/net/exchange.h"
#include "cf/x/net/post/system.h"
#include "cf/x/net/server/system.h"
#include "cf/x/net/server/socket.h"

#define DEFAULT_UNRESPONSIVE_CLIENT_TIME_SECONDS 32
#define ENGINE_INITIAL_SLEEP_MICROSECONDS CF_X_CORE_STANDARD_SLEEP_MICROSECONDS
#define CF_X_NET_SERVER_SYSTEM_SLEEP_MICROSECONDS CF_X_CORE_STANDARD_SLEEP_MICROSECONDS
#define MAX_THREADS_PER_ENGINE 8
#define PERFORMANCE_PERIOD_SECONDS 4

struct engine_container_t {
  cf_x_net_engine_id_t engine_id;
  cf_x_net_engine_enginey_t *enginey;

  void *engine_object;
  cf_x_core_bool_t stop_requested;
  unsigned long sleep;

  cf_x_case_list_t *inbox;

  pthread_t threads[MAX_THREADS_PER_ENGINE];
  cf_x_core_bool_t run_threads_quiesce_requested[MAX_THREADS_PER_ENGINE];
  cf_x_core_bool_t run_threads_quiesce_completed[MAX_THREADS_PER_ENGINE];
  cf_x_case_list_t *thread_inboxes[MAX_THREADS_PER_ENGINE];
  pthread_mutex_t thread_inbox_mutexes[MAX_THREADS_PER_ENGINE];

  unsigned short min_run_threads;
  unsigned short max_run_threads;
  unsigned short run_thread_count;

  pthread_t maintain_thread;
  cf_x_net_maintain_t maintain_schedule;

  cf_x_net_server_system_handle_message_f *message_handlers;
  unsigned long message_handlers_size;

  double messages_per_second;
  double messages_per_second_thread[MAX_THREADS_PER_ENGINE];
  cf_x_sync_period_t *performance_period;
  unsigned long messages_per_period;
  cf_x_core_bool_t reported_idle;
  unsigned long messages_per_period_thread[MAX_THREADS_PER_ENGINE];
};
typedef struct engine_container_t engine_container_t;

struct cf_x_net_server_system_t {
  char *name;
  int socket;
  unsigned short min_port;
  unsigned short max_port;

  unsigned short max_threads;
  unsigned short thread_count;

  cf_x_net_server_stats_t stats;

  pthread_t server_thread;
  pthread_t accept_thread;
  cf_x_core_bool_t accept_thread_created;

  cf_x_case_list_t *outbox;
  pthread_mutex_t outbox_mutex;

  cf_x_case_set_t *client_posts;
  cf_x_net_exchange_t *client_exchange;
  pthread_mutex_t client_posts_mutex;

  cf_x_case_list_t *engines;
  engine_container_t *engines_array[CF_X_NET_SERVER_SYSTEM_MAX_ENGINES];

  void *custom_server_object;

  unsigned long unresponsive_client_time_seconds;

  cf_x_core_messagey_t *messagey;
  cf_x_net_post_postey_t *postey;
  cf_x_net_engine_get_name_f get_engine_name;

  time_t create_time;

  cf_x_audit_log_t *log;

  cf_x_config_system_t *config_system;

  cf_x_core_objectey_t client_posts_objectey;
};

struct running_engine_t {
  cf_x_net_server_system_t *server;
  void *engine_object;
  cf_x_net_engine_id_t engine_id;

  cf_x_net_engine_enginey_start_f start;
  cf_x_net_engine_enginey_run_f run;
  cf_x_net_engine_enginey_stop_f stop;

  unsigned short thread_index;
};
typedef struct running_engine_t running_engine_t;

struct maintaining_engine_t {
  cf_x_net_server_system_t *server;
  void *engine_object;
  cf_x_net_engine_id_t engine_id;

  cf_x_net_engine_enginey_maintain_f maintain;
};
typedef struct maintaining_engine_t maintaining_engine_t;

static void *accept_thread(void *server_object);

static void close_disconnected_clients(cf_x_net_server_system_t *server);

static void close_unresponsive_clients(cf_x_net_server_system_t *server);

static engine_container_t *create_engine_container(cf_x_net_server_system_t *server,
    cf_x_net_engine_id_t engine_id, void *custom_server_object,
    cf_x_net_engine_enginey_t *enginey, unsigned short min_run_threads,
    unsigned short max_run_threads, cf_x_net_maintain_t maintain_schedule,
    unsigned long message_type_count);

static cf_x_core_bool_t create_engine_container_message_handlers
(engine_container_t *engine_container, unsigned long message_type_count,
    cf_x_audit_log_t *log);

static cf_x_core_bool_t create_engine_container_performance_period
(engine_container_t *engine_container, cf_x_audit_log_t *log);

static void create_engine_container_rollback
(engine_container_t *engine_container);

static void create_engine_container_stats
(engine_container_t *engine_container);

static cf_x_core_bool_t create_engine_container_threads
(engine_container_t *engine_container, cf_x_core_messagey_t *messagey,
    cf_x_audit_log_t *log);

static maintaining_engine_t *create_maintaining_engine(cf_x_net_server_system_t *server,
    void *engine_object, cf_x_net_engine_id_t engine_id,
    cf_x_net_engine_enginey_maintain_f maintain);

static cf_x_core_bool_t create_post_for_new_client(cf_x_net_server_system_t *server,
    int client_socket);

static running_engine_t *create_running_engine(cf_x_net_server_system_t *server,
    void *engine_object, cf_x_net_engine_id_t engine_id,
    cf_x_net_engine_enginey_start_f start, cf_x_net_engine_enginey_run_f run,
    cf_x_net_engine_enginey_stop_f stop, unsigned short thread_index);

static cf_x_core_bool_t create_thread(cf_x_net_server_system_t *server,
    pthread_t *pthread_address, thread_f thread_function, void *thread);

static void deliver_messages_to_engine(cf_x_net_server_system_t *server,
    engine_container_t *engine_container);

static unsigned long deliver_messages_to_engine_thread(cf_x_net_server_system_t *server,
    engine_container_t *engine_container, unsigned short thread_index,
    unsigned long target_number_of_messages_to_deliver);

static void deliver_messages_to_engines(cf_x_net_server_system_t *server);

static void destroy_engine_container(void *engine_container_object);

static void destroy_maintaining_engine
(maintaining_engine_t *maintaining_engine);

static void destroy_running_engine(running_engine_t *running_engine);

static unsigned long engine_count(cf_x_net_server_system_t *server);

static cf_x_core_bool_t engine_is_registered(cf_x_net_server_system_t *server,
    cf_x_net_engine_id_t engine_id);

static void engine_sleep(cf_x_net_server_system_t *server, cf_x_net_engine_id_t engine_id);

static void *find_client_post(cf_x_net_server_system_t *server,
    int client_socket);

static void handle_signal(int signal);

static cf_x_core_bool_t cf_x_net_server_system_create_client_exchange
(cf_x_net_server_system_t *server);

static cf_x_core_bool_t cf_x_net_server_system_create_client_posts(cf_x_net_server_system_t *server);

static cf_x_core_bool_t cf_x_net_server_system_create_engines(cf_x_net_server_system_t *server);

static cf_x_core_bool_t cf_x_net_server_system_create_outbox(cf_x_net_server_system_t *server);

static void cf_x_net_server_system_create_rollback(cf_x_net_server_system_t *server);

static cf_x_core_bool_t cf_x_net_server_system_register_engine_create
(engine_container_t *engine_container, cf_x_net_engine_enginey_create_f create,
    cf_x_net_server_system_t *server, void *custom_server_object);

static cf_x_core_bool_t join_thread(cf_x_net_server_system_t *server, pthread_t thread);

static void *maintain_engine(void *maintaining_engine_object);

static void manage_threads(cf_x_net_server_system_t *server);

static void post_messages_to_clients(cf_x_net_server_system_t *server);

static void receive_messages_from_client_post(cf_x_net_server_system_t *server,
    void *post_object);

static void receive_messages_from_clients(cf_x_net_server_system_t *server);

static void *run_engine(void *running_engine_object);

static cf_x_core_bool_t serversocket_bind_listen(cf_x_net_server_system_t *server);

static void *server_thread(void *server_object);

static void stop(cf_x_net_server_system_t *server);

static cf_x_core_bool_t start(cf_x_net_server_system_t *server);

static cf_x_core_bool_t start_engine(cf_x_net_server_system_t *server,
    cf_x_net_engine_id_t engine_id);

static cf_x_core_bool_t start_engine_maintain_thread(cf_x_net_server_system_t *server,
    cf_x_net_engine_id_t engine_id, engine_container_t *engine_container);

static cf_x_core_bool_t start_engine_run_thread(cf_x_net_server_system_t *server,
    cf_x_net_engine_id_t engine_id, engine_container_t *engine_container,
    unsigned short thread_index);

static cf_x_core_bool_t threads_maxed_out(cf_x_net_server_system_t *server);

void *accept_thread(void *server_object)
{
  cf_x_net_server_system_t *server;
  struct sockaddr_in client_address;
  socklen_t client_address_size;
  int client_socket;

  server = server_object;

  while (!cf_x_core_stop_requested) {
    client_socket = cf_x_net_server_socket_accept
      (server->socket, &client_address, &client_address_size);
    if (client_socket >= 0) {
      if (!create_post_for_new_client(server, client_socket)) {
        cf_x_audit_log_trace(server->log, "hnet", "x_net_serversocket_accept");
      }
    }
    usleep(CF_X_NET_SERVER_SYSTEM_SLEEP_MICROSECONDS);
  }

  return NULL;
}

void close_disconnected_clients(cf_x_net_server_system_t *server)
{
  assert(cf_x_case_set_get_size(server->client_posts)
      == cf_x_net_exchange_get_post_count(server->client_exchange));

  void *client_post_object;
  cf_x_case_set_t *client_posts;
  int client_socket;
  cf_x_core_bool_t success;

  client_posts = server->client_posts;

  cf_x_case_set_iterate_start(client_posts);
  while ((client_post_object = cf_x_case_set_iterate_next(client_posts))) {
    if (server->postey->socket_closed(client_post_object)) {
      client_socket = server->postey->get_socket(client_post_object);
      cf_x_case_set_iterate_remove(client_posts);
      success = cf_x_core_bool_true;
      if (!cf_x_net_exchange_unregister_post
          (server->client_exchange, client_socket)) {
        cf_x_audit_log_trace(server->log, "hnet",
            "x_net_exchange_unregister_post");
        success = cf_x_core_bool_false;
      }
      cf_x_net_client_socket_destroy(client_socket);
    }
  }

  assert(cf_x_case_set_get_size(server->client_posts)
      == cf_x_net_exchange_get_post_count(server->client_exchange));
}

void close_unresponsive_clients(cf_x_net_server_system_t *server)
{
  assert(cf_x_case_set_get_size(server->client_posts)
      == cf_x_net_exchange_get_post_count(server->client_exchange));
  void *client_post_object;
  cf_x_case_set_t *client_posts;
  int client_socket;
  time_t current_time;
  time_t post_activity_time;

  client_posts = server->client_posts;
  current_time = time(NULL);

  cf_x_case_set_iterate_start(client_posts);
  while ((client_post_object = cf_x_case_set_iterate_next(client_posts))) {
    post_activity_time = server->postey->get_last_receive_activity_time
      (client_post_object);
    if (current_time - post_activity_time
        > server->unresponsive_client_time_seconds) {
      cf_x_case_set_iterate_remove(client_posts);
      client_socket = server->postey->get_socket(client_post_object);
      if (!cf_x_net_exchange_unregister_post(server->client_exchange,
              client_socket)) {
        cf_x_audit_log_trace(server->log, "hnet",
            "x_net_exchange_unregister_post");
      }
      cf_x_net_client_socket_destroy(client_socket);
      cf_x_audit_log_enter(server->log, "hnet",
          "server closed unresponsive client %u", client_socket);
    }
  }
}

engine_container_t *create_engine_container(cf_x_net_server_system_t *server,
    cf_x_net_engine_id_t engine_id, void *custom_server_object,
    cf_x_net_engine_enginey_t *enginey, unsigned short min_run_threads,
    unsigned short max_run_threads, cf_x_net_maintain_t maintain_schedule,
    unsigned long message_type_count)
{
  assert(server);
  engine_container_t *engine_container;
  cf_x_core_bool_t so_far_so_good;

  engine_container = malloc(sizeof *engine_container);
  if (engine_container) {
    engine_container->engine_id = engine_id;
    engine_container->enginey = enginey;
    engine_container->stop_requested = cf_x_core_bool_false;
    engine_container->sleep = ENGINE_INITIAL_SLEEP_MICROSECONDS;
    engine_container->maintain_schedule = maintain_schedule;
    engine_container->min_run_threads = min_run_threads;
    engine_container->max_run_threads = max_run_threads;
    engine_container->run_thread_count = 0;
    create_engine_container_stats(engine_container);
    so_far_so_good = cf_x_core_bool_true;
  } else {
    so_far_so_good = cf_x_core_bool_false;
  }

  if (so_far_so_good) {
    engine_container->inbox = cf_x_case_list_create
      (CF_X_CORE_NO_COMPARE_FUNCTION, CF_X_CORE_NO_COPY_FUNCTION,
          CF_X_CORE_NO_DESTROY_FUNCTION);
    if (!engine_container->inbox) {
      cf_x_audit_log_trace(server->log, "hnet", "x_case_list_create");
      so_far_so_good = cf_x_core_bool_false;
    }
  }

  if (so_far_so_good) {
    so_far_so_good = create_engine_container_threads(engine_container,
        server->messagey,
        server->log);
  }

  if (so_far_so_good) {
    so_far_so_good = create_engine_container_message_handlers
      (engine_container, message_type_count, server->log);
  }

  if (so_far_so_good) {
    so_far_so_good = create_engine_container_performance_period
      (engine_container, server->log);
  }

  if (!so_far_so_good) {
    create_engine_container_rollback(engine_container);
    engine_container = NULL;
  }

  return engine_container;
}

cf_x_core_bool_t create_engine_container_message_handlers
(engine_container_t *engine_container, unsigned long message_type_count,
    cf_x_audit_log_t *log)
{
  assert(engine_container);
  assert(log);
  unsigned long each_handler;
  cf_x_core_bool_t success;

  engine_container->message_handlers_size = message_type_count;
  engine_container->message_handlers
    = malloc((sizeof *engine_container->message_handlers)
        * message_type_count);
  if (engine_container->message_handlers) {
    for (each_handler = 0; each_handler < message_type_count; each_handler++) {
      *(engine_container->message_handlers + each_handler) = NULL;
    }
    success = cf_x_core_bool_true;
  } else {
    success = cf_x_core_bool_false;
    cf_x_audit_log_trace(log, "hnet", "malloc");
  }

  return success;
}

cf_x_core_bool_t create_engine_container_performance_period
(engine_container_t *engine_container, cf_x_audit_log_t *log)
{
  assert(engine_container);
  assert(log);
  cf_x_core_bool_t success;

  engine_container->performance_period
    = cf_x_sync_period_create(PERFORMANCE_PERIOD_SECONDS);
  if (engine_container->performance_period) {
    success = cf_x_core_bool_true;
  } else {
    cf_x_audit_log_trace(log, "hnet", "new");
    success = cf_x_core_bool_false;
  }

  return success;
}

void create_engine_container_rollback(engine_container_t *engine_container)
{
  assert(engine_container);
  unsigned short each_thread;

  if (engine_container->engine_object) {
    engine_container->enginey->destroy(engine_container->engine_object);
  }
  if (engine_container->inbox) {
    cf_x_case_list_destroy(engine_container->inbox);
  }
  for (each_thread = 0; each_thread < MAX_THREADS_PER_ENGINE; each_thread++) {
    if (engine_container->thread_inboxes[each_thread]) {
      cf_x_case_list_destroy(engine_container->thread_inboxes[each_thread]);
    }
  }
  if (engine_container->message_handlers) {
    free(engine_container->message_handlers);
  }
  if (engine_container->performance_period) {
    cf_x_sync_period_destroy(engine_container->performance_period);
  }
  free(engine_container);
}

void create_engine_container_stats(engine_container_t *engine_container)
{
  assert(engine_container);
  unsigned short each_thread;

  engine_container->messages_per_second = 0.0;
  engine_container->messages_per_period = 0;
  engine_container->reported_idle = cf_x_core_bool_false;
  for (each_thread = 0; each_thread < MAX_THREADS_PER_ENGINE; each_thread++) {
    *(engine_container->messages_per_second_thread + each_thread) = 0.0;
    *(engine_container->messages_per_period_thread + each_thread) = 0;
  }
}

cf_x_core_bool_t create_engine_container_threads
(engine_container_t *engine_container, cf_x_core_messagey_t *messagey,
    cf_x_audit_log_t *log)
{
  assert(engine_container);
  assert(messagey);
  assert(log);
  unsigned short each_thread;
  pthread_mutex_t *each_mutex;
  cf_x_core_bool_t success;

  success = cf_x_core_bool_true;

  for (each_thread = 0; each_thread < MAX_THREADS_PER_ENGINE; each_thread++) {

    *(engine_container->run_threads_quiesce_requested
        + each_thread) = cf_x_core_bool_false;
    *(engine_container->run_threads_quiesce_completed
        + each_thread) = cf_x_core_bool_false;

    *(engine_container->thread_inboxes + each_thread)
      = cf_x_case_list_create(CF_X_CORE_NO_COMPARE_FUNCTION,
          CF_X_CORE_NO_COPY_FUNCTION, messagey->destroy);
    if (!*(engine_container->thread_inboxes + each_thread)) {
      success = cf_x_core_bool_false;
      cf_x_audit_log_trace(log, "hnet", "x_case_list_create");
      break;
    }

    each_mutex = &engine_container->thread_inbox_mutexes[each_thread];
    if (0 != pthread_mutex_init(each_mutex, NULL)) {
      success = cf_x_core_bool_false;
      cf_x_audit_log_trace(log, "hnet", "pthread_mutex_init");
      break;
    }
  }

  return success;
}

maintaining_engine_t *create_maintaining_engine(cf_x_net_server_system_t *server,
    void *engine_object, cf_x_net_engine_id_t engine_id,
    cf_x_net_engine_enginey_maintain_f maintain)
{
  maintaining_engine_t *maintaining_engine;

  maintaining_engine = malloc(sizeof *maintaining_engine);
  if (maintaining_engine) {
    maintaining_engine->server = server;
    maintaining_engine->engine_object = engine_object;
    maintaining_engine->engine_id = engine_id;
    maintaining_engine->maintain = maintain;
  } else {
    cf_x_audit_log_trace(server->log, "hnet", "malloc");
  }

  return maintaining_engine;
}

cf_x_core_bool_t create_post_for_new_client(cf_x_net_server_system_t *server,
    int client_socket)
{
  assert(server);
  cf_x_core_bool_t success;
  void *post_object;

  post_object = server->postey->create(client_socket);

  if (post_object) {

    pthread_mutex_lock(&server->client_posts_mutex);
    {
      if (cf_x_case_set_add(server->client_posts, post_object)) {
        success = cf_x_core_bool_true;
        if (!cf_x_net_exchange_register_post
            (server->client_exchange, post_object)) {
          cf_x_audit_log_trace(server->log, "hnet",
              "x_net_exchange_register_post");
          success = cf_x_core_bool_false;
        }
      } else {
        cf_x_audit_log_trace(server->log, "hnet", "x_case_set_add");
        success = cf_x_core_bool_false;
      }
    }
    pthread_mutex_unlock(&server->client_posts_mutex);

    if (!success) {
      server->postey->destroy(post_object);
    }

  } else {
    cf_x_audit_log_trace(server->log, "hnet", "post_create");
    success = cf_x_core_bool_false;
  }

  return success;
}

running_engine_t *create_running_engine(cf_x_net_server_system_t *server,
    void *engine_object, cf_x_net_engine_id_t engine_id,
    cf_x_net_engine_enginey_start_f start, cf_x_net_engine_enginey_run_f run,
    cf_x_net_engine_enginey_stop_f stop, unsigned short thread_index)
{
  running_engine_t *running_engine;

  running_engine = malloc(sizeof *running_engine);
  if (running_engine) {
    running_engine->server = server;
    running_engine->engine_object = engine_object;
    running_engine->engine_id = engine_id;
    running_engine->start = start;
    running_engine->run = run;
    running_engine->stop = stop;
    running_engine->thread_index = thread_index;
  }

  return running_engine;
}

cf_x_core_bool_t create_thread(cf_x_net_server_system_t *server, pthread_t *pthread_address,
    thread_f thread_function, void *thread)
{
  cf_x_core_bool_t success;

  success = cf_x_core_bool_false;

  if (!threads_maxed_out(server)) {
    if (0 == pthread_create(pthread_address, NULL, thread_function, thread)) {
      success = cf_x_core_bool_true;
      server->thread_count++;
    } else {
      cf_x_audit_log_trace(server->log, "hnet", "pthread_create");
    }
  } else {
    cf_x_audit_log_enter(server->log, "hnet", "threads are maxed out");
  }

  return success;
}

/*
  TODO: compartmentalize the reporting part of this function
*/
void deliver_messages_to_engine(cf_x_net_server_system_t *server,
    engine_container_t *engine_container)
{
  assert(server);
  assert(engine_container);
  unsigned short each_thread;
  unsigned long inbox_size;
  unsigned long target_number_of_messages_to_deliver;
  unsigned long actual_messages_delivered;
  unsigned long actual_messages_delivered_thread;
  double messages_per_second_this_period;
  double messages_per_second_this_period_on_this_thread;
  char *string;
  char little_string[5 + 1];

  actual_messages_delivered = 0;

  if (cf_x_sync_period_once(engine_container->performance_period)) {

    messages_per_second_this_period = engine_container->messages_per_period
      / ((double) PERFORMANCE_PERIOD_SECONDS);

    if (messages_per_second_this_period > 0.0) {
      engine_container->reported_idle = cf_x_core_bool_false;

      string = NULL;
      for (each_thread = 0; each_thread < engine_container->run_thread_count;
           each_thread++) {
        messages_per_second_this_period_on_this_thread
          = engine_container->messages_per_period_thread
          [each_thread] / ((double) PERFORMANCE_PERIOD_SECONDS);
        snprintf(little_string, 5 + 1, " %0.1f ",
            messages_per_second_this_period_on_this_thread);
        string = cf_x_core_string_append(string, little_string);
        if (!string) {
          cf_x_audit_log_trace(server->log, "hnet", "x_core_string_append");
        }
      }
      cf_x_audit_log_enter(server->log, "hnet", "%0.1f mps (%s) %s engine",
          messages_per_second_this_period, string,
          cf_x_net_engine_get_name(engine_container->engine_id,
              server->get_engine_name));
      free(string);

    } else if (!engine_container->reported_idle) {
      cf_x_audit_log_enter(server->log, "hnet", "%s engine is idle",
          cf_x_net_engine_get_name(engine_container->engine_id,
              server->get_engine_name));
      engine_container->reported_idle = cf_x_core_bool_true;

    }

    engine_container->messages_per_period = 0;
    for (each_thread = 0; each_thread < engine_container->run_thread_count;
         each_thread++) {
      *(engine_container->messages_per_period_thread + each_thread) = 0;
    }
  }

  if (engine_container->run_thread_count > 0) {
    inbox_size = cf_x_case_list_get_size(engine_container->inbox);
    target_number_of_messages_to_deliver
      = inbox_size / engine_container->run_thread_count;
    if (0 == target_number_of_messages_to_deliver) {
      target_number_of_messages_to_deliver = 1;
    }

    for (each_thread = 0; each_thread < engine_container->run_thread_count;
         each_thread++) {
      actual_messages_delivered_thread = deliver_messages_to_engine_thread
        (server, engine_container, each_thread,
            target_number_of_messages_to_deliver);
      actual_messages_delivered += actual_messages_delivered_thread;
      engine_container->messages_per_period_thread[each_thread]
        += actual_messages_delivered_thread;
    }
    engine_container->messages_per_period += actual_messages_delivered;
  }
}

unsigned long deliver_messages_to_engine_thread(cf_x_net_server_system_t *server,
    engine_container_t *engine_container, unsigned short thread_index,
    unsigned long target_number_of_messages_to_deliver)
{
  assert(server);
  assert(engine_container);
  unsigned long messages_delivered;
  unsigned long each_message;
  cf_x_case_list_t *thread_inbox;
  pthread_mutex_t *thread_inbox_mutex;
  void *message;

  messages_delivered = 0;
  thread_inbox_mutex = &engine_container->thread_inbox_mutexes[thread_index];
  thread_inbox = engine_container->thread_inboxes[thread_index];

  if (0 == pthread_mutex_trylock(thread_inbox_mutex)) {
    for (each_message = 0; each_message < target_number_of_messages_to_deliver;
         each_message++) {
      if ((message = cf_x_case_list_find_first(engine_container->inbox))) {
        if (cf_x_case_list_add_last(thread_inbox, message)) {
          cf_x_case_list_remove_first(engine_container->inbox);
          messages_delivered++;
        } else {
          cf_x_audit_log_trace(server->log, "hnet", "list_add_last");
        }
      }
    }
    pthread_mutex_unlock(thread_inbox_mutex);
  }

  return messages_delivered;
}

void deliver_messages_to_engines(cf_x_net_server_system_t *server)
{
  assert(server);
  engine_container_t *engine_container;

  cf_x_case_list_iterate_start(server->engines);
  while ((engine_container = cf_x_case_list_iterate_next(server->engines))) {
    deliver_messages_to_engine(server, engine_container);
  }
}

void destroy_engine_container(void *engine_container_object)
{
  assert(engine_container_object);
  engine_container_t *engine_container;
  unsigned short each_thread;

  engine_container = engine_container_object;

  engine_container->enginey->destroy(engine_container->engine_object);
  for (each_thread = 0; each_thread < MAX_THREADS_PER_ENGINE; each_thread++) {
    cf_x_case_list_destroy(engine_container->thread_inboxes[each_thread]);
  }
  cf_x_case_list_destroy(engine_container->inbox);
  free(engine_container->message_handlers);
  cf_x_sync_period_destroy(engine_container->performance_period);

  free(engine_container);
}

void destroy_maintaining_engine(maintaining_engine_t *maintaining_engine)
{
  free(maintaining_engine);
}

void destroy_running_engine(running_engine_t *running_engine)
{
  free(running_engine);
}

unsigned long engine_count(cf_x_net_server_system_t *server)
{
  return cf_x_case_list_get_size(server->engines);
}

cf_x_core_bool_t engine_is_registered(cf_x_net_server_system_t *server,
    cf_x_net_engine_id_t engine_id)
{
  cf_x_core_bool_t is_registered;

  if (server->engines_array[engine_id]) {
    is_registered = cf_x_core_bool_true;
  } else {
    is_registered = cf_x_core_bool_false;
  }

  return is_registered;
}

void engine_sleep(cf_x_net_server_system_t *server, cf_x_net_engine_id_t engine_id)
{
  usleep(server->engines_array[engine_id]->sleep);
}

void *find_client_post(cf_x_net_server_system_t *server, int client_socket)
{
  void *client_post_object;
  void *decoy_post_object;

  decoy_post_object = server->postey->create_decoy(client_socket);
  if (decoy_post_object) {
    client_post_object
      = cf_x_case_set_find(server->client_posts, decoy_post_object);
    server->postey->destroy_decoy(decoy_post_object);
  } else {
    cf_x_audit_log_trace(server->log, "hnet", "postey->create_decoy");
    client_post_object = NULL;
  }

  return client_post_object;
}

void handle_signal(int signal)
{
  switch (signal) {
    case SIGUSR1:
      break;
    case SIGUSR2:
      break;
    default:
      cf_x_core_stop_requested = cf_x_core_bool_true;
      break;
  }
}

cf_x_net_server_system_t *cf_x_net_server_system_create(const char *name, unsigned short min_port,
    unsigned short max_port, unsigned short max_threads,
    cf_x_core_messagey_t *messagey, cf_x_net_post_postey_t *postey,
    cf_x_net_engine_get_name_f get_engine_name,
    cf_x_config_system_t *config_system, cf_x_audit_log_t *log)
{
  assert(name);
  assert(messagey);
  assert(postey);
  assert(log);
  cf_x_net_server_system_t *server;
  cf_x_core_bool_t so_far_so_good;

  server = malloc(sizeof *server);
  if (server) {
    server->min_port = min_port;
    server->max_port = max_port;
    server->max_threads = max_threads;
    server->thread_count = 0;
    server->unresponsive_client_time_seconds
      = DEFAULT_UNRESPONSIVE_CLIENT_TIME_SECONDS;
    cf_x_net_server_stats_init(&server->stats);
    server->custom_server_object = NULL;
    server->messagey = messagey;
    server->postey = postey;
    server->get_engine_name = get_engine_name;
    server->create_time = time(NULL);
    server->log = log;
    server->config_system = config_system;
    server->accept_thread_created = cf_x_core_bool_false;
    so_far_so_good = cf_x_core_bool_true;
  } else {
    so_far_so_good = cf_x_core_bool_false;
    cf_x_audit_log_trace(server->log, "hnet", "malloc");
  }

  if (so_far_so_good) {
    server->name = strdup(name);
    if (!server->name) {
      so_far_so_good = cf_x_core_bool_false;
    }
  }

  if (so_far_so_good) {
    so_far_so_good = cf_x_net_server_system_create_outbox(server);
  }

  if (so_far_so_good) {
    so_far_so_good = cf_x_net_server_system_create_client_posts(server);
  }

  if (so_far_so_good) {
    so_far_so_good = cf_x_net_server_system_create_client_exchange(server);
  }

  if (so_far_so_good) {
    so_far_so_good = cf_x_net_server_system_create_engines(server);
  }

  if (!so_far_so_good) {
    cf_x_net_server_system_create_rollback(server);
    server = NULL;
  }

  return server;
}

cf_x_core_bool_t cf_x_net_server_system_create_client_exchange(cf_x_net_server_system_t *server)
{
  assert(server);
  cf_x_core_bool_t success;

  server->client_exchange = cf_x_net_exchange_create(server->postey);
  if (server->client_exchange) {
    success = cf_x_core_bool_true;
  } else {
    success = cf_x_core_bool_false;
    cf_x_audit_log_trace(server->log, "hnet", "x_net_exchange_create");
  }

  return success;
}

cf_x_core_bool_t cf_x_net_server_system_create_client_posts(cf_x_net_server_system_t *server)
{
  assert(server);
  cf_x_core_bool_t success;

  cf_x_core_objectey_init(&server->client_posts_objectey, server->postey->compare,
      CF_X_CORE_NO_COPY_FUNCTION, server->postey->destroy,
      CF_X_CORE_NO_EQUAL_FUNCTION, CF_X_CORE_NO_GET_AS_STRING_FUNCTION,
      CF_X_CORE_NO_MOD_FUNCTION);
  server->client_posts
    = cf_x_case_set_create(&server->client_posts_objectey);
  if (server->client_posts) {
    success = cf_x_core_bool_true;
  } else {
    success = cf_x_core_bool_false;
    cf_x_audit_log_trace(server->log, "hnet", "set_create");
  }

  if (success) {
    if (0 == pthread_mutex_init(&server->client_posts_mutex, NULL)) {
      success = cf_x_core_bool_true;
    } else {
      success = cf_x_core_bool_false;
      cf_x_audit_log_trace(server->log, "hnet", "pthread_mutex_init");
      cf_x_case_set_destroy(server->client_posts);
    }
  }

  return success;
}

cf_x_core_bool_t cf_x_net_server_system_create_engines(cf_x_net_server_system_t *server)
{
  assert(server);
  cf_x_core_bool_t success;
  unsigned long each_engine_id;

  server->engines = cf_x_case_list_create(CF_X_CORE_NO_COMPARE_FUNCTION,
      CF_X_CORE_NO_COPY_FUNCTION, destroy_engine_container);
  if (server->engines) {
    for (each_engine_id = 0; each_engine_id < CF_X_NET_SERVER_SYSTEM_MAX_ENGINES;
         each_engine_id++) {
      server->engines_array[each_engine_id] = NULL;
    }
    success = cf_x_core_bool_true;
  } else {
    success = cf_x_core_bool_false;
    cf_x_audit_log_trace(server->log, "hnet", "x_case_list_create");
  }

  return success;
}

cf_x_core_bool_t cf_x_net_server_system_create_outbox(cf_x_net_server_system_t *server)
{
  assert(server);
  cf_x_core_bool_t success;

  server->outbox = cf_x_case_list_create(CF_X_CORE_NO_COMPARE_FUNCTION,
      CF_X_CORE_NO_COPY_FUNCTION, CF_X_CORE_NO_DESTROY_FUNCTION);
  if (server->outbox) {
    success = cf_x_core_bool_true;
  } else {
    success = cf_x_core_bool_false;
    cf_x_audit_log_trace(server->log, "hnet", "list_create");
  }

  if (success) {
    if (0 == pthread_mutex_init(&server->outbox_mutex, NULL)) {
      success = cf_x_core_bool_true;
    } else {
      success = cf_x_core_bool_false;
      cf_x_audit_log_trace(server->log, "hnet", "pthread_mutex_init");
      cf_x_case_list_destroy(server->outbox);
    }
  }

  return success;
}

void cf_x_net_server_system_create_rollback(cf_x_net_server_system_t *server)
{
  if (server) {
    if (server->name) {
      free(server->name);
    }
    if (server->outbox) {
      cf_x_case_list_destroy(server->outbox);
    }
    if (server->client_posts) {
      cf_x_case_set_destroy(server->client_posts);
    }
    if (server->client_exchange) {
      cf_x_net_exchange_destroy(server->client_exchange);
    }
    if (server->engines) {
      cf_x_case_list_destroy(server->engines);
    }
    free(server);
  }
}

void cf_x_net_server_system_destroy(cf_x_net_server_system_t *server)
{
  assert(server);
  void *message_object;

  join_thread(server, server->server_thread);

  cf_x_case_list_iterate_start(server->outbox);
  while ((message_object = cf_x_case_list_iterate_next(server->outbox))) {
    server->messagey->destroy(message_object);
  }
  cf_x_case_list_destroy(server->outbox);

  cf_x_case_set_destroy(server->client_posts);
  cf_x_net_exchange_destroy(server->client_exchange);
  cf_x_case_list_destroy(server->engines);
  free(server->name);
  free(server);
}

cf_x_core_bool_t cf_x_net_server_system_engine_run_thread_quiesce_requested
(cf_x_net_server_system_t *server, cf_x_net_engine_id_t engine_id,
    unsigned short thread_index)
{
  return server->engines_array[engine_id]
    ->run_threads_quiesce_requested[thread_index];
}

cf_x_net_server_system_handle_message_f cf_x_net_server_system_get_handler_for_message
(cf_x_net_server_system_t *server, void *message_object)
{
  assert(server);
  assert(message_object);
  engine_container_t *engine_container;
  cf_x_net_engine_id_t engine_id;
  unsigned long message_type;
  cf_x_net_server_system_handle_message_f handler;

  engine_id = server->messagey->get_engine_id(message_object);
  engine_container = server->engines_array[engine_id];
  message_type = server->messagey->get_type(message_object);

  if (message_type < engine_container->message_handlers_size) {
    handler = *(engine_container->message_handlers + message_type);
  } else {
    cf_x_audit_log_enter(server->log, "hnet",
        "server has no handler for %s engine, message %lu",
        cf_x_net_engine_get_name(engine_container->engine_id,
            server->get_engine_name),
        message_type);
    handler = NULL;
  }

  return handler;
}

cf_x_config_system_t *cf_x_net_server_system_get_config_system(cf_x_net_server_system_t *server)
{
  return server->config_system;
}

void cf_x_net_server_system_get_stats(cf_x_net_server_system_t *server,
    cf_x_net_server_stats_t *server_stats)
{
  assert(server_stats);

  server_stats->engine_count = engine_count(server);
  server_stats->server_send_message_failure_count
    = server->stats.server_send_message_failure_count;
  server_stats->engine_cant_handle_message_now_count
    = server->stats.engine_cant_handle_message_now_count;
  server_stats->engine_handled_message_count
    = server->stats.engine_handled_message_count;
  server_stats->engine_cant_handle_message_count
    = server->stats.engine_cant_handle_message_count;
  server_stats->discarded_message_for_nonexistent_client_count
    = server->stats.discarded_message_for_nonexistent_client_count;
  server_stats->discarded_message_for_unregistered_engine_count
    = server->stats.discarded_message_for_unregistered_engine_count;
  server_stats->discarded_message_engine_inbox_add_failed_count
    = server->stats.discarded_message_engine_inbox_add_failed_count;
  server_stats->uptime_seconds = time(NULL) - server->create_time;
  server_stats->connected_client_count = cf_x_case_set_get_size
    (server->client_posts);
  server_stats->max_threads = server->max_threads;
  server_stats->thread_count = server->thread_count;
}

void cf_x_net_server_system_print_stats(cf_x_net_server_system_t *server)
{
  cf_x_net_server_stats_t stats;
  cf_x_core_time_t *time;
  char *time_string;
  cf_x_core_days_hours_minutes_seconds_t days_hours_minutes_seconds;

  cf_x_net_server_system_get_stats(server, &stats);

  time = cf_x_core_time_create(stats.uptime_seconds);
  if (time) {
    cf_x_core_time_get_days_hours_minutes_seconds
      (time, &days_hours_minutes_seconds);
    time_string = create_string_from_days_hours_minutes_seconds
      (&days_hours_minutes_seconds);
    cf_x_core_time_destroy(time);
  } else {
    time_string = NULL;
  }

  if (time_string) {
    cf_x_audit_log_enter(server->log, "hnet", "%s server: %lu engines, up %s, "
        "handled %lu messages", server->name, stats.engine_count, time_string,
        stats.engine_handled_message_count);
  }

  if (stats.server_send_message_failure_count > 0) {
    cf_x_audit_log_enter(server->log, "hnet", "%s server_send_message() "
        "failures: %lu", server->name,
        stats.server_send_message_failure_count);
  }

  if (stats.discarded_message_for_unregistered_engine_count > 0) {
    cf_x_audit_log_enter(server->log, "hnet", "%s messages received for "
        "unregistered engines: %lu", server->name,
        stats.discarded_message_for_unregistered_engine_count);
  }

  if (stats.engine_cant_handle_message_count > 0) {
    cf_x_audit_log_enter(server->log, "hnet", "%s instances of engine can't "
        "handle message: %lu", server->name,
        stats.engine_cant_handle_message_count);
  }

  if (stats.engine_cant_handle_message_now_count > 0) {
    cf_x_audit_log_enter(server->log, "hnet", "%s instances of engine can't "
        "handle message now: %lu", server->name,
        stats.engine_cant_handle_message_now_count);
  }

  if (time_string) {
    free(time_string);
  }
}

void cf_x_net_server_system_process_messages(cf_x_net_server_system_t *server,
    cf_x_net_engine_id_t engine_id, unsigned short thread_index)
{
  assert(server);
  cf_x_case_list_t *inbox;
  pthread_mutex_t *inbox_mutex;
  cf_x_net_message_status_t message_status;
  void *message_object;
  cf_x_net_server_system_handle_message_f handler;
  unsigned long message_type;
  engine_container_t *engine_container;

  engine_container = server->engines_array[engine_id];
  inbox = engine_container->thread_inboxes[thread_index];
  inbox_mutex = &engine_container->thread_inbox_mutexes[thread_index];

  pthread_mutex_lock(inbox_mutex);
  cf_x_case_list_iterate_start(inbox);
  while ((message_object = cf_x_case_list_iterate_next(inbox))) {
    handler = engine_container->enginey->get_handler_for_message
      (engine_container->engine_object, message_object);
    if (handler) {
      message_status
        = (*handler)(engine_container->engine_object, message_object);
      switch (message_status) {
        case CF_X_NET_MESSAGE_STATUS_HANDLED:
          server->stats.engine_handled_message_count++;
          cf_x_case_list_iterate_remove(inbox);
          break;
        case CF_X_NET_MESSAGE_STATUS_CANT_HANDLE:
          server->stats.engine_cant_handle_message_count++;
          cf_x_case_list_iterate_remove(inbox);
          break;
        case CF_X_NET_MESSAGE_STATUS_CANT_HANDLE_NOW:
          server->stats.engine_cant_handle_message_now_count++;
          break;
        default:
          break;
      }
    } else {
      message_type = server->messagey->get_type(message_object);
      cf_x_audit_log_enter(server->log, "hnet",
          "server has no handler for %s engine's message %lu",
          cf_x_net_engine_get_name(engine_container->engine_id,
              server->get_engine_name),
          message_type);
      cf_x_case_list_iterate_remove(inbox);
    }
  }
  pthread_mutex_unlock(inbox_mutex);
}

cf_x_core_bool_t cf_x_net_server_system_register_engine(cf_x_net_server_system_t *server,
    cf_x_net_engine_id_t engine_id, void *custom_server_object,
    cf_x_net_engine_enginey_t *enginey, unsigned short min_run_threads,
    unsigned short max_run_threads, cf_x_net_maintain_t maintain_schedule,
    unsigned long message_type_count)
{
  assert(min_run_threads > 0);
  assert(max_run_threads >= min_run_threads);
  assert(max_run_threads <= MAX_THREADS_PER_ENGINE);
  assert(!server->engines_array[engine_id]);
  engine_container_t *engine_container;
  cf_x_core_bool_t success;

  if (engine_is_registered(server, engine_id)) {
    success = cf_x_core_bool_false;
  } else {
    success = cf_x_core_bool_true;
  }

  if (success) {
    engine_container = create_engine_container(server, engine_id,
        custom_server_object, enginey, min_run_threads, max_run_threads,
        maintain_schedule, message_type_count);
    if (!engine_container) {
      success = cf_x_core_bool_false;
    }
  }

  if (success) {
    server->engines_array[engine_id] = engine_container;
    if (!cf_x_case_list_add_last(server->engines, engine_container)) {
      cf_x_audit_log_trace(server->log, "hnet", "x_case_list_add_last");
      success = cf_x_core_bool_false;
    }
  }

  if (success) {
    success = cf_x_net_server_system_register_engine_create(engine_container,
        enginey->create, server, custom_server_object);
  }

  return success;
}

cf_x_core_bool_t cf_x_net_server_system_register_engine_create
(engine_container_t *engine_container, cf_x_net_engine_enginey_create_f create,
    cf_x_net_server_system_t *server, void *custom_server_object)
{
  assert(engine_container);
  cf_x_core_bool_t success;

  engine_container->engine_object = create(server, custom_server_object);
  if (engine_container->engine_object) {
    success = cf_x_core_bool_true;
  } else {
    cf_x_audit_log_trace(server->log, "hnet", "create_engine_function");
    success = cf_x_core_bool_false;
  }

  return success;
}

void cf_x_net_server_system_register_message_handler(cf_x_net_server_system_t *server,
    cf_x_net_engine_id_t engine_id, unsigned long message_type,
    cf_x_net_server_system_handle_message_f message_handler)
{
  assert(server);
  assert(server->engines_array[engine_id]);
  assert(message_type
      < server->engines_array[engine_id]->message_handlers_size);
  engine_container_t *engine_container;

  engine_container = server->engines_array[engine_id];
  *(engine_container->message_handlers + message_type) = message_handler;
}

cf_x_core_bool_t cf_x_net_server_system_send_message(cf_x_net_server_system_t *server,
    void *message_object)
{
  cf_x_core_bool_t success;

  pthread_mutex_lock(&server->outbox_mutex);
  if (cf_x_case_list_add_last(server->outbox, message_object)) {
    success = cf_x_core_bool_true;
  } else {
    success = cf_x_core_bool_false;
    server->stats.server_send_message_failure_count++;
  }
  pthread_mutex_unlock(&server->outbox_mutex);

  return success;
}

void cf_x_net_server_system_set_unresponsive_client_time_seconds(cf_x_net_server_system_t *server,
    unsigned long unresponsive_client_time_seconds)
{
  server->unresponsive_client_time_seconds = unresponsive_client_time_seconds;
}

cf_x_core_bool_t cf_x_net_server_system_start(cf_x_net_server_system_t *server)
{
  cf_x_core_bool_t success;

  cf_x_core_stop_requested = cf_x_core_bool_false;

  if (start(server)) {
    if (create_thread
        (server, &server->server_thread, server_thread, (void *) server)) {
      success = cf_x_core_bool_true;
    } else {
      success = cf_x_core_bool_false;
    }
  } else {
    success = cf_x_core_bool_false;
  }
  if (!success) {
    stop(server);
  }

  return success;
}

cf_x_case_list_t *cf_x_net_server_system_take_unprocessed_messages
(cf_x_net_server_system_t *server)
{
  return NULL;
}

cf_x_case_list_t *cf_x_net_server_system_take_unsent_messages(cf_x_net_server_system_t *server)
{
  return NULL;
}

cf_x_core_bool_t join_thread(cf_x_net_server_system_t *server, pthread_t thread)
{
  cf_x_core_bool_t success;

  if (0 == pthread_join(thread, NULL)) {
    success = cf_x_core_bool_true;
    server->thread_count--;
  } else {
    success = cf_x_core_bool_false;
  }

  return success;
}

void *maintain_engine(void *maintaining_engine_object)
{
  maintaining_engine_t *maintaining_engine;
  void *engine_object;
  cf_x_net_engine_id_t engine_id;
  cf_x_net_server_system_t *server;
  engine_container_t *engine_container;
  cf_x_sync_period_t *maintenance_period;

  maintaining_engine = maintaining_engine_object;
  engine_object = maintaining_engine->engine_object;
  engine_id = maintaining_engine->engine_id;
  server = maintaining_engine->server;
  engine_container = server->engines_array[engine_id];

  maintenance_period
    = cf_x_sync_period_create(engine_container->maintain_schedule);
  if (maintenance_period) {
    while (!engine_container->stop_requested) {
      if (cf_x_sync_period_once(maintenance_period)) {
        maintaining_engine->maintain(engine_object);
      }
      engine_sleep(server, engine_id);
    }
    cf_x_sync_period_destroy(maintenance_period);
  } else {
    cf_x_audit_log_trace(server->log, "hnet", "new");
  }

  destroy_maintaining_engine(maintaining_engine);

  return NULL;
}

void manage_threads(cf_x_net_server_system_t *server)
{
}

void post_messages_to_clients(cf_x_net_server_system_t *server)
{
  assert(server);
  void *message_object;
  void *client_post_object;
  int client_socket;

  pthread_mutex_lock(&server->outbox_mutex);
  cf_x_case_list_iterate_start(server->outbox);
  while ((message_object = cf_x_case_list_iterate_next(server->outbox))) {
    client_socket = server->messagey->get_client_socket(message_object);
    client_post_object = find_client_post(server, client_socket);
    if (client_post_object) {
      if (server->postey->send_message(client_post_object, message_object)) {
        cf_x_case_list_iterate_remove(server->outbox);
      } else {
        cf_x_audit_log_trace(server->log, "hnet", "postey->send_message");
      }
    } else {
      cf_x_case_list_iterate_remove(server->outbox);
      server->stats.discarded_message_for_nonexistent_client_count++;
      server->messagey->destroy(message_object);
      cf_x_audit_log_enter(server->log, "hnet", "server "
          "post_messages_to_clients() discarded "
          "message for nonexistent client %i", client_socket);
    }
  }
  pthread_mutex_unlock(&server->outbox_mutex);
}

void receive_messages_from_client_post(cf_x_net_server_system_t *server,
    void *post_object)
{
  assert(server);
  assert(post_object);
  void *message_object;
  cf_x_net_engine_id_t engine_id;
  engine_container_t *engine_container;
  cf_x_case_list_t *engine_inbox;

  while ((message_object = server->postey->receive_message(post_object))) {
    engine_id = server->messagey->get_engine_id(message_object);
    if (engine_is_registered(server, engine_id)) {
      engine_container = server->engines_array[engine_id];
      engine_inbox = engine_container->inbox;
      if (!cf_x_case_list_add_last(engine_inbox, message_object)) {
        cf_x_audit_log_trace(server->log, "hnet", "x_case_list_add_last");
        server->stats.discarded_message_engine_inbox_add_failed_count++;
        server->messagey->destroy(message_object);
      }
    } else {
      cf_x_audit_log_enter(server->log, "hnet",
          "server discarded message type %lu for %s "
          "engine because the engine is not registered",
          server->messagey->get_type(message_object),
          cf_x_net_engine_get_name(engine_id, server->get_engine_name));
      server->stats.discarded_message_for_unregistered_engine_count++;
      server->messagey->destroy(message_object);
    }
  }
}

void receive_messages_from_clients(cf_x_net_server_system_t *server)
{
  void *client_post_object;
  cf_x_case_set_t *client_posts;

  client_posts = server->client_posts;
  cf_x_case_set_iterate_start(client_posts);
  while ((client_post_object = cf_x_case_set_iterate_next(client_posts))) {
    receive_messages_from_client_post(server, client_post_object);
  }
}

void *run_engine(void *running_engine_object)
{
  running_engine_t *running_engine;
  void *engine_object;
  cf_x_net_engine_id_t engine_id;
  cf_x_net_server_system_t *server;
  engine_container_t *engine_container;
  cf_x_net_engine_thread_t *engine_thread;

  running_engine = running_engine_object;
  engine_object = running_engine->engine_object;
  engine_id = running_engine->engine_id;
  server = running_engine->server;
  engine_container = server->engines_array[engine_id];
  engine_thread = cf_x_net_engine_create_thread
    (engine_object, running_engine->thread_index);

  running_engine->start(engine_thread);
  while (!engine_container->stop_requested) {
    running_engine->run(engine_thread);
    engine_sleep(server, engine_id);
  }
  running_engine->stop(engine_thread);

  cf_x_net_engine_destroy_thread(engine_thread);
  destroy_running_engine(running_engine);

  return NULL;
}

cf_x_core_bool_t serversocket_bind_listen(cf_x_net_server_system_t *server)
{
  assert(server);
  cf_x_core_bool_t success;
  unsigned short port;

  success = cf_x_core_bool_false;

  for (port = server->min_port;
       (port <= server->max_port) && (!success); port++) {
    server->socket = cf_x_net_server_socket_create(port);
    if (server->socket >= 0) {
      success = cf_x_core_bool_true;
      cf_x_audit_log_enter(server->log, "hnet", "listening on port %i", port);
    } else {
      cf_x_audit_log_enter(server->log, "hnet", "x_net_serversocket_create");
    }
  }

  return success;
}

void *server_thread(void *server_object)
{
  cf_x_net_server_system_t *server;

  server = server_object;

  while (!cf_x_core_stop_requested) {

    pthread_mutex_lock(&server->client_posts_mutex);
    {
      cf_x_net_exchange_send_and_receive_messages(server->client_exchange);
      post_messages_to_clients(server);
      receive_messages_from_clients(server);
      close_disconnected_clients(server);
      if (server->unresponsive_client_time_seconds) {
        close_unresponsive_clients(server);
      }
    }
    pthread_mutex_unlock(&server->client_posts_mutex);

    deliver_messages_to_engines(server);

    manage_threads(server);

    usleep(CF_X_NET_SERVER_SYSTEM_SLEEP_MICROSECONDS);

  }

  stop(server);

  return NULL;
}

cf_x_core_bool_t start(cf_x_net_server_system_t *server)
{
  engine_container_t *engine_container;
  cf_x_net_engine_id_t engine_id;
  cf_x_core_bool_t success;

  success = cf_x_core_bool_true;

  signal(SIGINT, handle_signal);
  signal(SIGUSR1, handle_signal);
  signal(SIGUSR2, handle_signal);
  signal(SIGPIPE, SIG_IGN);

  cf_x_case_list_iterate_start(server->engines);
  while ((engine_container = cf_x_case_list_iterate_next(server->engines))) {
    engine_id = engine_container->engine_id;
    if (!start_engine(server, engine_id)) {
      success = cf_x_core_bool_false;
      cf_x_audit_log_trace(server->log, "hnet", "start_engine");
      break;
    }
  }

  if (success) {
    if (!serversocket_bind_listen(server)) {
      success = cf_x_core_bool_false;
      cf_x_audit_log_trace(server->log, "hnet", "serversocket_bind_listen");
    }
  }

  if (success) {
    if (create_thread(server, &server->accept_thread, accept_thread,
            (void *) server)) {
      server->accept_thread_created = cf_x_core_bool_true;
    } else {
      success = cf_x_core_bool_false;
      cf_x_audit_log_trace(server->log, "hnet", "create_thread");
    }
  }

  return success;
}

cf_x_core_bool_t start_engine(cf_x_net_server_system_t *server, cf_x_net_engine_id_t engine_id)
{
  cf_x_core_bool_t success;
  unsigned short each_thread;
  engine_container_t *engine_container;

  success = cf_x_core_bool_true;
  engine_container = server->engines_array[engine_id];

  for (each_thread = 0; each_thread < engine_container->min_run_threads;
       each_thread++) {
    success = start_engine_run_thread(server, engine_id, engine_container,
        each_thread);
    if (success) {
      engine_container->run_thread_count++;
    } else {
      break;
    }
  }
  if (success) {
    success = start_engine_maintain_thread(server, engine_id,
        engine_container);
  }

  return success;
}

cf_x_core_bool_t start_engine_maintain_thread(cf_x_net_server_system_t *server,
    cf_x_net_engine_id_t engine_id, engine_container_t *engine_container)
{
  cf_x_core_bool_t success;
  pthread_t *pthread_address;
  maintaining_engine_t *maintaining_engine;

  pthread_address = &engine_container->maintain_thread;
  maintaining_engine = create_maintaining_engine(server,
      engine_container->engine_object, engine_id,
      engine_container->enginey->maintain);
  if (maintaining_engine) {
    success = create_thread(server, pthread_address, maintain_engine,
        maintaining_engine);
  } else {
    success = cf_x_core_bool_false;
  }

  return success;
}

cf_x_core_bool_t start_engine_run_thread(cf_x_net_server_system_t *server,
    cf_x_net_engine_id_t engine_id, engine_container_t *engine_container,
    unsigned short thread_index)
{
  cf_x_core_bool_t success;
  running_engine_t *running_engine;
  pthread_t *pthread_address;

  pthread_address = &engine_container->threads[thread_index];
  running_engine = create_running_engine(server,
      engine_container->engine_object, engine_id,
      engine_container->enginey->start, engine_container->enginey->run,
      engine_container->enginey->stop, thread_index);
  if (running_engine) {
    success
      = create_thread(server, pthread_address, run_engine, running_engine);
  } else {
    success = cf_x_core_bool_false;
  }

  return success;
}

void stop(cf_x_net_server_system_t *server)
{
  unsigned short each_thread;
  unsigned short run_thread_count;
  engine_container_t *engine_container;

  if (server->accept_thread_created) {
    join_thread(server, server->accept_thread);
  }

  cf_x_net_server_socket_destroy(server->socket);

  cf_x_case_list_iterate_start(server->engines);
  while ((engine_container = cf_x_case_list_iterate_next(server->engines))) {
    engine_container->stop_requested = cf_x_core_bool_true;
  }

  cf_x_case_list_iterate_start(server->engines);
  while ((engine_container = cf_x_case_list_iterate_next(server->engines))) {
    if (engine_container->enginey->maintain) {
      join_thread(server, engine_container->maintain_thread);
    }
    run_thread_count = engine_container->run_thread_count;
    for (each_thread = 0; each_thread < run_thread_count; each_thread++) {
      join_thread(server, engine_container->threads[each_thread]);
    }
  }
}

cf_x_core_bool_t threads_maxed_out(cf_x_net_server_system_t *server)
{
  assert(server);
  cf_x_core_bool_t maxed_out;

  if (server->thread_count < server->max_threads) {
    maxed_out = cf_x_core_bool_false;
  } else {
    maxed_out = cf_x_core_bool_true;
  }

  return maxed_out;
}
