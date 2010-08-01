#include "cf/inferno/search/isearch.h"

void cf_inferno_searcx_isearch_init(cf_inferno_searcx_isearch_t *isearch,
    cf_inferno_searcx_isearch_create_f create, cf_inferno_searcx_isearch_destroy_f destroy,
    cf_inferno_searcx_isearch_get_solutions_copy_f get_solutions_copy,
    cf_inferno_searcx_isearch_searcx_f search)
{
  assert(isearch);

  isearch->create = create;
  isearch->destroy = destroy;
  isearch->get_solutions_copy = get_solutions_copy;
  isearch->search = search;
}
