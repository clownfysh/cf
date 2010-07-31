#ifndef cf_x_net_engine_thread_h
#define cf_x_net_engine_thread_h

struct cf_x_net_engine_thread_t {
  void *engine_object;
  unsigned short thread_index;
};
typedef struct cf_x_net_engine_thread_t cf_x_net_engine_thread_t;

cf_x_net_engine_thread_t *cf_x_net_engine_create_thread(void *engine_object,
    unsigned short thread_index);

void cf_x_net_engine_destroy_thread(cf_x_net_engine_thread_t *engine_thread);

#endif
