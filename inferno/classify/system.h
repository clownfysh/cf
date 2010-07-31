#ifndef cf_inferno_classify_system_h
#define cf_inferno_classify_system_h

#include "cf/x/core/log.h"
#include "cf/x/case/array.h"
#include "cf/x/core/bit.h"
#include "cf/x/core/bitarray.h"
#include "cf/x/core/tools.h"
#include "cf/inferno/core/goal.h"
#include "cf/inferno/classify/algorithm.h"

struct cf_inferno_classify_system_t;
typedef struct cf_inferno_classify_system_t cf_inferno_classify_system_t;

/*!
 * create an inhesion::classify system.  the resulting pointer is a context
 * variable that other classify methods use.  when you're finished using the
 * classification system, call cf_inferno_classify_system_destroy() to release
 * resources the classify is using.
 *
 * \param classified_objects an array of cf_x_core_bitarray_t *s, each one
 *        representing an object that has already been classified.  the objects
 *        should all be of the same size in bits, and they need to contain at
 *        least two bits.  the last bit in each object is the a-priori
 *        classification of that object.  these objects are observed by the
 *        classify system just as those passed to the observe_object() and
 *        observe_objects() functions.  when you call create(), you should pass
 *        in a sample size that represents the sample size inhesion should
 *        consider relevant for decision-making...in your classification
 *        problem, how many objects should be taken into account when making a
 *        classification?  the answer to that question should be the
 *        approximate size of the classified_objects array passed into this
 *        function.
 * \param algorithm the classification algorithm to use
 * \param log a log for the classify system to use when reporting its output
 *
 * \return context variable for the classify, or NULL if not successful
 */
cf_inferno_classify_system_t *cf_inferno_classify_system_create
(cf_x_case_array_t *classified_objects, cf_inferno_classify_algorithm_t algorithm,
    cf_x_core_log_t *log);

/*!
 * destroy a classify system.  free up resources the classify is using.
 *
 * \param system pointer to the classify system to destroy
 */
void cf_inferno_classify_system_destroy(cf_inferno_classify_system_t *system);

/*!
 * classify one unknown object.
 *
 * \param system the classify system to use
 * \param object the unknown object.  the object is assumed not to contain any
 *        classification bit...it is assumed to be in the same format as the
 *        training objects that have been passed in, except without the
 *        trailing classification bit.
 *
 * \return the classification of the object
 */
cf_x_core_bit_t cf_inferno_classify_system_classify_object(cf_inferno_classify_system_t *system,
    cf_x_core_bitarray_t *object);

/*!
 * classify an array of unknown objects.
 *
 * \param system the classify system to use
 * \param objects an array of cf_x_core_bitarray_t *s, each representing an
 *        unclassified object
 *
 * \return an array of cf_x_core_bit_t values.  each is the classification of the
 *         corresponding input object.  if you pass in 100 objects, this will
 *         return an array of 100 classifications.
 */
cf_x_case_array_t *cf_inferno_classify_system_classify_objects
(cf_inferno_classify_system_t *system, cf_x_case_array_t *objects);

/*!
 * spend some time learning about observed objects.  call this when you want to
 * use some processing/time resources to study the objects passed in to the
 * create() method of this system and passed in via the observe() method of
 * this system.
 *
 * \param system the classify system to use
 * \param max_wall_time_microseconds the maximum amount of time to spend
 *        learning.  there will be some minimum amount of time this method
 *        takes to complete.  above that, if the max_wall_time_microseconds
 *        value is greater than that minimum amount of time, this value will be
 *        used to cap the time spent in this call.
 *
 * \return the success of the call.
 */
cf_x_core_bool_t cf_inferno_classify_system_learn(cf_inferno_classify_system_t *system,
    unsigned long max_wall_time_microseconds);

/*!
 * observe an additional object.  at any time after the call to create(),
 * you can call this method to alert the classify system to the existence of
 * an additional object that has been classified outside of this system.  this
 * method can be called for an object that has already been seen by the
 * inhesion::classify system, and can be called for an object that has already
 * been classified by classify_object() or classify_objects().  in fact, if,
 * after getting a classification on an unknown object from inhesion, you get
 * feedback on the accuracy of that classification from outside of inhesion,
 * you should call this method, or observe_objects(), to give inhesion feedback
 * on objects it has already classified.  the objects passed to the create()
 * method of this system are also observed.
 *
 * \param system the classify system to use
 * \param classified_object an object for inhesion to observe
 *
 * \return the success of the call
 */
cf_x_core_bool_t cf_inferno_classify_system_observe_object(cf_inferno_classify_system_t *system,
    cf_x_core_bitarray_t *classified_object);

/*!
 * observe additional objects.  like observe_object() except for observing many
 * objects with one function call.
 *
 * \param system the classify system to use
 * \param classified_objects an array of objects for inhesion::classify to
 *        observe.  each object in the array is an cf_x_core_bitarray_t *.
 *
 * \return the success of the call
 */
cf_x_core_bool_t cf_inferno_classify_system_observe_objects(cf_inferno_classify_system_t *system,
    cf_x_case_array_t *classified_objects);

#endif
