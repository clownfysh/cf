#include "x/container/list.h"
#include "x/core/tools.h"
#include "x/net/post.h"
#include "x/net/socket.h"

/*  TODO: add support for message encodings  */

#define POST_MESSAGE_MAGIC 19780112

struct post_message_header_t {
  long int magic;
  short int encoding;
  short int engine_id;
  long int message_type;
  long int data_size;
};
typedef struct post_message_header_t post_message_header_t;

struct x_net_post_t {
  int socket;
  x_container_list_t *inbox;
  x_container_list_t *outbox;
  post_message_header_t current_message_header;
  char *current_message_data;
  x_core_bool_t have_message_header;
  x_net_post_stats_t stats;
  unsigned long magic;

  char *out_buffer;
  unsigned long out_buffer_size;
  x_core_bool_t currently_sending_out_buffer;
  unsigned long out_buffer_send_position;

  char *in_buffer;
  x_core_bool_t in_buffer_have_complete_header;
  unsigned long in_buffer_receive_position;
  post_message_header_t in_message_header;

  time_t last_receive_activity_time;
  x_core_bool_t socket_closed;
};

static void x_net_post_create_in_buffer(x_net_post_t *post);

static x_core_bool_t x_net_post_create_inbox(x_net_post_t *post);

static void x_net_post_create_out_buffer(x_net_post_t *post);

static x_core_bool_t x_net_post_create_outbox(x_net_post_t *post);

static void x_net_post_create_rollback(x_net_post_t *post);

static void init_post_message_header(x_net_post_t *post,
    post_message_header_t *post_message_header,
    x_core_message_encoding_t encoding, x_net_engine_id_t engine_id,
    unsigned long message_type, unsigned long data_size);

static void ntox_post_message_header
(post_message_header_t *post_message_header);

static x_core_bool_t put_message_into_out_buffer(x_net_post_t *post,
    x_core_message_t *message);

static x_core_bool_t put_received_message_in_inbox(x_net_post_t *post);

static x_core_bool_t receive_messages_body(x_net_post_t *post);

static x_core_bool_t receive_messages_header(x_net_post_t *post);

static void reset_for_next_receive(x_net_post_t *post);

static void reset_for_next_send(x_net_post_t *post);

static void send_messages_get_new_message(x_net_post_t *post);

static x_core_bool_t send_messages_send_current_message(x_net_post_t *post);

int x_net_post_compare(void *post_object_a,
    void *post_object_b)
{
  int compare_result;
  x_net_post_t *post_a;
  x_net_post_t *post_b;

  post_a = post_object_a;
  post_b = post_object_b;

  if (post_a->socket == post_b->socket) {
    compare_result = 0;
  } else if (post_a->socket < post_b->socket) {
    compare_result = -1;
  } else {
    compare_result = 1;
  }

  return compare_result;
}

void *x_net_post_create(int socket)
{
  x_net_post_t *post;
  x_core_bool_t success;

  post = malloc(sizeof *post);
  if (post) {
    post->socket = socket;
    post->have_message_header = x_core_bool_false;
    post->magic = htonl(POST_MESSAGE_MAGIC);
    post->currently_sending_out_buffer = x_core_bool_false;
    post->last_receive_activity_time = time(NULL);
    x_net_post_stats_init(&post->stats);
    x_net_post_create_in_buffer(post);
    x_net_post_create_out_buffer(post);
    post->socket_closed = x_core_bool_false;
    success = x_core_bool_true;
  } else {
    x_core_trace("malloc");
    success = x_core_bool_false;
  }

  if (success) {
    success = x_net_post_create_inbox(post);
  }

  if (success) {
    success = x_net_post_create_outbox(post);
  }

  if (!success) {
    x_net_post_create_rollback(post);
    post = NULL;
  }

  return post;
}

void *x_net_post_create_decoy(int socket)
{
  x_net_post_t *post;

  post = malloc(sizeof *post);
  if (post) {
    post->socket = socket;
  } else {
    x_core_trace("malloc");
  }

  return post;
}

void x_net_post_create_in_buffer(x_net_post_t *post)
{
  post->in_buffer = NULL;
  post->in_buffer_have_complete_header = x_core_bool_false;
  post->in_buffer_receive_position = 0;
}

