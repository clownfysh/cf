#include "cf/inferno/search/searchey.h"

void cf_inferno_searcx_searchey_init(cf_inferno_searcx_searchey_t *searchey,
    cf_inferno_searcx_searchey_create_f create, cf_inferno_searcx_searchey_destroy_f destroy,
    cf_inferno_searcx_searchey_get_solutions_copy_f get_solutions_copy,
    cf_inferno_searcx_searchey_searcx_f search)
{
  assert(searchey);

  searchey->create = create;
  searchey->destroy = destroy;
  searchey->get_solutions_copy = get_solutions_copy;
  searchey->search = search;
}
