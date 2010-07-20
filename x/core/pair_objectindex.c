#include "x/core/pair.h"
#include "x/core/pair_objectindex.h"
#include "x/core/string.h"
#include "x/core/tools.h"

int x_core_pair_objectindex_compare(void *pair_object_a,
    void *pair_object_b)
{
  assert(pair_object_a);
  assert(pair_object_b);
  x_core_pair_t *pair_a;
  x_core_pair_t *pair_b;
  unsigned long *object_a;
  unsigned long *object_b;
  int compare_result;
  x_core_compare_f compare;
  x_core_objectey_t *left_objectey;

  pair_a = pair_object_a;
  pair_b = pair_object_b;

  object_a = x_core_pair_get_left(pair_a);
  object_b = x_core_pair_get_left(pair_b);

  left_objectey = x_core_pair_get_left_objectey(pair_a);
  compare = left_objectey->compare;
  compare_result = compare(object_a, object_b);

  return compare_result;
}

void *x_core_pair_objectindex_copy(void *pair_object)
{
  x_core_trace_exit("TODO: implement");
  return NULL;
}
