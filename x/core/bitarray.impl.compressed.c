#include "x/core/bitarray.h"
#include "x/core/tools.h"

struct x_core_bitarray_t {
  unsigned long array_size;

  unsigned char *array;
  unsigned long bit_count;
};

struct x_core_bitarray_array_position_t {
  unsigned long char_index;
  unsigned short char_bit_index;
};
typedef struct x_core_bitarray_array_position_t
x_core_bitarray_array_position_t;

static void calculate_array_postion(unsigned long bit_index,
    x_core_bitarray_array_position_t *array_position);

void calculate_array_postion(unsigned long bit_index,
    x_core_bitarray_array_position_t *array_position)
{
  assert(array_position);
  array_position->char_index = bit_index / 8;
  array_position->char_bit_index = bit_index % 8;
}

int x_core_bitarray_compare(void *bitarray_object_a,
    void *bitarray_object_b)
{
  x_core_trace_exit("TODO: implement");
}

void *x_core_bitarray_copy(void *bitarray_object)
{
  assert(bitarray_object);
  x_core_bitarray_t *bitarray;
  x_core_bitarray_t *bitarray_copy;

  bitarray = bitarray_object;

  bitarray_copy = x_core_bitarray_create(bitarray->bit_count);
  if (bitarray_copy) {
    memcpy(bitarray_copy->array, bitarray->array, bitarray->array_size);
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
    array_memory_size = (size / 8) + 1;
    bitarray->array = malloc(array_memory_size);
    if (bitarray->array) {
      memset(bitarray->array, '\0', array_memory_size);
      bitarray->array_size = array_memory_size;
      bitarray->bit_count = size;
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
  assert(bitarray);
  x_core_bitarray_array_position_t array_position;
  x_core_bit_t bit;
  unsigned char *c;

  calculate_array_postion(index, &array_position);
  c = bitarray->array + array_position.char_index;
  bit = get_bit_from_unsigned_char(c, array_position.char_bit_index);

  return bit;
}

unsigned long x_core_bitarray_get_size(x_core_bitarray_t *bitarray)
{
  return bitarray->bit_count;
}

void x_core_bitarray_set_all(x_core_bitarray_t *bitarray)
{
  memset(bitarray->array, 255, bitarray->array_size);
}

void x_core_bitarray_set_bit_actual(x_core_bitarray_t *bitarray,
    unsigned long index, x_core_bit_t value)
{
  assert(bitarray);
  x_core_bitarray_array_position_t array_position;
  unsigned char *c;

  calculate_array_postion(index, &array_position);
  c = bitarray->array + array_position.char_index;
  set_bit_in_unsigned_char(c, array_position.char_bit_index, value);
}

unsigned long x_core_bitarray_get_memory_size_bytes
(x_core_bitarray_t *bitarray)
{
  assert(bitarray);
  unsigned long size;

  size = sizeof *bitarray;
  size += (bitarray->array_size / 8) + 1;

  return size;
}

void x_core_bitarray_unset_all(x_core_bitarray_t *bitarray)
{
  memset(bitarray->array, 0, bitarray->array_size);
}
