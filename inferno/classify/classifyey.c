#include "inferno/classify/classifyey.h"
#include "x/core/tools.h"

void inferno_classify_classifyey_free
(inferno_classify_classifyey_t *classifyey) {}

void inferno_classify_classifyey_init
(inferno_classify_classifyey_t *classifyey,
    inferno_classify_classifyey_create_f create,
    inferno_classify_classifyey_destroy_f destroy,
    inferno_classify_classifyey_classify_object_f classify_object,
    inferno_classify_classifyey_learn_f learn,
    inferno_classify_classifyey_observe_object_f observe_object)
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
