#ifndef x_pingpong_message_h
#define x_pingpong_message_h

#define X_PINGPONG_MESSAGE_TYPE_COUNT 3
enum x_pingpong_message_t {
  X_PINGPONG_MESSAGE_UNKNOWN = 0,
  X_PINGPONG_MESSAGE_PING = 1,
  X_PINGPONG_MESSAGE_PONG = 2
};
typedef enum x_pingpong_message_t x_pingpong_message_t;

x_core_message_t *x_pingpong_message_create(int client_socket,
    unsigned long message_type);

#endif
