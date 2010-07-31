#ifndef cf_x_net_hypermessage_h
#define cf_x_net_hypermessage_h

#include "cf/x/case/set.h"
#include "cf/x/core/content.h"
#include "cf/x/core/engine.h"
#include "cf/x/core/nameobject.h"
#include "cf/x/core/object.h"
#include "cf/x/core/uuid.h"
#include "cf/x/net/hyperversion.h"

#define CF_X_NET_HYPERMESSAGE_NO_HEADERS NULL
#define CF_X_NET_HYPERMESSAGE_NO_RESOURCE_PATH NULL

enum cf_x_net_hypermethod_t {
  CF_X_NET_HYPERMETHOD_UNKNOWN,
  CF_X_NET_HYPERMETHOD_GET,
  CF_X_NET_HYPERMETHOD_HEAD,
  CF_X_NET_HYPERMETHOD_POST,
};
typedef enum cf_x_net_hypermethod_t cf_x_net_hypermethod_t;

enum cf_x_net_hyperstatus_t {
  CF_X_NET_HYPERSTATUS_UNKNOWN,
  CF_X_NET_HYPERSTATUS_OK,
  CF_X_NET_HYPERSTATUS_NOT_FOUND,
};
typedef enum cf_x_net_hyperstatus_t cf_x_net_hyperstatus_t;

struct cf_x_net_hypermessage_t;
typedef struct cf_x_net_hypermessage_t cf_x_net_hypermessage_t;

cf_x_net_hypermessage_t *cf_x_net_hypermessage_create(int client_socket,
    cf_x_net_hypermethod_t hypermethod, cf_x_net_hyperstatus_t hyperstatus,
    char *resource_path, cf_x_net_hyperversion_t hyperversion,
    cf_x_case_set_t *hyperheaders);

void cf_x_net_hypermessage_destroy(void *hypermessage_object);

char *cf_x_net_hypermessage_get_body(cf_x_net_hypermessage_t *hypermessage);

unsigned long cf_x_net_hypermessage_get_body_size
(cf_x_net_hypermessage_t *hypermessage);

int cf_x_net_hypermessage_get_client_socket(void *hypermessage_object);

cf_x_net_engine_id_t cf_x_net_hypermessage_get_engine_id(void *message_object);

cf_x_core_nameobject_t *cf_x_net_hypermessage_get_hyperheader
(cf_x_net_hypermessage_t *hypermessage, char *header_name);

cf_x_case_set_t *cf_x_net_hypermessage_get_hyperheaders
(cf_x_net_hypermessage_t *hypermessage);

cf_x_net_hypermethod_t cf_x_net_hypermessage_get_hypermethod
(cf_x_net_hypermessage_t *hypermessage);

cf_x_net_hyperstatus_t cf_x_net_hypermessage_get_hyperstatus
(cf_x_net_hypermessage_t *hypermessage);

cf_x_net_hyperversion_t cf_x_net_hypermessage_get_hyperversion
(cf_x_net_hypermessage_t *hypermessage);

char *cf_x_net_hypermessage_get_resource_name(cf_x_net_hypermessage_t *hypermessage);

char *cf_x_net_hypermessage_get_pri_parameter(cf_x_net_hypermessage_t *hypermessage,
    char *parameter_name);

double cf_x_net_hypermessage_get_pri_parameter_as_double
(cf_x_net_hypermessage_t *hypermessage, char *parameter_name);

cf_x_core_bool_t cf_x_net_hypermessage_get_pri_parameter_as_unsigned_long
(cf_x_net_hypermessage_t *hypermessage, char *parameter_name,
    unsigned long *value);

cf_x_core_uuid_t *cf_x_net_hypermessage_get_pri_parameter_as_uuid
(cf_x_net_hypermessage_t *hypermessage, char *parameter_name);

cf_x_case_set_t *cf_x_net_hypermessage_get_pri_parameter_as_uuid_set
(cf_x_net_hypermessage_t *hypermessage, char *parameter_name);

cf_x_case_set_t *cf_x_net_hypermessage_get_pri_parameters
(cf_x_net_hypermessage_t *hypermessage);

char *cf_x_net_hypermessage_get_resource_path(cf_x_net_hypermessage_t *hypermessage);

unsigned long cf_x_net_hypermessage_get_type(void *message_object);

cf_x_core_bool_t cf_x_net_hypermessage_set_body(cf_x_net_hypermessage_t *hypermessage,
    cf_x_core_content_t content_type, char *body, unsigned long body_size);

cf_x_core_bool_t cf_x_net_hypermessage_set_hyperheader
(cf_x_net_hypermessage_t *hypermessage, char *header_name, char *header_value);

#endif
