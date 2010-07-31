#include "cf/x/config/tools.h"
#include "cf/x/core/constants.h"
#include "cf/x/sync/period.h"
#include "cf/x/core/tools.h"
#include "cf/x/net/star/client/system.h"
#include "cf/x/net/star/client/conf.h"

static void handle_signal(int signal);

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

int main(int argc, char *argv[])
{
  cf_x_config_disable_if_running_batch_tests(argc, argv);

  cf_x_net_star_client_system_t *starclient;
  cf_x_net_star_client_conf_t *starclient_conf;
  cf_x_sync_period_t *period;
  void *custom_client_context;
  cf_x_audit_log_t *log;

  custom_client_context = NULL;

  signal(SIGINT, handle_signal);
  signal(SIGUSR1, handle_signal);
  signal(SIGUSR2, handle_signal);
  signal(SIGPIPE, SIG_IGN);

  log = cf_x_audit_log_create(stdout);
  if (!log) {
    cf_x_core_trace_exit("x_audit_log_create");
  }

  period = cf_x_sync_period_create(8);
  if (!period) {
    cf_x_core_trace_exit("x_core_period_create");
  }

  starclient_conf = cf_x_net_star_client_conf_create("x_net_starclient_test.conf");
  if (!starclient_conf) {
    cf_x_core_trace_exit("x_net_starclient_conf_create");
  }

  starclient = cf_x_net_star_client_system_create(starclient_conf->star_arm_ips,
      starclient_conf->star_arm_port_min,
      starclient_conf->star_arm_port_max,
      starclient_conf->node_server_exclude_ip,
      starclient_conf->node_server_exclude_port,
      starclient_conf->node_server_exclude_port,
      custom_client_context, log);
  if (starclient) {
    if (cf_x_net_star_client_system_connect(starclient)) {
      printf("starclient connected to star\n");
      while (!cf_x_core_stop_requested) {
        cf_x_net_star_client_system_process_messages(starclient);

        /*
          TODO: move this to the starclient
        */
        if (!cf_x_net_star_client_system_star_available(starclient)) {
          if (cf_x_sync_period_once(period)) {
            printf("star is unavailable\n");
          }
        }

        usleep(CF_X_CORE_STANDARD_SLEEP_MICROSECONDS);
      }
      printf("starclient shutting down gracefully\n");
    } else {
      printf("starclient could not connect to star\n");
    }
  } else {
    cf_x_core_trace("x_net_starclient_create");
  }

  cf_x_net_star_client_system_destroy(starclient);
  cf_x_net_star_client_conf_destroy(starclient_conf);
  cf_x_sync_period_destroy(period);
  cf_x_audit_log_destroy(log);

  return 0;
}
