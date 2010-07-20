#ifndef x_core_content_h
#define x_core_content_h

#define X_CORE_CONTENT_TYPE_COUNT 5
enum x_core_content_t {
  X_CORE_CONTENT_UNKNOWN = 0,
  X_CORE_CONTENT_MIME_IMAGE_GIF = 1,
  X_CORE_CONTENT_MIME_TEXT_CSS = 2,
  X_CORE_CONTENT_MIME_TEXT_HTML = 3,
  X_CORE_CONTENT_MIME_TEXT_JAVASCRIPT = 4,
};
typedef enum x_core_content_t x_core_content_t;

char *x_core_content_get_name(x_core_content_t content_type);

#endif
