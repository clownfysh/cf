#include "x/core/nameobject.h"
#include "x/core/string.h"
#include "x/core/tools.h"

struct x_core_nameobject_t {
  char *name;
  void *object;
  x_core_copy_f copy;
  x_core_destroy_f destroy;
  x_core_get_as_string_f get_object_as_string;
};

int x_core_nameobject_compare(void *nameobject_object_a,
    void *nameobject_object_b)
{
  assert(nameobject_object_a);
  assert(nameobject_object_b);
  x_core_nameobject_t *nameobject_a;
  x_core_nameobject_t *nameobject_b;
  int compare;

  nameobject_a = nameobject_object_a;
  nameobject_b = nameobject_object_b;

  compare = strcmp(nameobject_a->name, nameobject_b->name);

  return compare;
}

void *x_core_nameobject_copy(void *nameobject_object)
{
  assert(nameobject_object);
  x_core_nameobject_t *nameobject;
  x_core_nameobject_t *nameobject_copy;
  x_core_bool_t so_far_so_good;

  nameobject = nameobject_object;

  nameobject_copy = malloc(sizeof *nameobject_copy);
  if (nameobject_copy) {
    nameobject_copy->copy = nameobject->copy;
    nameobject_copy->destroy = nameobject->destroy;
    nameobject_copy->get_object_as_string = nameobject->get_object_as_string;
    nameobject_copy->name = strdup(nameobject->name);
    if (nameobject_copy->name) {
      so_far_so_good = x_core_bool_true;
    } else {
      so_far_so_good = x_core_bool_false;
    }
  } else {
    printf("malloc");
    so_far_so_good = x_core_bool_false;
  }

  if (so_far_so_good) {
    if (nameobject->copy) {
      nameobject_copy->object = nameobject->copy(nameobject->object);
      if (!nameobject_copy->object) {
        so_far_so_good = x_core_bool_false;
      }
    } else {
      nameobject_copy->object = nameobject->object;
    }
  }

  if (!so_far_so_good && nameobject_copy) {
    if (nameobject_copy->name) {
      free(nameobject_copy->name);
    }
    if (nameobject_copy->object) {
      nameobject->destroy(nameobject_copy->object);
    }
    free(nameobject_copy);
    nameobject_copy = NULL;
  }

  return nameobject_copy;
}

x_core_nameobject_t *x_core_nameobject_create(char *name,
    void *object, x_core_copy_f copy,
    x_core_destroy_f destroy,
    x_core_get_as_string_f get_object_as_string)
{
  assert(name);
  assert(object);
  assert(destroy);
  assert(get_object_as_string);
  x_core_nameobject_t *nameobject;
  x_core_bool_t so_far_so_good;

  nameobject = malloc(sizeof *nameobject);
  if (nameobject) {
    nameobject->name = strdup(name);
    if (nameobject->name) {
      nameobject->copy = copy;
      nameobject->destroy = destroy;
      nameobject->get_object_as_string = get_object_as_string;
      so_far_so_good = x_core_bool_true;
    } else {
      so_far_so_good = x_core_bool_false;
      x_core_trace("strdup");
    }
  } else {
    so_far_so_good = x_core_bool_false;
    x_core_trace("malloc");
  }

  if (so_far_so_good) {
    if (copy) {
      nameobject->object = copy(object);
      if (!nameobject->object) {
        so_far_so_good = x_core_bool_false;
      }
    } else {
      nameobject->object = object;
    }
  }

  if (!so_far_so_good && nameobject) {
    free(nameobject);
    nameobject = NULL;
  }

  return nameobject;
  assert(!nameobject || nameobject->name);
}

x_core_nameobject_t *x_core_nameobject_create_decoy(char *name)
{
  assert(name);
  x_core_nameobject_t *nameobject;

  nameobject = malloc(sizeof *nameobject);
  if (nameobject) {
    nameobject->name = strdup(name);
    if (!nameobject->name) {
      x_core_trace("strdup");
      free(nameobject);
      nameobject = NULL;
    }
  } else {
    x_core_trace("malloc");
  }

  return nameobject;
}

void x_core_nameobject_destroy(void *nameobject_object)
{
  assert(nameobject_object);
  x_core_nameobject_t *nameobject;

  nameobject = nameobject_object;

  free(nameobject->name);
  nameobject->destroy(nameobject->object);
  free(nameobject);
}

void x_core_nameobject_destroy_decoy(void *nameobject_object)
{
  assert(nameobject_object);
  x_core_nameobject_t *nameobject;

  nameobject = nameobject_object;
  free(nameobject->name);
  free(nameobject);
}

x_core_bool_t x_core_nameobject_equal(void *nameobject_a_object,
    void *nameobject_b_object)
{
  return (0 == x_core_nameobject_compare(nameobject_a_object,
          nameobject_b_object));
}

char *x_core_nameobject_get_as_string(void *nameobject_object)
{
  assert(nameobject_object);
  x_core_nameobject_t *nameobject;
  char *string;
  unsigned long name_size;
  char *object_string;
  unsigned long object_string_size;
  unsigned long total_size;

  nameobject = nameobject_object;

  name_size = strlen(nameobject->name);
  object_string = nameobject->get_object_as_string(nameobject->object);
  object_string_size = strlen(object_string);
  total_size = name_size + 1 + object_string_size;

  string = malloc(total_size + 1);
  if (string) {
    memcpy(string, nameobject->name, name_size);
    *(string + name_size) = ':';
    memcpy(string + name_size + 1, object_string, object_string_size);
    *(string + total_size) = '\0';
  } else {
    x_core_trace("malloc");
  }

  return string;
}

char *x_core_nameobject_get_name(x_core_nameobject_t *nameobject)
{
  return nameobject->name;
}

void *x_core_nameobject_get_object(x_core_nameobject_t *nameobject)
{
  return nameobject->object;
}

void x_core_nameobject_init_objectey(x_core_objectey_t *objectey)
{
  x_core_objectey_init(objectey, x_core_nameobject_compare,
      x_core_nameobject_copy, x_core_nameobject_destroy,
      x_core_nameobject_equal, x_core_nameobject_get_as_string,
      x_core_nameobject_mod);
}

unsigned long x_core_nameobject_mod(void *nameobject_object,
    unsigned long divisor)
{
  assert(nameobject_object);
  x_core_nameobject_t *nameobject = nameobject_object;
  return x_core_string_mod(nameobject->name, divisor);
}

void x_core_nameobject_print(void *nameobject_object)
{
  assert(nameobject_object);
  x_core_nameobject_t *nameobject;
  char *object_string;

  nameobject = nameobject_object;
  object_string = nameobject->get_object_as_string(nameobject->object);
  if (object_string) {
    printf("%s", object_string);
    free(object_string);
  } else {
    x_core_trace("get_as_string");
  }
}
