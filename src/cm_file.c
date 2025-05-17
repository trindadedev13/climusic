#include "cm_file.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

#include "cm_string.h"

// List all files at path.
// Returns **struct cm_file_list_dir_data**
// after use, call cm_file_list_dir_close(cm_file_list_dir_data*)
// https://stackoverflow.com/questions/4204666/how-to-list-files-in-a-directory-in-a-c-program
struct cm_file_list_dir_data* cm_file_list_dir(cm_string path) {
  struct cm_file_list_dir_data* tld =
      malloc(sizeof(struct cm_file_list_dir_data));
  tld->dirs = malloc(sizeof(struct cm_file) *
                     CM_FILE_LIST_DIR_INITIAL_CAPACITY);  // 100
  tld->len = 0;
  tld->capacity = CM_FILE_LIST_DIR_INITIAL_CAPACITY;

  DIR* d;
  struct dirent* dir;

  d = opendir(path);

  if (d) {
    while ((dir = readdir(d)) != NULL) {
      if (tld->len >= tld->capacity) {
        tld->capacity = tld->len + 1;
        tld->dirs = realloc(tld->dirs, sizeof(struct cm_file) * tld->capacity);
      }
      int nlen = tld->len++;
      tld->dirs[nlen].filetype = (dir->d_type == DT_DIR) ? TDIR : TFILE;
      tld->dirs[nlen].name = strdup(dir->d_name);
    }
    closedir(d);
  }

  return tld;
}

// desalocate all memory used in cm_file_list_dir()
void cm_file_list_dir_close(struct cm_file_list_dir_data* tld) {
  for (int i = 0; i < tld->len; i++) {
    free(tld->dirs[i].name);
    free(tld->dirs[i].rawdir);
  }
  free(tld->dirs);
  free(tld);
}

// Returns the string of filetype
cm_string cm_file_get_filetype_str(enum cm_file_type filetype) {
  if (filetype == TFILE) {
    return FILE_TYPE_FILE_STR;
  } else {
    return FILE_TYPE_DIR_STR;
  }
}

// Creates an file
FILE* cm_file_create(cm_string file_name) {
  FILE* file;
  file = fopen(file_name, FILE_MODE_WRITE);
  return file;
}