#include "x/container/list.h"
#include "x/container/set.h"
#include "x/core/content.h"
#include "x/core/messagey.h"
#include "x/core/nameobject.h"
#include "x/core/string.h"
#include "x/core/tools.h"
#include "x/core/types.h"
#include "x/net/hypermessage.h"

struct x_net_hypermessage_t {
  int client_socket;

  x_net_hypermethod_t hypermethod;
  x_net_hyperstatus_t hyperstatus;
  char *resource_path;
  x_net_hyperversion_t hyperversion;
  x_container_set_t *hyperheaders;
  char *body;
  unsigned long body_size;

  x_core_content_t content_type;
  char *resource_name;
  x_container_set_t *pri_parameters;

  x_core_objectey_t nameobject_objectey;
  x_core_objectey_t uuid_objectey;
};

static x_core_bool_t x_net_hypermessage_create_pri
(x_net_hypermessage_t *hypermessage);

static void x_net_hypermessage_create_rollback
(x_net_hypermessage_t *hypermessage);

/*
  TODO: simplify
*/
x_net_hypermessage_t *x_net_hypermessage_create(int client_socket,
    x_net_hypermethod_t hypermethod, x_net_hyperstatus_t hyperstatus,
    char *resource_path, x_net_hyperversion_t hyperversion,
    x_container_set_t *hyperheaders)
{
  x_net_hypermessage_t *hypermessage;
  x_core_bool_t so_far_so_good;

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
    x_core_uuid_init_objectey(&hypermessage->uuid_objectey);

    if (resource_path) {
      hypermessage->resource_path = strdup(resource_path);
      if (hypermessage->resource_path) {
        so_far_so_good = x_core_bool_true;
      } else {
        x_core_trace("strdup");
        so_far_so_good = x_core_bool_false;
      }
    } else {
      hypermessage->resource_path = NULL;
      so_far_so_good = x_core_bool_true;
    }

  } else {
    so_far_so_good = x_core_bool_false;
    x_core_trace("malloc");
  }

  if (so_far_so_good) {
    if (hyperheaders) {
      hypermessage->hyperheaders = x_container_set_copy(hyperheaders);
    } else {
      x_core_nameobject_init_objectey(&hypermessage->nameobject_objectey);
      hypermessage->hyperheaders
        = x_container_set_create(&hypermessage->nameobject_objectey);
      if (!hypermessage->hyperheaders) {
        so_far_so_good = x_core_bool_false;
        x_core_trace("x_container_set_create");
      }
    }
  }

  if (so_far_so_good) {
    if (hypermessage->resource_path) {
      if (!x_net_hypermessage_create_pri(hypermessage)) {
        so_far_so_good = x_core_bool_false;
      }
    }
  }

  if (!so_far_so_good && hypermessage) {
    x_net_hypermessage_create_rollback(hypermessage);
    hypermessage = NULL;
  }

  assert(!hypermessage || hypermessage->hyperheaders);
  return hypermessage;
}

x_core_bool_t x_net_hypermessage_create_pri(x_net_hypermessage_t *hypermessage)
{
  assert(hypermessage->resource_path);
  x_core_bool_t success;
  char *parameters;
  char *parameter;
  char *name;
  char *value;
  char *parameter_context;
  char *nameobject_context;
  x_core_nameobject_t *nameobject;
  char *resource_name;

  nameobject_context = NULL;

  resource_name = strtok(hypermessage->resource_path, "?");
  hypermessage->resource_name = strdup(resource_name);
  parameters = strtok(NULL, "?");
  if (parameters) {
    hypermessage->pri_parameters
      = x_container_set_create(&hypermessage->nameobject_objectey);
    if (hypermessage->pri_parameters) {
      success = x_core_bool_true;
      parameter = strtok_r(parameters, "&", &parameter_context);
      while (parameter) {
        name = strtok_r(parameter, "=", &nameobject_context);
        value = strtok_r(NULL, "=", &nameobject_context);
        if (name && value) {
          nameobject = x_core_nameobject_create(name, value,
              x_core_string_copy, x_core_string_destroy,
              x_core_string_get_as_string);
          if (nameobject) {
            if (!x_container_set_add
                (hypermessage->pri_parameters, nameobject)) {
              x_core_nameobject_destroy(nameobject);
            }
          } else {
            x_core_trace("x_core_nameobject_create");
            success = x_core_bool_false;
          }
        }
        parameter = strtok_r(NULL, "&", &parameter_context);
      }
    } else {
      x_core_trace("x_container_set_create");
      success = x_core_bool_false;
    }
  } else {
    hypermessage->pri_parameters = NULL;
    success = x_core_bool_true;
  }

  return success;
}

