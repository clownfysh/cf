#ifndef x_net_hypermessage_h
#define x_net_hypermessage_h

#include "x/case/set.h"
#include "x/core/content.h"
#include "x/core/engine.h"
#include "x/core/nameobject.h"
#include "x/core/object.h"
#include "x/core/uuid.h"
#include "x/net/hyperversion.h"

#define X_NET_HYPERMESSAGE_NO_HEADERS NULL
#define X_NET_HYPERMESSAGE_NO_RESOURCE_PATH NULL

enum x_net_hypermethod_t {
  X_NET_HYPERMETHOD_UNKNOWN,
  X_NET_HYPERMETHOD_GET,
  X_NET_HYPERMETHOD_HEAD,
  X_NET_HYPERMETHOD_POST,
};
typedef enum x_net_hypermethod_t x_net_hypermethod_t;

enum x_net_hyperstatus_t {
  X_NET_HYPERSTATUS_UNKNOWN,
  X_NET_HYPERSTATUS_OK,
  X_NET_HYPERSTATUS_NOT_FOUND,
};
typedef enum x_net_hyperstatus_t x_net_hyperstatus_t;

struct x_net_hypermessage_t;
typedef struct x_net_hypermessage_t x_net_hypermessage_t;

x_net_hypermessage_t *x_net_hypermessage_create(int client_socket,
    x_net_hypermethod_t hypermethod, x_net_hyperstatus_t hyperstatus,
    char *resource_path, x_net_hyperversion_t hyperversion,
    x_case_set_t *hyperheaders);

void x_net_hypermessage_destroy(void *hypermessage_object);

char *x_net_hypermessage_get_body(x_net_hypermessage_t *hypermessage);

unsigned long x_net_hypermessage_get_body_size
(x_net_hypermessage_t *hypermessage);

int x_net_hypermessage_get_client_socket(void *hypermessage_object);

x_net_engine_id_t x_net_hypermessage_get_engine_id(void *message_object);

x_core_nameobject_t *x_net_hypermessage_get_hyperheader
(x_net_hypermessage_t *hypermessage, char *header_name);

x_case_set_t *x_net_hypermessage_get_hyperheaders
(x_net_hypermessage_t *hypermessage);

x_net_hypermethod_t x_net_hypermessage_get_hypermethod
(x_net_hypermessage_t *hypermessage);

x_net_hyperstatus_t x_net_hypermessage_get_hyperstatus
(x_net_hypermessage_t *hypermessage);

x_net_hyperversion_t x_net_hypermessage_get_hyperversion
(x_net_hypermessage_t *hypermessage);

char *x_net_hypermessage_get_resource_name(x_net_hypermessage_t *hypermessage);

char *x_net_hypermessage_get_pri_parameter(x_net_hypermessage_t *hypermessage,
    char *parameter_name);

double x_net_hypermessage_get_pri_parameter_as_double
(x_net_hypermessage_t *hypermessage, char *parameter_name);

x_core_bool_t x_net_hypermessage_get_pri_parameter_as_unsigned_long
(x_net_hypermessage_t *hypermessage, char *parameter_name,
    unsigned long *value);

x_core_uuid_t *x_net_hypermessage_get_pri_parameter_as_uuid
(x_net_hypermessage_t *hypermessage, char *parameter_name);

x_case_set_t *x_net_hypermessage_get_pri_parameter_as_uuid_set
(x_net_hypermessage_t *hypermessage, char *parameter_name);

x_case_set_t *x_net_hypermessage_get_pri_parameters
(x_net_hypermessage_t *hypermessage);

char *x_net_hypermessage_get_resource_path(x_net_hypermessage_t *hypermessage);

unsigned long x_net_hypermessage_get_type(void *message_object);

x_core_bool_t x_net_hypermessage_set_body(x_net_hypermessage_t *hypermessage,
    x_core_content_t content_type, char *body, unsigned long body_size);

x_core_bool_t x_net_hypermessage_set_hyperheader
(x_net_hypermessage_t *hypermessage, char *header_name, char *header_value);

#endif
