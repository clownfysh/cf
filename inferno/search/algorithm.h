#ifndef cf_inferno_search_algorithm_h
#define cf_inferno_search_algorithm_h

#include "cf/x/core/bool.h"

/*!
 * identifier for a particular search algorithm.  used by the inhesion::search
 * system.
 */
enum cf_inferno_search_algorithm_t {
  CF_INFERNO_SEARCH_ALGORITHM_BIOS,
  CF_INFERNO_SEARCH_ALGORITHM_COR3,
  CF_INFERNO_SEARCH_ALGORITHM_EOS,
  CF_INFERNO_SEARCH_ALGORITHM_GENETIC,
};
typedef enum cf_inferno_search_algorithm_t cf_inferno_search_algorithm_t;

/*!
 * encode a string into a search algorithm identifier.
 *
 * \param string the string representing the search algorithm, to decode.  one
 *        of {"bios","cor3","eos","genetic"}.
 * \param algorithm pointer to an cf_inferno_search_algorithm_t to set.  if this
 *        function returns cf_x_core_bool_true, the algorithm pointed to by this
 *        parameter will be set to a valid algorithm.
 *
 * \return success or failure.
 */
cf_x_core_bool_t cf_inferno_search_algorithm_get_from_string(char *string,
    cf_inferno_search_algorithm_t *algorithm);

/*!
 * decode a search algorithm identifier as a string.
 *
 * \param algorithm the algorithm you want represented as a string.
 *
 * \return a string representing the specified search algorithm, or NULL if a
 *         bogus algorithm identifier was supplied.
 */
char *cf_inferno_search_algorithm_get_string(cf_inferno_search_algorithm_t algorithm);

#endif
