#ifndef cf_x_core_content_h
#define cf_x_core_content_h

#define CF_X_CORE_CONTENT_TYPE_COUNT 5
enum cf_x_core_content_t {
  CF_X_CORE_CONTENT_UNKNOWN = 0,
  CF_X_CORE_CONTENT_MIME_IMAGE_GIF = 1,
  CF_X_CORE_CONTENT_MIME_TEXT_CSS = 2,
  CF_X_CORE_CONTENT_MIME_TEXT_HTML = 3,
  CF_X_CORE_CONTENT_MIME_TEXT_JAVASCRIPT = 4,
};
typedef enum cf_x_core_content_t cf_x_core_content_t;

char *cf_x_core_content_get_name(cf_x_core_content_t content_type);

#endif
