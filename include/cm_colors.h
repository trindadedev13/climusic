#ifndef CM_COLORS_H
#define CM_COLORS_H

// pairs
#define CM_COLOR_PRIMARY_PAIR 2
#define CM_COLOR_PRIMARY_DARK_PAIR 1
#define CM_COLOR_SECONDARY_PAIR 3
#define CM_COLOR_BACKGROUND_PAIR 4
#define CM_COLOR_RED_PAIR 5

// ids
#define CM_COLOR_PRIMARY_DARK_ID 8
#define CM_COLOR_PRIMARY_ID 9
#define CM_COLOR_SECONDARY_ID 10
#define CM_COLOR_BACKGROUND_ID 11
#define CM_COLOR_RED_ID 12

typedef int cm_color_pair;

void cm_colors_init(cm_color_pair);

void cm_colors_enable_color(cm_color_pair);

void cm_colors_disable_color(cm_color_pair);

void cm_colors_set_background_color(cm_color_pair);

#endif