#include "x/config/tools.h"
#include "x/core/constants.h"
#include "x/core/period.h"
#include "x/core/tools.h"
#include "x/net/starclient.h"
#include "x/net/starclient_conf.h"

static void handle_signal(int signal);

void handle_signal(int signal)
{
  switch (signal) {
    case SIGUSR1:
      break;
    case SIGUSR2:
      break;
    default:
      x_core_stop_requested = x_core_bool_true;
      break;
  }
}

int main(int argc, char *argv[])
{
  x_config_disable_if_running_batcx_tests(argc, argv);

  x_net_starclient_t *starclient;
  x_net_starclient_conf_t *starclient_conf;
  x_core_period_t *period;
  void *custom_client_context;
  x_audit_log_t *log;

  custom_client_context = NULL;

  signal(SIGINT, handle_signal);
  signal(SIGUSR1, handle_signal);
  signal(SIGUSR2, handle_signal);
  signal(SIGPIPE, SIG_IGN);

  log = x_audit_log_create(stdout);
  if (!log) {
    x_core_trace_exit("x_audit_log_create");
  }

  period = x_core_period_create(8);
  if (!period) {
    x_core_trace_exit("x_core_period_create");
  }

  starclient_conf = x_net_starclient_conf_create("x_net_starclient_test.conf");
  if (!starclient_conf) {
    x_core_trace_exit("x_net_starclient_conf_create");
  }

  starclient = x_net_starclient_create(starclient_conf->star_arm_ips,
      starclient_conf->star_arm_port_min,
      starclient_conf->star_arm_port_max,
      starclient_conf->node_server_exclude_ip,
      starclient_conf->node_server_exclude_port,
      starclient_conf->node_server_exclude_port,
      custom_client_context, log);
  if (starclient) {
    if (x_net_starclient_connect(starclient)) {
      printf("starclient connected to star\n");
      while (!x_core_stop_requested) {
        x_net_starclient_process_messages(starclient);

        /*
          TODO: move this to the starclient
        */
        if (!x_net_starclient_star_available(starclient)) {
          if (x_core_period_once(period)) {
            printf("star is unavailable\n");
          }
        }

        usleep(X_CORE_STANDARD_SLEEP_MICROSECONDS);
      }
      printf("starclient shutting down gracefully\n");
    } else {
      printf("starclient could not connect to star\n");
    }
  } else {
    x_core_trace("x_net_starclient_create");
  }

  x_net_starclient_destroy(starclient);
  x_net_starclient_conf_destroy(starclient_conf);
  x_core_period_destroy(period);
  x_audit_log_destroy(log);

  return 0;
}
