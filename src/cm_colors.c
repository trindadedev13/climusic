#include "cm_colors.h"

#include <ncurses.h>

void cm_colors_init(cm_color_pair background_id) {
  start_color();
  use_default_colors();  // allow -1 as transparent

  if (can_change_color()) {
    init_color(CM_COLOR_PRIMARY_DARK_ID, 80, 80, 80);   // #505050 (dark grey)
    init_color(CM_COLOR_PRIMARY_ID, 1000, 1000, 1000);  // #ffffff (white)
    init_color(CM_COLOR_SECONDARY_ID, 200, 200, 200);   // #c8c8c8 (light grey)
    init_color(CM_COLOR_BACKGROUND_ID, 33, 33, 33);     // #212121 (dark grey)
    init_color(CM_COLOR_RED_ID, 1000, 0, 0);            // #FF0000 (red)
  }

  // init pairs
  init_pair(CM_COLOR_PRIMARY_DARK_PAIR, CM_COLOR_PRIMARY_DARK_ID,
            background_id);
  init_pair(CM_COLOR_PRIMARY_PAIR, CM_COLOR_PRIMARY_ID, background_id);
  init_pair(CM_COLOR_SECONDARY_PAIR, CM_COLOR_SECONDARY_ID, background_id);
  init_pair(CM_COLOR_BACKGROUND_PAIR, COLOR_BLACK,
            background_id);  // background
  init_pair(CM_COLOR_RED_PAIR, CM_COLOR_RED_ID, background_id);
}

void cm_colors_set_background_color(cm_color_pair ccp) {
  bkgd(COLOR_PAIR(ccp));
  refresh();
}