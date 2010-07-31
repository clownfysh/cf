#include "x/core/uuid.h"

/*
x_core_bool_t x_core_uuid_equal(void *uuid_a_object, void *uuid_b_object)
{
  assert(uuid_a_object);
  assert(uuid_b_object);
  unsigned char *uuid_a = uuid_a_object;
  unsigned char *uuid_b = uuid_b_object;
  x_core_bool_t equal = x_core_bool_true;
  unsigned char i;

  for (i = 0; i < 16; i++) {
    if (*(uuid_a + i) != *(uuid_b + i)) {
      equal = x_core_bool_false;
      break;
    }
  }

  return equal;
}
*/

unsigned long x_core_uuid_mod(void *uuid_object, unsigned long divisor)
{
  assert(uuid_object);
  unsigned char *uuid = uuid_object;
  unsigned long dividend;
  unsigned long remainder;

  dividend = *((unsigned long *) (uuid + 12));
  remainder = dividend % divisor;
  /*  printf("%lu::%lu\n", dividend, remainder);  */

  return remainder;
}
