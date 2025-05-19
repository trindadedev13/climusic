#ifndef CM_FILE_H
#define CM_FILE_H

#include <dirent.h>
#include <stdio.h>

#include "cm_inline.h"
#include "cm_string.h"

#define CM_FILE_LIST_DIR_INITIAL_CAPACITY 1
#define CM_FILE_MODE_WRITE "w"
#define CM_FILE_MODE_READ "r"
#define CM_FILE_TYPE_FILE_STR "File"
#define CM_FILE_TYPE_DIR_STR "Directory"

// Represents the type of file
enum cm_file_type { CM_FILE, CM_DIR };

// Represents the file
struct cm_file {
  struct dirent* rawdir;
  cm_string name;
  enum cm_file_type filetype;
};

// Store the files and amount of files
struct cm_file_list_dir_data {
  int len;
  struct cm_file* dirs;
  int capacity;
};

// List all files at path.
// Returns **struct cm_file_list_dir_data**
// after use, call cm_file_list_dir_close(cm_file_list_dir_data*)
struct cm_file_list_dir_data* cm_file_list_dir(cm_string path);

// desalocate all memory used in cm_file_list_dir()
void cm_file_list_dir_close(struct cm_file_list_dir_data* tld);

// Returns the string of filetype
CM_INLINE static cm_string cm_file_get_filetype_str(
    enum cm_file_type filetype) {
  if (filetype == CM_FILE) {
    return CM_FILE_TYPE_FILE_STR;
  } else {
    return CM_FILE_TYPE_DIR_STR;
  }
}

// Creates an file
CM_INLINE static FILE* cm_file_create(cm_string file_name) {
  FILE* file;
  file = fopen(file_name, CM_FILE_MODE_WRITE);
  return file;
}

#endif