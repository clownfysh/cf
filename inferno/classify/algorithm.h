#ifndef inferno_classify_algorithm_h
#define inferno_classify_algorithm_h

#include "x/core/bool.h"

/*!
 * identifier for a particular classify algorithm.  used by the
 * inhesion::classify system.
 */
enum inferno_classify_algorithm_t {
  INFERNO_CLASSIFY_ALGORITHM_ARCHETYPE,
};
typedef enum inferno_classify_algorithm_t inferno_classify_algorithm_t;

/*!
 * encode a string into a classify algorithm identifier.
 *
 * \param string the string representing the classify algorithm, to decode.
 *        one of {"archetype"}.
 * \param algorithm pointer to an inferno_classify_algorithm_t to set.  if this
 *        function returns x_core_bool_true, the algorithm pointed to by this
 *        parameter will be set to a valid algorithm.
 *
 * \return success or failure.
 */
x_core_bool_t inferno_classify_algorithm_get_from_string(char *string,
    inferno_classify_algorithm_t *algorithm);

/*!
 * decode a classify algorithm identifier as a string.
 *
 * \param algorithm the algorithm you want represented as a string.
 *
 * \return a string representing the specified classify algorithm, or NULL if a
 *         bogus algorithm identifier was supplied.
 */
char *inferno_classify_algorithm_get_string(inferno_classify_algorithm_t algorithm);

#endif
