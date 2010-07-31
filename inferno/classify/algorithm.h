#ifndef cf_inferno_classify_algorithm_h
#define cf_inferno_classify_algorithm_h

#include "cf/x/core/bool.h"

/*!
 * identifier for a particular classify algorithm.  used by the
 * inhesion::classify system.
 */
enum cf_inferno_classify_algorithm_t {
  CF_INFERNO_CLASSIFY_ALGORITHM_ARCHETYPE,
};
typedef enum cf_inferno_classify_algorithm_t cf_inferno_classify_algorithm_t;

/*!
 * encode a string into a classify algorithm identifier.
 *
 * \param string the string representing the classify algorithm, to decode.
 *        one of {"archetype"}.
 * \param algorithm pointer to an cf_inferno_classify_algorithm_t to set.  if this
 *        function returns cf_x_core_bool_true, the algorithm pointed to by this
 *        parameter will be set to a valid algorithm.
 *
 * \return success or failure.
 */
cf_x_core_bool_t cf_inferno_classify_algorithm_get_from_string(char *string,
    cf_inferno_classify_algorithm_t *algorithm);

/*!
 * decode a classify algorithm identifier as a string.
 *
 * \param algorithm the algorithm you want represented as a string.
 *
 * \return a string representing the specified classify algorithm, or NULL if a
 *         bogus algorithm identifier was supplied.
 */
char *cf_inferno_classify_algorithm_get_string(cf_inferno_classify_algorithm_t algorithm);

#endif
