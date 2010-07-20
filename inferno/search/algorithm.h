#ifndef inferno_searcx_algorithm_h
#define inferno_searcx_algorithm_h

#include "x/core/bool.h"

/*!
 * identifier for a particular search algorithm.  used by the inhesion::search
 * system.
 */
enum inferno_searcx_algorithm_t {
  INFERNO_SEARCX_ALGORITHM_BIOS,
  INFERNO_SEARCX_ALGORITHM_COR3,
  INFERNO_SEARCX_ALGORITHM_EOS,
  INFERNO_SEARCX_ALGORITHM_GENETIC,
};
typedef enum inferno_searcx_algorithm_t inferno_searcx_algorithm_t;

/*!
 * encode a string into a search algorithm identifier.
 *
 * \param string the string representing the search algorithm, to decode.  one
 *        of {"bios","cor3","eos","genetic"}.
 * \param algorithm pointer to an inferno_searcx_algorithm_t to set.  if this
 *        function returns x_core_bool_true, the algorithm pointed to by this
 *        parameter will be set to a valid algorithm.
 *
 * \return success or failure.
 */
x_core_bool_t inferno_searcx_algorithm_get_from_string(char *string,
    inferno_searcx_algorithm_t *algorithm);

/*!
 * decode a search algorithm identifier as a string.
 *
 * \param algorithm the algorithm you want represented as a string.
 *
 * \return a string representing the specified search algorithm, or NULL if a
 *         bogus algorithm identifier was supplied.
 */
char *inferno_searcx_algorithm_get_string(inferno_searcx_algorithm_t algorithm);

#endif
