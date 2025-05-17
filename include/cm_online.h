#ifndef CM_WEB_H
#define CM_WEB_H

#include <stdio.h>

#include <curl/curl.h>

#include "cm_buffer.h"
#include "cm_string.h"

struct cm_online {
  const cm_string url;
  CURL* curl;
  CURLcode res;
  struct cm_buffer response_buffer;
};

// Initializes thr cm online.
struct cm_online cm_online_init();

// The callback of curl requisition.
size_t cm_online_get_musics_callback(void* contents,
                                     size_t size,
                                     size_t nmemb,
                                     void* userp);

// Start the Requisition.
void cm_online_get_musics(struct cm_online* co, const cm_string url);

// Displays musics
void cm_online_display_musics(struct cm_online* co);

// Closes the curl and stuff.
void cm_online_close(struct cm_online* co);

#endif