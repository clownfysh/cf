#ifndef x_file_mode_h
#define x_file_mode_h

enum x_file_mode_t {
  X_FILE_MODE_OPEN_FOR_READ = 0,
  X_FILE_MODE_TRUNCATE_OR_CREATE_FOR_WRITE = 1,
  X_FILE_MODE_OPEN_OR_CREATE_FOR_WRITE_AT_END = 2,
  X_FILE_MODE_OPEN_FOR_READ_AND_WRITE = 3,
  X_FILE_MODE_TRUNCATE_OR_CREATE_FOR_READ_AND_WRITE = 4,
  X_FILE_MODE_OPEN_OR_CREATE_FOR_READ_AND_WRITE_AT_END = 5
};
typedef enum x_file_mode_t x_file_mode_t;

#endif
