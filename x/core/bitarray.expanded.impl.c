#include "x/core/bitarray.h"
#include "x/core/long.h"
#include "x/core/tools.h"

struct x_core_bitarray_t {
  unsigned long array_size;
  x_core_bit_t *array;
};

int x_core_bitarray_compare(void *bitarray_object_a,
    void *bitarray_object_b)
{
  assert(bitarray_object_a);
  assert(bitarray_object_b);
  x_core_bitarray_t *bitarray_a;
  x_core_bitarray_t *bitarray_b;
  int compare_result;
  unsigned long eacx_bit;
  x_core_bit_t bit_a;
  x_core_bit_t bit_b;

  bitarray_a = bitarray_object_a;
  bitarray_b = bitarray_object_b;

  compare_result = x_core_long_compare(&bitarray_a->array_size,
      &bitarray_b->array_size);
  if (0 == compare_result) {
    for (eacx_bit = 0; eacx_bit < bitarray_a->array_size; eacx_bit++) {
      bit_a = *(bitarray_a->array + eacx_bit);
      bit_b = *(bitarray_b->array + eacx_bit);
      if (bit_a < bit_b) {
        compare_result = -1;
        break;
      } else if (bit_a > bit_b) {
        compare_result = 1;
        break;
      }
    }
  }

  return compare_result;
}

void *x_core_bitarray_copy(void *bitarray_object)
{
  assert(bitarray_object);
  x_core_bitarray_t *bitarray;
  x_core_bitarray_t *bitarray_copy;
  unsigned long index;

  bitarray = bitarray_object;

  bitarray_copy = x_core_bitarray_create(bitarray->array_size);
  if (bitarray_copy) {
    for (index = 0; index < bitarray->array_size; index++) {
      *(bitarray_copy->array + index) = *(bitarray->array + index);
    }
  } else {
    x_core_trace("x_core_bitarray_create");
  }

  return bitarray_copy;
}

x_core_bitarray_t *x_core_bitarray_create(unsigned long size)
{
  assert(size > 0);
  x_core_bitarray_t *bitarray;
  unsigned long array_memory_size;

  bitarray = malloc(sizeof *bitarray);
  if (bitarray) {
    array_memory_size = size * sizeof(x_core_bit_t);
    bitarray->array = malloc(array_memory_size);
    if (bitarray->array) {
      memset(bitarray->array, '\0', array_memory_size);
      bitarray->array_size = size;
    } else {
      x_core_trace("malloc");
      free(bitarray);
      bitarray = NULL;
    }
  } else {
    x_core_trace("malloc");
  }

  return bitarray;
}

void x_core_bitarray_destroy(void *bitarray_object)
{
  assert(bitarray_object);
  x_core_bitarray_t *bitarray;

  bitarray = bitarray_object;

  free(bitarray->array);
  free(bitarray);

}

x_core_bit_t x_core_bitarray_get_bit_actual(x_core_bitarray_t *bitarray,
    unsigned long index)
{
  return *(bitarray->array + index);
}

void x_core_bitarray_set_all(x_core_bitarray_t *bitarray)
{
  memset(bitarray->array, 1, bitarray->array_size * sizeof(x_core_bit_t));
}

void x_core_bitarray_set_bit_actual(x_core_bitarray_t *bitarray,
    unsigned long index, x_core_bit_t value)
{
  *(bitarray->array + index) = value;
}

unsigned long x_core_bitarray_get_size(x_core_bitarray_t *bitarray)
{
  return bitarray->array_size;
}

unsigned long x_core_bitarray_get_memory_size_bytes
(x_core_bitarray_t *bitarray)
{
  assert(bitarray);
  unsigned long size;

  size = sizeof *bitarray;
  size += bitarray->array_size * sizeof(x_core_bit_t);

  return size;
}

void x_core_bitarray_unset_all(x_core_bitarray_t *bitarray)
{
  memset(bitarray->array, 0, bitarray->array_size * sizeof(x_core_bit_t));
}
