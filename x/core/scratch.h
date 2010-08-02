#ifndef cf_x_core_scratch_h
#define cf_x_core_scratch_h

struct cf_x_core_scratch_t;
typedef struct cf_x_core_scratch_t cf_x_core_scratch_t;

cf_x_core_scratch_t *cf_x_core_scratch_create();

void cf_x_core_scratch_destroy(cf_x_core_scratch_t *cf_x_core_scratch);

#endif