x_core_bool_t x_net_post_create_inbox(x_net_post_t *post)
{
  x_core_bool_t success;

  post->inbox = x_container_list_create(X_CORE_NO_COMPARE_FUNCTION,
      x_core_message_copy, X_CORE_NO_DESTROY_FUNCTION);
  if (post->inbox) {
    success = x_core_bool_true;
  } else {
    x_core_trace("x_container_list_create");
    success = x_core_bool_false;
  }

  return success;
}

void x_net_post_create_out_buffer(x_net_post_t *post)
{
  post->out_buffer = NULL;
  post->currently_sending_out_buffer = x_core_bool_false;
  post->out_buffer_send_position = 0;
}

x_core_bool_t x_net_post_create_outbox(x_net_post_t *post)
{
  x_core_bool_t success;

  post->outbox = x_container_list_create(X_CORE_NO_COMPARE_FUNCTION,
      x_core_message_copy, x_core_message_destroy);
  if (post->outbox) {
    success = x_core_bool_true;
  } else {
    x_core_trace("x_container_list_create");
    success = x_core_bool_false;
  }

  return success;
}

void x_net_post_create_rollback(x_net_post_t *post)
{
  if (post) {
    if (post->in_buffer) {
      free(post->in_buffer);
    }
    if (post->inbox) {
      x_container_list_destroy(post->inbox);
    }
    if (post->outbox) {
      x_container_list_destroy(post->outbox);
    }
  }
}

void x_net_post_destroy(void *post_object)
{
  assert(post_object);
  x_net_post_t *post;
  x_core_message_t *message;

  post = post_object;

  x_container_list_iterate_start(post->inbox);
  while ((message = x_container_list_iterate_next(post->inbox))) {
    x_core_message_destroy(message);
  }
  x_container_list_destroy(post->inbox);

  x_container_list_destroy(post->outbox);

  if (post->in_buffer) {
    free(post->in_buffer);
  }
  if (post->out_buffer) {
    free(post->out_buffer);
  }

  free(post);
}

void x_net_post_destroy_decoy(void *post_object)
{
  free(post_object);
}

time_t x_net_post_get_last_receive_activity_time(void *post_object)
{
  assert(post_object);
  x_net_post_t *post;

  post = post_object;

  return post->last_receive_activity_time;
}

int x_net_post_get_socket(void *post_object)
{
  assert(post_object);
  x_net_post_t *post;

  post = post_object;

  return post->socket;
}

void x_net_post_get_stats(void *post_object,
    x_net_post_stats_t *post_stats)
{
  assert(post_object);
  assert(post_stats);
  x_net_post_t *post;

  post = post_object;

  post_stats->messages_sent = post->stats.messages_sent;
  post_stats->messages_received = post->stats.messages_received;
  post_stats->messages_in_inbox = post->stats.messages_in_inbox;
  post_stats->messages_in_outbox = post->stats.messages_in_outbox;
  post_stats->most_messages_ever_in_inbox
    = post->stats.most_messages_ever_in_inbox;
  post_stats->most_messages_ever_in_outbox
    = post->stats.most_messages_ever_in_outbox;
  post_stats->send_message_failures = post->stats.send_message_failures;
  post_stats->receive_message_failures
    = post->stats.receive_message_failures;
  post_stats->messages_not_sent_due_to_socket_send_failures
    = post->stats.messages_not_sent_due_to_socket_send_failures;
}

x_core_bool_t x_net_post_is_socket_closed(void *post_object)
{
  assert(post_object);
  x_net_post_t *post;

  post = post_object;

  return post->socket_closed;
}

void *x_net_post_receive_message(void *post_object)
{
  assert(post_object);
  x_net_post_t *post;
  x_core_message_t *message;

  post = post_object;

  message = x_container_list_find_first(post->inbox);
  if (message) {
    x_container_list_remove_first(post->inbox);
  }

  return message;
}

void x_net_post_receive_messages(void *post_object)
{
  assert(post_object);
  x_net_post_t *post;

  post = post_object;

  post->last_receive_activity_time = time(NULL);

  if (!post->in_buffer_have_complete_header) {
    if (receive_messages_header(post)) {
      post->in_buffer_have_complete_header = x_core_bool_true;
    }
  }
  if (post->in_buffer_have_complete_header) {
    if (receive_messages_body(post)) {
      reset_for_next_receive(post);
    }
  }
}

