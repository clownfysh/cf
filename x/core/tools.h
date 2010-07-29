#ifndef x_core_tools_h
#define x_core_tools_h

#include "x/core/standard.h"
#include "x/core/bit.h"
#include "x/core/types.h"

void x_core_do_nothing();

#define x_trace(message, ...) fprintf(stderr, message "() in %s:%d\n",  \
      ##__VA_ARGS__, __FILE__, __LINE__)

#define x_disable_test() printf("  %s disabled\n", argv[0]); return 0

#define x_core_get_bit(value, bit_index) ((value >> bit_index) & 1)

#define x_core_hash(string) x_core_hash_djb2_xor(string)

#define x_core_max(a, b) ((a > b) ? a : b)

#define x_core_min(a, b) ((a < b) ? a : b)

#define x_core_trace(message, ...) fprintf(stderr, message "() in %s:%d\n", \
      ##__VA_ARGS__, __FILE__, __LINE__)

#define x_core_trace_exit(message, ...) fprintf                         \
  (stderr, message"() in %s:%d\n", ##__VA_ARGS__, __FILE__, __LINE__);  \
  exit(22);

extern x_core_bool_t x_core_stop_requested;

unsigned long x_get_microseconds();

x_core_bool_t x_core_get_current_time_string(x_core_time_string_t time_string);

double x_core_gray_coin_toss();

unsigned long x_core_hash_djb2(char *string);

unsigned long x_core_hash_djb2_xor(char *string);

unsigned long x_core_hash_sdbm(char *string);

x_core_bool_t x_core_key_hit();

void x_core_note_maximum_double(double *maximum, double candidate);

void x_core_note_maximum_unsigned_long(unsigned long *maximum,
    unsigned long candidate);

double x_core_percentage(double part, double whole);

void x_core_request_stop();

void x_core_seed_random(unsigned long seed);

void x_core_seed_random_witx_time();

void x_core_set_bit_in_unsigned_char(unsigned char *c,
    unsigned short bit_index, x_core_bit_t bit_value);

void x_core_show_memory(void *location, unsigned short length);

char *x_core_string_append(char *original, const char *addition);

char *x_core_string_append_char(char *original, char addition);

char *x_core_string_append_multiple(char *original, ...);

char *x_core_string_append_n(char *original, char *addition,
    unsigned long addition_size);

char *x_core_substring(char *string, unsigned short max_length);

void x_core_terminal_block();

void x_core_terminal_nonblock();

/*!
 * determine if time is remaining, or has elapsed.
 *
 * \param start_time pointer to an already-initialized timeval containing the
 *        start time.
 * \param max_wall_time_microseconds the number of microseconds after which
 *        this function will consider that time is not remaining.
 *
 * \return whether or not time is remaining
 */
x_core_bool_t x_core_time_is_remaining_microseconds(struct timeval *start_time,
    unsigned long max_wall_time_microseconds);

unsigned short x_core_toss_coin();

void x_core_truncate_string(char *string, unsigned short max_length);

unsigned long x_core_wrap_index(long virtual_index, unsigned long range);

#endif