void x_net_hypermessage_create_rollback(x_net_hypermessage_t *hypermessage)
{
  assert(hypermessage);

  if (hypermessage->resource_path) {
    free(hypermessage->resource_path);
  }
  if (hypermessage->hyperheaders) {
    x_container_set_destroy(hypermessage->hyperheaders);
  }
  if (hypermessage->body) {
    free(hypermessage->body);
  }
  free(hypermessage);
}

void x_net_hypermessage_destroy(void *hypermessage_object)
{
  assert(hypermessage_object);
  x_net_hypermessage_t *hypermessage;

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
    x_container_set_destroy(hypermessage->pri_parameters);
  }

  x_container_set_destroy(hypermessage->hyperheaders);

  free(hypermessage);
}

char *x_net_hypermessage_get_body(x_net_hypermessage_t *hypermessage)
{
  return hypermessage->body;
}

unsigned long x_net_hypermessage_get_body_size
(x_net_hypermessage_t *hypermessage)
{
  return hypermessage->body_size;
}

int x_net_hypermessage_get_client_socket(void *hypermessage_object)
{
  assert(hypermessage_object);
  x_net_hypermessage_t *hypermessage;

  hypermessage = hypermessage_object;

  return hypermessage->client_socket;
}

x_net_engine_id_t x_net_hypermessage_get_engine_id
(void *message_object)
{
  return X_NET_ENGINE_HYPER;
}

x_core_nameobject_t *x_net_hypermessage_get_hyperheader
(x_net_hypermessage_t *hypermessage, char *header_name)
{
  assert(hypermessage);
  assert(header_name);
  x_core_nameobject_t *decoy_header;
  x_core_nameobject_t *found_header;

  decoy_header = x_core_nameobject_create_decoy(header_name);
  found_header = x_container_set_find
    (hypermessage->hyperheaders, decoy_header);

  return found_header;
}

x_container_set_t *x_net_hypermessage_get_hyperheaders
(x_net_hypermessage_t *hypermessage)
{
  return hypermessage->hyperheaders;
}

x_net_hypermethod_t x_net_hypermessage_get_hypermethod
(x_net_hypermessage_t *hypermessage)
{
  return hypermessage->hypermethod;
}

x_net_hyperstatus_t x_net_hypermessage_get_hyperstatus
(x_net_hypermessage_t *hypermessage)
{
  return hypermessage->hyperstatus;
}

x_net_hyperversion_t x_net_hypermessage_get_hyperversion
(x_net_hypermessage_t *hypermessage)
{
  return hypermessage->hyperversion;
}

char *x_net_hypermessage_get_resource_name(x_net_hypermessage_t *hypermessage)
{
  return hypermessage->resource_name;
}

char *x_net_hypermessage_get_pri_parameter(x_net_hypermessage_t *hypermessage,
    char *parameter_name)
{
  char *parameter_value;
  x_core_nameobject_t *nameobject_decoy;
  x_core_nameobject_t *nameobject;

  if (hypermessage->pri_parameters) {
    nameobject_decoy = x_core_nameobject_create_decoy(parameter_name);
    if (nameobject_decoy) {
      nameobject
        = x_container_set_find(hypermessage->pri_parameters, nameobject_decoy);
      if (nameobject) {
        parameter_value = x_core_nameobject_get_object(nameobject);
      } else {
        parameter_value = NULL;
      }
      x_core_nameobject_destroy_decoy(nameobject_decoy);
    } else {
      x_core_trace("x_core_nameobject_create_decoy");
      parameter_value = NULL;
    }
  } else {
    parameter_value = NULL;
  }

  return parameter_value;
}

double x_net_hypermessage_get_pri_parameter_as_double
(x_net_hypermessage_t *hypermessage, char *parameter_name)
{
  assert(hypermessage);
  assert(parameter_name);
  char *value_string;
  double value_double;

  value_double = 0.0;

  value_string
    = x_net_hypermessage_get_pri_parameter(hypermessage, parameter_name);
  if (value_string) {
    value_double = atof(value_string);
    free(value_string);
  } else {
    x_core_trace("x_net_hypermessage_get_pri_parameter");
  }

  return value_double;
}

x_core_bool_t x_net_hypermessage_get_pri_parameter_as_unsigned_long
(x_net_hypermessage_t *hypermessage, char *parameter_name,
    unsigned long *value)
{
  assert(hypermessage);
  assert(parameter_name);
  assert(value);
  char *value_string;
  x_core_bool_t success;

  value_string
    = x_net_hypermessage_get_pri_parameter(hypermessage, parameter_name);
  if (value_string) {
    success = x_core_bool_true;
    *value = atol(value_string);
  } else {
    success = x_core_bool_false;
    x_core_trace("x_net_hypermessage_get_pri_parameter");
  }

  return success;
}