x_core_bool_t x_net_post_send_message(void *post_object,
    void *message_object)
{
  assert(post_object);
  assert(message_object);
  x_core_bool_t success;
  x_net_post_t *post;

  post = post_object;

  success = x_container_list_add_last(post->outbox, message_object);

  return success;
}

void x_net_post_send_messages(void *post_object)
{
  assert(post_object);
  x_net_post_t *post;

  post = post_object;

  if (!post->currently_sending_out_buffer) {
    send_messages_get_new_message(post);
  }
  if (post->currently_sending_out_buffer) {
    if (send_messages_send_current_message(post)) {
      reset_for_next_send(post);
    }
  }
}

x_container_list_t *x_net_post_take_unsent_messages(x_net_post_t *post)
{
  assert(post);
  x_container_list_t *list;
  x_core_message_t *message;

  list = x_container_list_create(X_CORE_NO_COMPARE_FUNCTION,
      x_core_message_copy, X_CORE_NO_DESTROY_FUNCTION);
  if (list) {
    x_container_list_iterate_start(post->outbox);
    while ((message = x_container_list_iterate_next(post->outbox))) {
      if (x_container_list_add_last(list, message)) {
        x_container_list_iterate_remove(post->outbox);
      } else {
        x_core_trace("x_container_list_add_last");
      }
    }
  } else {
    x_core_trace("x_container_list_create");
  }

  return list;
}

void init_post_message_header(x_net_post_t *post,
    post_message_header_t *post_message_header,
    x_core_message_encoding_t encoding, x_net_engine_id_t engine_id,
    unsigned long message_type, unsigned long data_size)
{
  post_message_header->magic = post->magic;
  post_message_header->encoding = htons(encoding);
  post_message_header->engine_id = htons(engine_id);
  post_message_header->message_type = htonl(message_type);
  post_message_header->data_size = htonl(data_size);
}

void ntox_post_message_header(post_message_header_t *post_message_header)
{
  post_message_header->magic = ntohl(post_message_header->magic);
  post_message_header->encoding = ntohs(post_message_header->encoding);
  post_message_header->engine_id = ntohs(post_message_header->engine_id);
  post_message_header->message_type = ntohl(post_message_header->message_type);
  post_message_header->data_size = ntohl(post_message_header->data_size);
}

x_core_bool_t put_message_into_out_buffer(x_net_post_t *post,
    x_core_message_t *message)
{
  x_core_bool_t success;
  unsigned long post_message_size;
  post_message_header_t post_message_header;
  unsigned long message_data_size;

  message_data_size = x_core_message_get_data_get_size(message);
  init_post_message_header(post, &post_message_header,
      x_core_message_get_encoding(message),
      x_core_message_get_engine_id(message),
      x_core_message_get_type(message), message_data_size);

  post_message_size = (sizeof post_message_header) + message_data_size;

  post->out_buffer = malloc(post_message_size);
  if (post->out_buffer) {
    memcpy(post->out_buffer, &post_message_header, sizeof post_message_header);
    memcpy(post->out_buffer + (sizeof post_message_header),
        x_core_message_get_data(message), message_data_size);
    post->out_buffer_size = post_message_size;
    success = x_core_bool_true;
  } else {
    x_core_trace("malloc");
    success = x_core_bool_false;
  }

  return success;
}

x_core_bool_t put_received_message_in_inbox(x_net_post_t *post)
{
  x_core_bool_t success;
  x_core_message_t *message;

  message = x_core_message_create(post->socket,
      post->in_message_header.engine_id, post->in_message_header.message_type,
      post->in_buffer, post->in_message_header.data_size);
  if (message) {
    if (x_container_list_add_last(post->inbox, message)) {
      success = x_core_bool_true;
    } else {
      x_core_trace("x_container_list_add_last");
      success = x_core_bool_false;
    }
  } else {
    x_core_trace("x_core_message_create");
    success = x_core_bool_false;
  }

  return success;
}

