#ifndef x_net_engine_thread_h
#define x_net_engine_thread_h

struct x_net_engine_thread_t {
  void *engine_object;
  unsigned short thread_index;
};
typedef struct x_net_engine_thread_t x_net_engine_thread_t;

x_net_engine_thread_t *x_net_engine_create_thread(void *engine_object,
    unsigned short thread_index);

void x_net_engine_destroy_thread(x_net_engine_thread_t *engine_thread);

#endif
