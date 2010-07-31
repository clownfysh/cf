#include "cf/x/case/list.h"
#include "cf/x/case/set.h"
#include "cf/x/core/content.h"
#include "cf/x/core/imessage.h"
#include "cf/x/core/nameobject.h"
#include "cf/x/core/string.h"
#include "cf/x/core/tools.h"
#include "cf/x/core/types.h"
#include "cf/x/net/http/message.h"

struct cf_x_net_http_message_t {
  int client_socket;

  cf_x_net_hypermethod_t hypermethod;
  cf_x_net_hyperstatus_t hyperstatus;
  char *resource_path;
  cf_x_net_http_version_t hyperversion;
  cf_x_case_set_t *hyperheaders;
  char *body;
  unsigned long body_size;

  cf_x_core_content_t content_type;
  char *resource_name;
  cf_x_case_set_t *pri_parameters;

  cf_x_core_iobject_t nameobject_iobject;
  cf_x_core_iobject_t uuid_iobject;
};

static cf_x_core_bool_t cf_x_net_http_message_create_pri
(cf_x_net_http_message_t *hypermessage);

static void cf_x_net_http_message_create_rollback
(cf_x_net_http_message_t *hypermessage);

/*
  TODO: simplify
*/
cf_x_net_http_message_t *cf_x_net_http_message_create(int client_socket,
    cf_x_net_hypermethod_t hypermethod, cf_x_net_hyperstatus_t hyperstatus,
    char *resource_path, cf_x_net_http_version_t hyperversion,
    cf_x_case_set_t *hyperheaders)
{
  cf_x_net_http_message_t *hypermessage;
  cf_x_core_bool_t so_far_so_good;

  hypermessage = malloc(sizeof *hypermessage);
  if (hypermessage) {
    hypermessage->client_socket = client_socket;
    hypermessage->hypermethod = hypermethod;
    hypermessage->hyperstatus = hyperstatus;
    hypermessage->hyperversion = hyperversion;
    hypermessage->body = NULL;
    hypermessage->body_size = 0;
    hypermessage->resource_name = NULL;
    hypermessage->pri_parameters = NULL;
    cf_x_core_uuid_init_iobject(&hypermessage->uuid_iobject);

    if (resource_path) {
      hypermessage->resource_path = strdup(resource_path);
      if (hypermessage->resource_path) {
        so_far_so_good = cf_x_core_bool_true;
      } else {
        cf_x_core_trace("strdup");
        so_far_so_good = cf_x_core_bool_false;
      }
    } else {
      hypermessage->resource_path = NULL;
      so_far_so_good = cf_x_core_bool_true;
    }

  } else {
    so_far_so_good = cf_x_core_bool_false;
    cf_x_core_trace("malloc");
  }

  if (so_far_so_good) {
    if (hyperheaders) {
      hypermessage->hyperheaders = cf_x_case_set_copy(hyperheaders);
    } else {
      cf_x_core_nameobject_init_iobject(&hypermessage->nameobject_iobject);
      hypermessage->hyperheaders
        = cf_x_case_set_create(&hypermessage->nameobject_iobject);
      if (!hypermessage->hyperheaders) {
        so_far_so_good = cf_x_core_bool_false;
        cf_x_core_trace("x_case_set_create");
      }
    }
  }

  if (so_far_so_good) {
    if (hypermessage->resource_path) {
      if (!cf_x_net_http_message_create_pri(hypermessage)) {
        so_far_so_good = cf_x_core_bool_false;
      }
    }
  }

  if (!so_far_so_good && hypermessage) {
    cf_x_net_http_message_create_rollback(hypermessage);
    hypermessage = NULL;
  }

  assert(!hypermessage || hypermessage->hyperheaders);
  return hypermessage;
}