x_core_bool_t receive_messages_body(x_net_post_t *post)
{
  assert(post);
  assert(post->in_buffer_have_complete_header);
  assert(post->in_buffer);
  x_core_bool_t received_complete_message;
  unsigned long bytes_remaining_to_read;
  int actual_bytes_read;

  received_complete_message = x_core_bool_false;

  bytes_remaining_to_read = post->in_message_header.data_size
    - post->in_buffer_receive_position;
  if (bytes_remaining_to_read > 0) {
    actual_bytes_read = x_net_socket_receive(post->socket,
        post->in_buffer + post->in_buffer_receive_position,
        bytes_remaining_to_read);
    if (actual_bytes_read > 0) {
      post->in_buffer_receive_position += actual_bytes_read;
      if (post->in_message_header.data_size
          == post->in_buffer_receive_position) {
        received_complete_message = x_core_bool_true;
      }
    } else {
      post->socket_closed = x_core_bool_true;
    }
  } else {
    received_complete_message = x_core_bool_true;
  }
  if (received_complete_message) {
    if (!put_received_message_in_inbox(post)) {
      x_core_trace("put_received_message_in_inbox failed()");
    }
  }

  return received_complete_message;
}

x_core_bool_t receive_messages_header(x_net_post_t *post)
{
  unsigned long bytes_remaining_to_receive;
  int actual_bytes_read;
  x_core_bool_t received_a_header;

  received_a_header = x_core_bool_false;

  bytes_remaining_to_receive = (sizeof post->in_message_header)
    - post->in_buffer_receive_position;
  actual_bytes_read = x_net_socket_receive(post->socket,
      &post->in_message_header + post->in_buffer_receive_position,
      bytes_remaining_to_receive);
  if (actual_bytes_read > 0) {
    post->in_buffer_receive_position += actual_bytes_read;
    if ((sizeof post->in_message_header) == post->in_buffer_receive_position) {
      ntox_post_message_header(&post->in_message_header);
      if (POST_MESSAGE_MAGIC != post->in_message_header.magic) {
        x_core_trace("[post] message magic is incorrect\n");
      }
      received_a_header = x_core_bool_true;
      post->in_buffer_receive_position = 0;
      post->in_buffer = malloc(post->in_message_header.data_size);
      if (!post->in_buffer) {
        x_core_trace("malloc");
      }
    }
  } else {
    post->socket_closed = x_core_bool_true;
  }

  return received_a_header;
}

void reset_for_next_receive(x_net_post_t *post)
{
  free(post->in_buffer);
  post->in_buffer = NULL;
  post->in_buffer_have_complete_header = x_core_bool_false;
  post->in_buffer_receive_position = 0;
}

void reset_for_next_send(x_net_post_t *post)
{
  free(post->out_buffer);
  post->out_buffer = NULL;
  post->currently_sending_out_buffer = x_core_bool_false;
  post->out_buffer_send_position = 0;
}

void send_messages_get_new_message(x_net_post_t *post)
{
  x_core_message_t *message;

  message = x_container_list_find_first(post->outbox);
  if (message) {
    if (put_message_into_out_buffer(post, message)) {
      x_container_list_remove_first(post->outbox);
      post->currently_sending_out_buffer = x_core_bool_true;
    } else {
      x_core_trace("put_message_into_out_buffer");
    }
  }
}

x_core_bool_t send_messages_send_current_message(x_net_post_t *post)
{
  x_core_bool_t sent_complete_message;
  unsigned long bytes_remaining_to_send;
  int actual_bytes_sent;

  bytes_remaining_to_send = post->out_buffer_size
    - post->out_buffer_send_position;

  if (bytes_remaining_to_send > 0) {
    actual_bytes_sent = x_net_socket_send(post->socket,
        post->out_buffer + post->out_buffer_send_position,
        bytes_remaining_to_send);
    if (actual_bytes_sent > 0) {
      post->out_buffer_send_position += actual_bytes_sent;
      if (post->out_buffer_send_position == post->out_buffer_size) {
        sent_complete_message = x_core_bool_true;
      } else {
        sent_complete_message = x_core_bool_false;
      }
    } else {
      post->socket_closed = x_core_bool_true;
      sent_complete_message = x_core_bool_false;
    }
  } else {
    sent_complete_message = x_core_bool_true;
  }

  return sent_complete_message;
}
