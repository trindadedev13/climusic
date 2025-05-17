#ifndef CM_FILE_H
#define CM_FILE_H

#include <dirent.h>
#include <stdio.h>

#define CM_FILE_LIST_DIR_INITIAL_CAPACITY 1
#define FILE_MODE_WRITE "w"
#define FILE_MODE_READ "r"
#define FILE_TYPE_FILE_STR "File"
#define FILE_TYPE_DIR_STR "Directory"

// Represents the type of file
enum cm_file_type { TFILE, TDIR };

// Represents the file
struct cm_file {
  struct dirent* rawdir;
  char* name;
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
struct cm_file_list_dir_data* cm_file_list_dir(char* path);

// desalocate all memory used in cm_file_list_dir()
void cm_file_list_dir_close(struct cm_file_list_dir_data* tld);

// Returns the string of filetype
char* cm_file_get_filetype_str(enum cm_file_type filetype);

// Creates an file
FILE* cm_file_create(char* file_name);

#endif