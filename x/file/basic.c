#include "cf/x/core/string.h"
#include "cf/x/core/tools.h"
#include "cf/x/file/basic.h"

#define CF_X_FILE_BASIC_MEMORY_BLOCK_SIZE 4096

static const char *filemode_strings[] = {"r", "w", "a", "r+", "w+", "a+"};

static cf_x_core_bool_t slurp(cf_x_file_basic_t *file, char **blob,
    unsigned long *blob_size);

static char *slurp_string(cf_x_file_basic_t *file);

struct cf_x_file_basic_t {
  FILE *file;
  cf_x_case_list_t *token_list_cache;
};

cf_x_file_basic_t *cf_x_file_basic_create(const char *filename, cf_x_file_mode_t mode)
{
  assert(filename);
  cf_x_file_basic_t *file;
  const char *mode_string;

  file = malloc(sizeof *file);
  if (file) {
    mode_string = filemode_strings[mode];
    file->token_list_cache = NULL;
    file->file = fopen(filename, mode_string);
    if (!file->file) {
      cf_x_core_trace("fopen--%s", filename);
    }
  }

  return file;
}

void cf_x_file_basic_destroy(cf_x_file_basic_t *file)
{
  assert(file);

  if (file->token_list_cache) {
    cf_x_case_list_destroy(file->token_list_cache);
  }

  fclose(file->file);
  free(file);

}

cf_x_core_bool_t cf_x_file_basic_exists(const char *filename)
{
  assert(filename);
  cf_x_core_bool_t exists;
  FILE *file;

  file = fopen(filename, filemode_strings[CF_X_FILE_MODE_OPEN_FOR_READ]);
  if (file) {
    fclose(file);
    exists = cf_x_core_bool_true;
  } else {
    exists = cf_x_core_bool_false;
  }

  return exists;
}

cf_x_core_bool_t cf_x_file_basic_get_as_blob(cf_x_file_basic_t *file, char **blob,
    unsigned long *blob_size)
{
  return slurp(file, blob, blob_size);
}

cf_x_case_list_t *cf_x_file_basic_get_as_line_list(cf_x_file_basic_t *file)
{
  return cf_x_file_basic_get_as_token_list(file, "\n");
}

char *cf_x_file_basic_get_as_string(cf_x_file_basic_t *file)
{
  return slurp_string(file);
}

cf_x_case_list_t *cf_x_file_basic_get_as_token_list(cf_x_file_basic_t *file,
    char *delimiters)
{
  assert(file);
  assert(delimiters);
  cf_x_case_list_t *token_list;
  char *token;
  char *token_copy;
  char *file_string;
  char *strtok_context;

  if (!file->token_list_cache) {
    file->token_list_cache = cf_x_case_list_create(cf_x_core_string_compare,
        cf_x_core_string_copy, cf_x_core_string_destroy);
    if (file->token_list_cache) {
      file_string = slurp_string(file);
      token = strtok_r(file_string, delimiters, &strtok_context);
      while (token) {
        token_copy = strdup(token);
        if (token_copy) {
          if (!cf_x_case_list_add_last(file->token_list_cache, token_copy)) {
            cf_x_core_trace("x_case_list_add_last");
            free(token_copy);
          }
        } else {
          cf_x_core_trace("strdup");
        }
        token = strtok_r(NULL, delimiters, &strtok_context);
      }
      free(file_string);
    } else {
      cf_x_core_trace("x_case_list_create");
    }
  }

  if (file->token_list_cache) {
    token_list = cf_x_case_list_copy(file->token_list_cache);
    if (!token_list) {
      cf_x_core_trace("x_case_list_copy");
    }
  } else {
    token_list = NULL;
  }

  return token_list;
}

FILE *cf_x_file_basic_get_file(cf_x_file_basic_t *file)
{
  return file->file;
}

cf_x_core_bool_t cf_x_file_basic_get_line_count(cf_x_file_basic_t *file,
    unsigned long *line_count)
{
  assert(file);
  cf_x_core_bool_t success;
  int c;

  if (fseek(file->file, 0, SEEK_SET) == 0) {
    success = cf_x_core_bool_true;
    *line_count = 0;
    while ((c = fgetc(file->file)) != EOF) {
      if ('\n' == c) {
        (*line_count)++;
      }
    }
  } else {
    success = cf_x_core_bool_false;
    cf_x_core_trace("");
  }

  return success;
}

cf_x_core_bool_t cf_x_file_basic_write_char(cf_x_file_basic_t *file, int c)
{
  assert(file);
  cf_x_core_bool_t success;

  if (EOF != fputc(c, file->file)) {
    success = cf_x_core_bool_true;
  } else {
    success = cf_x_core_bool_false;
  }

  return success;
}

cf_x_core_bool_t cf_x_file_basic_write_string(cf_x_file_basic_t *file, char *string)
{
  assert(file);
  cf_x_core_bool_t success;
  char *c;

  success = cf_x_core_bool_true;

  c = string;
  while (*c) {
    if (EOF != fputc(*c, file->file)) {
      c++;
    } else {
      success = cf_x_core_bool_false;
      break;
    }
  }

  return success;
}

/*
  TODO: fix the realloc calls _ they don't check the return value properly
*/
cf_x_core_bool_t slurp(cf_x_file_basic_t *file, char **blob,
    unsigned long *blob_size)
{
  assert(file);
  assert(blob);
  assert(blob_size);
  unsigned long characters_read;
  int c;
  cf_x_core_bool_t success;

  *blob_size = 0;
  characters_read = 0;
  *blob = NULL;
  success = cf_x_core_bool_true;

  if (fseek(file->file, 0, SEEK_SET) == 0) {
    while ((c = fgetc(file->file)) != EOF) {
      characters_read++;
      if (characters_read > *blob_size) {
        *blob = realloc(*blob, *blob_size + CF_X_FILE_BASIC_MEMORY_BLOCK_SIZE);
        if (*blob) {
          *blob_size += CF_X_FILE_BASIC_MEMORY_BLOCK_SIZE;
        } else {
          success = cf_x_core_bool_false;
          cf_x_core_trace("realloc");
        }
      }
      if (*blob) {
        *(*blob + (characters_read - 1)) = c;
      }
    }
    *blob = realloc(*blob, characters_read);
    if (!*blob) {
      cf_x_core_trace("realloc");
    }
    *blob_size = characters_read;
  } else {
    success = cf_x_core_bool_false;
    cf_x_core_trace("fseek");
  }

  return success;
}

char *slurp_string(cf_x_file_basic_t *file)
{
  assert(file);
  char *string;
  char *blob;
  unsigned long blob_size;

  if (slurp(file, &blob, &blob_size)) {
    string = malloc(blob_size + 1);
    if (string) {
      memcpy(string, blob, blob_size);
      *(string + blob_size) = '\0';
    } else {
      cf_x_core_trace("malloc");
    }
    free(blob);
  } else {
    string = NULL;
    cf_x_core_trace("malloc");
  }

  return string;
}
