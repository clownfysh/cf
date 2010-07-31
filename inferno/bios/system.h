#ifndef cf_inferno_bios_system_h
#define cf_inferno_bios_system_h

#include "cf/inferno/bios/declarations.h"
#include "cf/inferno/search/searchey.h"
#include "cf/inferno/search/tools.h"

void *cf_inferno_bios_system_create(cf_inferno_core_score_solution_f score_solution,
    cf_inferno_core_goal_t goal, void *context, cf_x_case_array_t *initial_solutions,
    cf_x_audit_log_t *log);

void cf_inferno_bios_system_destroy(void *system_object);

cf_inferno_box_system_t *cf_inferno_bios_system_get_box(cf_inferno_bios_system_t *system);

void *cf_inferno_bios_system_get_context(cf_inferno_bios_system_t *system);

cf_inferno_core_goal_t cf_inferno_bios_system_get_goal(cf_inferno_bios_system_t *system);

cf_x_audit_log_t *cf_inferno_bios_system_get_log(cf_inferno_bios_system_t *system);

cf_inferno_core_score_solution_f cf_inferno_bios_system_get_score_solution
(cf_inferno_bios_system_t *system);

cf_x_case_array_t *cf_inferno_bios_system_get_solutions_copy(void *system_object,
    unsigned short max_solution_count);

void cf_inferno_bios_system_init_searchey(cf_inferno_searcx_searchey_t *searchey);

void cf_inferno_bios_system_search(void *system_object);

#endif
