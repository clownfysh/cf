#ifndef trf_tools_h
#define trf_tools_h

#include "i/tools.h"

i_bool_t trf_pause_is_requested();
i_bool_t trf_platform_free();
i_bool_t trf_platform_init();
i_bool_t trf_platform_input();
i_bool_t trf_platform_output();
void trf_request_pause();

#endif
