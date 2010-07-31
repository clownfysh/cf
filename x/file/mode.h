#ifndef cf_x_file_mode_h
#define cf_x_file_mode_h

enum cf_x_file_mode_t {
  CF_X_FILE_MODE_OPEN_FOR_READ = 0,
  CF_X_FILE_MODE_TRUNCATE_OR_CREATE_FOR_WRITE = 1,
  CF_X_FILE_MODE_OPEN_OR_CREATE_FOR_WRITE_AT_END = 2,
  CF_X_FILE_MODE_OPEN_FOR_READ_AND_WRITE = 3,
  CF_X_FILE_MODE_TRUNCATE_OR_CREATE_FOR_READ_AND_WRITE = 4,
  CF_X_FILE_MODE_OPEN_OR_CREATE_FOR_READ_AND_WRITE_AT_END = 5
};
typedef enum cf_x_file_mode_t cf_x_file_mode_t;

#endif
