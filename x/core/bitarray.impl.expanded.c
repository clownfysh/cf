#include "cf/x/core/bitarray.h"
#include "cf/x/core/long.h"
#include "cf/x/core/tools.h"

struct cf_x_core_bitarray_t {
  unsigned long array_size;
  cf_x_core_bit_t *array;
};

int cf_x_core_bitarray_compare(void *bitarray_object_a,
    void *bitarray_object_b)
{
  assert(bitarray_object_a);
  assert(bitarray_object_b);
  cf_x_core_bitarray_t *bitarray_a;
  cf_x_core_bitarray_t *bitarray_b;
  int compare_result;
  unsigned long each_bit;
  cf_x_core_bit_t bit_a;
  cf_x_core_bit_t bit_b;

  bitarray_a = bitarray_object_a;
  bitarray_b = bitarray_object_b;

  compare_result = cf_x_core_long_compare(&bitarray_a->array_size,
      &bitarray_b->array_size);
  if (0 == compare_result) {
    for (each_bit = 0; each_bit < bitarray_a->array_size; each_bit++) {
      bit_a = *(bitarray_a->array + each_bit);
      bit_b = *(bitarray_b->array + each_bit);
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

void *cf_x_core_bitarray_copy(void *bitarray_object)
{
  assert(bitarray_object);
  cf_x_core_bitarray_t *bitarray;
  cf_x_core_bitarray_t *bitarray_copy;
  unsigned long index;

  bitarray = bitarray_object;

  bitarray_copy = cf_x_core_bitarray_create(bitarray->array_size);
  if (bitarray_copy) {
    for (index = 0; index < bitarray->array_size; index++) {
      *(bitarray_copy->array + index) = *(bitarray->array + index);
    }
  } else {
    cf_x_core_trace("x_core_bitarray_create");
  }

  return bitarray_copy;
}

cf_x_core_bitarray_t *cf_x_core_bitarray_create(unsigned long size)
{
  assert(size > 0);
  cf_x_core_bitarray_t *bitarray;
  unsigned long array_memory_size;

  bitarray = malloc(sizeof *bitarray);
  if (bitarray) {
    array_memory_size = size * sizeof(cf_x_core_bit_t);
    bitarray->array = malloc(array_memory_size);
    if (bitarray->array) {
      memset(bitarray->array, '\0', array_memory_size);
      bitarray->array_size = size;
    } else {
      cf_x_core_trace("malloc");
      free(bitarray);
      bitarray = NULL;
    }
  } else {
    cf_x_core_trace("malloc");
  }

  return bitarray;
}

void cf_x_core_bitarray_destroy(void *bitarray_object)
{
  assert(bitarray_object);
  cf_x_core_bitarray_t *bitarray;

  bitarray = bitarray_object;

  free(bitarray->array);
  free(bitarray);

}

cf_x_core_bit_t cf_x_core_bitarray_get_bit_actual(cf_x_core_bitarray_t *bitarray,
    unsigned long index)
{
  return *(bitarray->array + index);
}

void cf_x_core_bitarray_set_all(cf_x_core_bitarray_t *bitarray)
{
  memset(bitarray->array, 1, bitarray->array_size * sizeof(cf_x_core_bit_t));
}

void cf_x_core_bitarray_set_bit_actual(cf_x_core_bitarray_t *bitarray,
    unsigned long index, cf_x_core_bit_t value)
{
  *(bitarray->array + index) = value;
}

unsigned long cf_x_core_bitarray_get_size(cf_x_core_bitarray_t *bitarray)
{
  return bitarray->array_size;
}

unsigned long cf_x_core_bitarray_get_memory_size_bytes
(cf_x_core_bitarray_t *bitarray)
{
  assert(bitarray);
  unsigned long size;

  size = sizeof *bitarray;
  size += bitarray->array_size * sizeof(cf_x_core_bit_t);

  return size;
}

void cf_x_core_bitarray_unset_all(cf_x_core_bitarray_t *bitarray)
{
  memset(bitarray->array, 0, bitarray->array_size * sizeof(cf_x_core_bit_t));
}