cf_x_core_bool_t cf_x_net_http_message_create_pri(cf_x_net_http_message_t *hypermessage)
{
  assert(hypermessage->resource_path);
  cf_x_core_bool_t success;
  char *parameters;
  char *parameter;
  char *name;
  char *value;
  char *parameter_context;
  char *nameobject_context;
  cf_x_core_nameobject_t *nameobject;
  char *resource_name;

  nameobject_context = NULL;

  resource_name = strtok(hypermessage->resource_path, "?");
  hypermessage->resource_name = strdup(resource_name);
  parameters = strtok(NULL, "?");
  if (parameters) {
    hypermessage->pri_parameters
      = cf_x_case_set_create(&hypermessage->nameobject_iobject);
    if (hypermessage->pri_parameters) {
      success = cf_x_core_bool_true;
      parameter = strtok_r(parameters, "&", &parameter_context);
      while (parameter) {
        name = strtok_r(parameter, "=", &nameobject_context);
        value = strtok_r(NULL, "=", &nameobject_context);
        if (name && value) {
          nameobject = cf_x_core_nameobject_create(name, value,
              cf_x_core_string_copy, cf_x_core_string_destroy,
              cf_x_core_string_get_as_string);
          if (nameobject) {
            if (!cf_x_case_set_add
                (hypermessage->pri_parameters, nameobject)) {
              cf_x_core_nameobject_destroy(nameobject);
            }
          } else {
            cf_x_core_trace("x_core_nameobject_create");
            success = cf_x_core_bool_false;
          }
        }
        parameter = strtok_r(NULL, "&", &parameter_context);
      }
    } else {
      cf_x_core_trace("x_case_set_create");
      success = cf_x_core_bool_false;
    }
  } else {
    hypermessage->pri_parameters = NULL;
    success = cf_x_core_bool_true;
  }

  return success;
}

void cf_x_net_http_message_create_rollback(cf_x_net_http_message_t *hypermessage)
{
  assert(hypermessage);

  if (hypermessage->resource_path) {
    free(hypermessage->resource_path);
  }
  if (hypermessage->hyperheaders) {
    cf_x_case_set_destroy(hypermessage->hyperheaders);
  }
  if (hypermessage->body) {
    free(hypermessage->body);
  }
  free(hypermessage);
}

void cf_x_net_http_message_destroy(void *hypermessage_object)
{
  assert(hypermessage_object);
  cf_x_net_http_message_t *hypermessage;

  hypermessage = hypermessage_object;

  if (hypermessage->body) {
    free(hypermessage->body);
  }

  if (hypermessage->resource_path) {
    free(hypermessage->resource_path);
  }

  if (hypermessage->resource_name) {
    free(hypermessage->resource_name);
  }

  if (hypermessage->pri_parameters) {
    cf_x_case_set_destroy(hypermessage->pri_parameters);
  }

  cf_x_case_set_destroy(hypermessage->hyperheaders);

  free(hypermessage);
}

char *cf_x_net_http_message_get_body(cf_x_net_http_message_t *hypermessage)
{
  return hypermessage->body;
}

unsigned long cf_x_net_http_message_get_body_size
(cf_x_net_http_message_t *hypermessage)
{
  return hypermessage->body_size;
}

int cf_x_net_http_message_get_client_socket(void *hypermessage_object)
{
  assert(hypermessage_object);
  cf_x_net_http_message_t *hypermessage;

  hypermessage = hypermessage_object;

  return hypermessage->client_socket;
}

cf_x_net_engine_id_t cf_x_net_http_message_get_engine_id
(void *message_object)
{
  return CF_X_NET_ENGINE_HYPER;
}

cf_x_core_nameobject_t *cf_x_net_http_message_get_hyperheader
(cf_x_net_http_message_t *hypermessage, char *header_name)
{
  assert(hypermessage);
  assert(header_name);
  cf_x_core_nameobject_t *decoy_header;
  cf_x_core_nameobject_t *found_header;

  decoy_header = cf_x_core_nameobject_create_decoy(header_name);
  found_header = cf_x_case_set_find
    (hypermessage->hyperheaders, decoy_header);

  return found_header;
}

cf_x_case_set_t *cf_x_net_http_message_get_hyperheaders
(cf_x_net_http_message_t *hypermessage)
{
  return hypermessage->hyperheaders;
}

