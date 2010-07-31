#ifndef cf_x_pingpong_message_h
#define cf_x_pingpong_message_h

#define cf_x_PINGPONG_MESSAGE_TYPE_COUNT 3
enum cf_x_pingpong_message_t {
  cf_x_PINGPONG_MESSAGE_UNKNOWN = 0,
  cf_x_PINGPONG_MESSAGE_PING = 1,
  cf_x_PINGPONG_MESSAGE_PONG = 2
};
typedef enum cf_x_pingpong_message_t cf_x_pingpong_message_t;

cf_x_core_message_t *cf_x_pingpong_message_create(int client_socket,
    unsigned long message_type);

#endif