x_core_uuid_t *x_net_hypermessage_get_pri_parameter_as_uuid
(x_net_hypermessage_t *hypermessage, char *parameter_name)
{
  assert(hypermessage);
  assert(parameter_name);
  x_core_uuid_t *uuid;
  char *uuid_string;

  uuid = NULL;

  uuid_string
    = x_net_hypermessage_get_pri_parameter(hypermessage, parameter_name);
  if (uuid_string) {
    uuid = x_core_uuid_create_from_string(uuid_string);
    if (!uuid) {
      x_core_trace("x_core_uuid_create_from_string");
    }
  } else {
    x_core_trace("x_net_hypermessage_get_pri_parameter");
  }

  return uuid;
}

x_container_set_t *x_net_hypermessage_get_pri_parameter_as_uuid_set
(x_net_hypermessage_t *hypermessage, char *parameter_name)
{
  assert(hypermessage);
  assert(parameter_name);
  x_core_uuid_t *uuid;
  char *uuid_string;
  char *parameter_string;
  x_container_list_t *uuid_strings;
  x_container_set_t *uuid_set;

  uuid_set = x_container_set_create(&hypermessage->uuid_objectey);
  if (uuid_set) {
    parameter_string
      = x_net_hypermessage_get_pri_parameter(hypermessage, parameter_name);
    if (parameter_string) {
      uuid_strings
        = x_container_list_create_strings_from_string(parameter_string, ",");
      if (uuid_strings) {
        x_container_list_iterate_start(uuid_strings);
        while ((uuid_string = x_container_list_iterate_next(uuid_strings))) {
          uuid = x_core_uuid_create_from_string(uuid_string);
          if (uuid) {
            if (!x_container_set_find(uuid_set, uuid)) {
              if (!x_container_set_add(uuid_set, uuid)) {
                x_core_uuid_destroy(uuid);
                x_core_trace("x_container_set_add");
              }
            } else {
              x_core_uuid_destroy(uuid);
            }
          } else {
            x_core_trace("x_core_uuid_create_from_string");
          }
        }
        x_container_list_destroy(uuid_strings);
      } else {
        x_core_trace("x_container_list_create_string_from_strings");
      }
    } else {
      x_core_trace("x_net_hypermessage_get_pri_parameter");
    }
  } else {
    x_core_trace("x_container_set_create");
  }

  return uuid_set;
}

x_container_set_t *x_net_hypermessage_get_pri_parameters
(x_net_hypermessage_t *hypermessage)
{
  return hypermessage->pri_parameters;
}

char *x_net_hypermessage_get_resource_path(x_net_hypermessage_t *hypermessage)
{
  return hypermessage->resource_path;
}

unsigned long x_net_hypermessage_get_type(void *message_object)
{
  return X_CORE_MESSAGEY_NO_TYPE;
}

x_core_bool_t x_net_hypermessage_set_body(x_net_hypermessage_t *hypermessage,
    x_core_content_t content_type, char *body, unsigned long body_size)
{
  assert(hypermessage);
  assert(body);
  assert(body_size > 0);
  x_core_bool_t success;
  char *content_type_name;
  char *body_size_string;

  content_type_name = x_core_content_get_name(content_type);
  success = x_core_bool_true;

  if (!x_net_hypermessage_set_hyperheader
      (hypermessage, "Content-Type", content_type_name)) {
    success = x_core_bool_false;
  }

  if (asprintf(&body_size_string, "%lu", body_size) != -1) {
    if (!x_net_hypermessage_set_hyperheader
        (hypermessage, "Content-Length", body_size_string)) {
      success = x_core_bool_false;
    }
    free(body_size_string);
  } else {
    x_core_trace("asprintf");
  }

  if (success) {
    hypermessage->body = malloc(body_size);
    if (hypermessage->body) {
      memcpy(hypermessage->body, body, body_size);
      hypermessage->body_size = body_size;
    } else {
      x_core_trace("malloc");
      success = x_core_bool_false;
    }
  }

  return success;
}

x_core_bool_t x_net_hypermessage_set_hyperheader
(x_net_hypermessage_t *hypermessage, char *header_name, char *header_value)
{
  assert(hypermessage);
  assert(header_name);
  assert(header_value);
  x_core_bool_t success;
  x_core_nameobject_t *nameobject;

  nameobject = x_core_nameobject_create(header_name, header_value,
      x_core_string_copy, x_core_string_destroy, x_core_string_get_as_string);
  if (nameobject) {
    if (x_container_set_add(hypermessage->hyperheaders, nameobject)) {
      success = x_core_bool_true;
    } else {
      success = x_core_bool_false;
    }
  } else {
    success = x_core_bool_false;
  }

  return success;
}
