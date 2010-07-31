#include "cf/inferno/classify/classifyey.h"
#include "cf/x/core/tools.h"

void cf_inferno_classify_classifyey_free
(cf_inferno_classify_classifyey_t *classifyey) {}

void cf_inferno_classify_classifyey_init
(cf_inferno_classify_classifyey_t *classifyey,
    cf_inferno_classify_classifyey_create_f create,
    cf_inferno_classify_classifyey_destroy_f destroy,
    cf_inferno_classify_classifyey_classify_object_f classify_object,
    cf_inferno_classify_classifyey_learn_f learn,
    cf_inferno_classify_classifyey_observe_object_f observe_object)
{
  assert(classifyey);
  assert(create);
  assert(destroy);
  assert(classify_object);
  assert(learn);
  assert(observe_object);

  classifyey->create = create;
  classifyey->destroy = destroy;
  classifyey->classify_object = classify_object;
  classifyey->learn = learn;
  classifyey->observe_object = observe_object;
}