cf_x_net_hypermethod_t cf_x_net_http_message_get_hypermethod
(cf_x_net_http_message_t *hypermessage)
{
  return hypermessage->hypermethod;
}

cf_x_net_hyperstatus_t cf_x_net_http_message_get_hyperstatus
(cf_x_net_http_message_t *hypermessage)
{
  return hypermessage->hyperstatus;
}

cf_x_net_http_version_t cf_x_net_http_message_get_hyperversion
(cf_x_net_http_message_t *hypermessage)
{
  return hypermessage->hyperversion;
}

char *cf_x_net_http_message_get_resource_name(cf_x_net_http_message_t *hypermessage)
{
  return hypermessage->resource_name;
}

char *cf_x_net_http_message_get_pri_parameter(cf_x_net_http_message_t *hypermessage,
    char *parameter_name)
{
  char *parameter_value;
  cf_x_core_nameobject_t *nameobject_decoy;
  cf_x_core_nameobject_t *nameobject;

  if (hypermessage->pri_parameters) {
    nameobject_decoy = cf_x_core_nameobject_create_decoy(parameter_name);
    if (nameobject_decoy) {
      nameobject
        = cf_x_case_set_find(hypermessage->pri_parameters, nameobject_decoy);
      if (nameobject) {
        parameter_value = cf_x_core_nameobject_get_object(nameobject);
      } else {
        parameter_value = NULL;
      }
      cf_x_core_nameobject_destroy_decoy(nameobject_decoy);
    } else {
      cf_x_core_trace("x_core_nameobject_create_decoy");
      parameter_value = NULL;
    }
  } else {
    parameter_value = NULL;
  }

  return parameter_value;
}

double cf_x_net_http_message_get_pri_parameter_as_double
(cf_x_net_http_message_t *hypermessage, char *parameter_name)
{
  assert(hypermessage);
  assert(parameter_name);
  char *value_string;
  double value_double;

  value_double = 0.0;

  value_string
    = cf_x_net_http_message_get_pri_parameter(hypermessage, parameter_name);
  if (value_string) {
    value_double = atof(value_string);
    free(value_string);
  } else {
    cf_x_core_trace("x_net_hypermessage_get_pri_parameter");
  }

  return value_double;
}

cf_x_core_bool_t cf_x_net_http_message_get_pri_parameter_as_unsigned_long
(cf_x_net_http_message_t *hypermessage, char *parameter_name,
    unsigned long *value)
{
  assert(hypermessage);
  assert(parameter_name);
  assert(value);
  char *value_string;
  cf_x_core_bool_t success;

  value_string
    = cf_x_net_http_message_get_pri_parameter(hypermessage, parameter_name);
  if (value_string) {
    success = cf_x_core_bool_true;
    *value = atol(value_string);
  } else {
    success = cf_x_core_bool_false;
    cf_x_core_trace("x_net_hypermessage_get_pri_parameter");
  }

  return success;
}

cf_x_core_uuid_t *cf_x_net_http_message_get_pri_parameter_as_uuid
(cf_x_net_http_message_t *hypermessage, char *parameter_name)
{
  assert(hypermessage);
  assert(parameter_name);
  cf_x_core_uuid_t *uuid;
  char *uuid_string;

  uuid = NULL;

  uuid_string
    = cf_x_net_http_message_get_pri_parameter(hypermessage, parameter_name);
  if (uuid_string) {
    uuid = cf_x_core_uuid_create_from_string(uuid_string);
    if (!uuid) {
      cf_x_core_trace("x_core_uuid_create_from_string");
    }
  } else {
    cf_x_core_trace("x_net_hypermessage_get_pri_parameter");
  }

  return uuid;
}

