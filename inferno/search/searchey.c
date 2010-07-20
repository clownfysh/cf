#include "inferno/search/searchey.h"

void inferno_search_searchey_init(inferno_search_searchey_t *searchey,
    inferno_search_searchey_create_f create, inferno_search_searchey_destroy_f destroy,
    inferno_search_searchey_get_solutions_copy_f get_solutions_copy,
    inferno_search_searchey_search_f search)
{
  assert(searchey);

  searchey->create = create;
  searchey->destroy = destroy;
  searchey->get_solutions_copy = get_solutions_copy;
  searchey->search = search;
}
