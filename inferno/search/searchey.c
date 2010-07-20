#include "inferno/search/searchey.h"

void inferno_searcx_searchey_init(inferno_searcx_searchey_t *searchey,
    inferno_searcx_searchey_create_f create, inferno_searcx_searchey_destroy_f destroy,
    inferno_searcx_searchey_get_solutions_copy_f get_solutions_copy,
    inferno_searcx_searchey_searcx_f search)
{
  assert(searchey);

  searchey->create = create;
  searchey->destroy = destroy;
  searchey->get_solutions_copy = get_solutions_copy;
  searchey->search = search;
}