cf_x_case_set_t *cf_x_net_http_message_get_pri_parameter_as_uuid_set
(cf_x_net_http_message_t *hypermessage, char *parameter_name)
{
  assert(hypermessage);
  assert(parameter_name);
  cf_x_core_uuid_t *uuid;
  char *uuid_string;
  char *parameter_string;
  cf_x_case_list_t *uuid_strings;
  cf_x_case_set_t *uuid_set;

  uuid_set = cf_x_case_set_create(&hypermessage->uuid_iobject);
  if (uuid_set) {
    parameter_string
      = cf_x_net_http_message_get_pri_parameter(hypermessage, parameter_name);
    if (parameter_string) {
      uuid_strings
        = cf_x_case_list_create_strings_from_string(parameter_string, ",");
      if (uuid_strings) {
        cf_x_case_list_iterate_start(uuid_strings);
        while ((uuid_string = cf_x_case_list_iterate_next(uuid_strings))) {
          uuid = cf_x_core_uuid_create_from_string(uuid_string);
          if (uuid) {
            if (!cf_x_case_set_find(uuid_set, uuid)) {
              if (!cf_x_case_set_add(uuid_set, uuid)) {
                cf_x_core_uuid_destroy(uuid);
                cf_x_core_trace("x_case_set_add");
              }
            } else {
              cf_x_core_uuid_destroy(uuid);
            }
          } else {
            cf_x_core_trace("x_core_uuid_create_from_string");
          }
        }
        cf_x_case_list_destroy(uuid_strings);
      } else {
        cf_x_core_trace("x_case_list_create_string_from_strings");
      }
    } else {
      cf_x_core_trace("x_net_hypermessage_get_pri_parameter");
    }
  } else {
    cf_x_core_trace("x_case_set_create");
  }

  return uuid_set;
}

cf_x_case_set_t *cf_x_net_http_message_get_pri_parameters
(cf_x_net_http_message_t *hypermessage)
{
  return hypermessage->pri_parameters;
}

char *cf_x_net_http_message_get_resource_path(cf_x_net_http_message_t *hypermessage)
{
  return hypermessage->resource_path;
}

unsigned long cf_x_net_http_message_get_type(void *message_object)
{
  return CF_X_CORE_IMESSAGE_NO_TYPE;
}

cf_x_core_bool_t cf_x_net_http_message_set_body(cf_x_net_http_message_t *hypermessage,
    cf_x_core_content_t content_type, char *body, unsigned long body_size)
{
  assert(hypermessage);
  assert(body);
  assert(body_size > 0);
  cf_x_core_bool_t success;
  char *content_type_name;
  char *body_size_string;

  content_type_name = cf_x_core_content_get_name(content_type);
  success = cf_x_core_bool_true;

  if (!cf_x_net_http_message_set_hyperheader
      (hypermessage, "Content-Type", content_type_name)) {
    success = cf_x_core_bool_false;
  }

  if (asprintf(&body_size_string, "%lu", body_size) != -1) {
    if (!cf_x_net_http_message_set_hyperheader
        (hypermessage, "Content-Length", body_size_string)) {
      success = cf_x_core_bool_false;
    }
    free(body_size_string);
  } else {
    cf_x_core_trace("asprintf");
  }

  if (success) {
    hypermessage->body = malloc(body_size);
    if (hypermessage->body) {
      memcpy(hypermessage->body, body, body_size);
      hypermessage->body_size = body_size;
    } else {
      cf_x_core_trace("malloc");
      success = cf_x_core_bool_false;
    }
  }

  return success;
}

cf_x_core_bool_t cf_x_net_http_message_set_hyperheader
(cf_x_net_http_message_t *hypermessage, char *header_name, char *header_value)
{
  assert(hypermessage);
  assert(header_name);
  assert(header_value);
  cf_x_core_bool_t success;
  cf_x_core_nameobject_t *nameobject;

  nameobject = cf_x_core_nameobject_create(header_name, header_value,
      cf_x_core_string_copy, cf_x_core_string_destroy, cf_x_core_string_get_as_string);
  if (nameobject) {
    if (cf_x_case_set_add(hypermessage->hyperheaders, nameobject)) {
      success = cf_x_core_bool_true;
    } else {
      success = cf_x_core_bool_false;
    }
  } else {
    success = cf_x_core_bool_false;
  }

  return success;
}
