#ifndef cf_x_math_combination_h
#define cf_x_math_combination_h

#include "cf/x/case/set.h"

struct cf_x_math_combination_t;
typedef struct cf_x_math_combination_t cf_x_math_combination_t;

cf_x_math_combination_t *cf_x_math_combination_create(cf_x_case_set_t *set);

void cf_x_math_combination_destroy(cf_x_math_combination_t *combination);

cf_x_case_set_t *cf_x_math_combination_iterate_next
(cf_x_math_combination_t *combination);

void cf_x_math_combination_iterate_start(cf_x_math_combination_t *combination);

#endif
