#include "cf/inferno/classify/iclassify.h"
#include "cf/x/core/tools.h"

void cf_inferno_classify_iclassify_free
(cf_inferno_classify_iclassify_t *iclassify) {}

void cf_inferno_classify_iclassify_init
(cf_inferno_classify_iclassify_t *iclassify,
    cf_inferno_classify_iclassify_create_f create,
    cf_inferno_classify_iclassify_destroy_f destroy,
    cf_inferno_classify_iclassify_classify_object_f classify_object,
    cf_inferno_classify_iclassify_learn_f learn,
    cf_inferno_classify_iclassify_observe_object_f observe_object)
{
  assert(iclassify);
  assert(create);
  assert(destroy);
  assert(classify_object);
  assert(learn);
  assert(observe_object);

  iclassify->create = create;
  iclassify->destroy = destroy;
  iclassify->classify_object = classify_object;
  iclassify->learn = learn;
  iclassify->observe_object = observe_object;
}
