#ifndef x_math_combination_h
#define x_math_combination_h

#include "x/case/set.h"

struct x_math_combination_t;
typedef struct x_math_combination_t x_math_combination_t;

x_math_combination_t *x_math_combination_create(x_case_set_t *set);

void x_math_combination_destroy(x_math_combination_t *combination);

x_case_set_t *x_math_combination_iterate_next
(x_math_combination_t *combination);

void x_math_combination_iterate_start(x_math_combination_t *combination);

#endif
