#include "cm_online.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>

#include "cm_buffer.h"
#include "cm_colors.h"
#include "cm_env.h"
#include "cm_file.h"
#include "cm_gui.h"
#include "cm_screen.h"
#include "cm_string.h"

struct cm_online cm_online_init() {
  struct cm_online co;
  co.curl = curl_easy_init();
  struct cm_buffer cmbrj = {.data = malloc(1024), .size = 0, .capacity = 1024};
  co.response_buffer = cmbrj;
  if (!co.response_buffer.data) {
    cm_gui_draw_textln_d(defcms, CM_COLOR_RED_PAIR,
                         "Failed to allocate data for response.");
    // return NULL;
  }
  return co;
}

size_t cm_online_get_musics_callback(void* contents,
                                     size_t size,
                                     size_t nmemb,
                                     void* userp) {
  size_t total_size = size * nmemb;
  // response json buffer
  struct cm_buffer* rj_buffer = (struct cm_buffer*)userp;

  // reallocates memory dynamically
  if (rj_buffer->size + total_size + 1 > rj_buffer->capacity) {
    size_t new_capacity = (rj_buffer->capacity + total_size) * 2;
    char* new_data = realloc(rj_buffer->data, new_capacity);
    if (!new_data) {
      cm_gui_draw_textln_d(defcms, CM_COLOR_RED_PAIR,
                           "Failed to reallocate data for response.");
      return 0;
    }
    rj_buffer->data = new_data;
    rj_buffer->capacity = new_capacity;
  }

  // copt memory from contents to rj buffer data
  memcpy(rj_buffer->data + rj_buffer->size, contents, total_size);
  rj_buffer->size += total_size;
  rj_buffer->data[rj_buffer->size] = '\0';
  return total_size;
}

void cm_online_get_musics(struct cm_online* co, const cm_string search) {
  if (co->curl) {
    cm_string full_url = strdup(CM_ENV_ONLINE_BASE_URL);
    cm_string encoded = cm_string_encode_spaces(search);
    strcat(full_url, encoded);

    cm_string path = strdup(CM_ENV_PATH);
    strcat(path, "log.txt");
    FILE* log_file = cm_file_create(path);

    curl_easy_setopt(co->curl, CURLOPT_URL, full_url);
    curl_easy_setopt(co->curl, CURLOPT_WRITEFUNCTION,
                     cm_online_get_musics_callback);
    curl_easy_setopt(co->curl, CURLOPT_WRITEDATA, &co->response_buffer);
    co->response_buffer.size = 0;
    co->res = curl_easy_perform(co->curl);

    if (co->res != CURLE_OK) {
      cm_gui_draw_textln_d(defcms, CM_COLOR_RED_PAIR, "Requisition error: %s",
                           curl_easy_strerror(co->res));
    } else {
      fprintf(log_file, "%s", co->response_buffer.data);
    }

    free(encoded);
    free(full_url);
    fclose(log_file);
  }
}

void cm_online_display_musics(struct cm_online* co) {}

void cm_online_close(struct cm_online* co) {
  curl_easy_cleanup(co->curl);
}