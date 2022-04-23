#include "end.h"
#include "../generate.h"

static uint16_t s_state = 0;
static uint16_t s_wincount = 0;
static char s_victories[16];

void updateProcEnd(PlaydateAPI* _pd) {

  if (m_dungeon.m_gameOver == 1) {
    static const char _end1[] = "OOOOH, NASTY...";
    PDRect _rect = {.x = 0, .y = 0, .width = 144, .height = 43};
    renderTextInFrame(_pd, _end1, _rect);
    drawBitmapAbs(_pd, m_grave, 50, 50);
    static const char _end2[] = "RESTART";
    PDRect _rect2 = {.x = 28, .y = 125, .width = 88, .height = 43};
    renderTextInFrame(_pd, _end2, _rect2);
    if (getGameState() == kAwaitInput && getFrameCount() < ANIM_FPS/2) {
      drawCBitmap(_pd, &m_arrow, 3, 13);
      drawCBitmap(_pd, &m_arrow, 13, 13);
    }
  } else {
    static const char _end1[] = "NICELY DONE! DUNGEONEER";
    PDRect _rect = {.x = 0, .y = 0, .width = 144, .height = 63};
    renderTextInFrame(_pd, _end1, _rect);
    drawBitmapAbs(_pd, m_treasureBanner, 19, 68);
    PDRect _rect2 = {.x = 0, .y = 145, .width = 144, .height = 20};
    renderBorderText(_pd, _rect2, /*fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD),*/ s_victories, 2, /*GTextAlignmentCenter,*/ false);
    if (getGameState() == kAwaitInput && getFrameCount() < ANIM_FPS/2) {
      drawCBitmap(_pd, &m_arrow, 8, 12);
    }
  }
}

bool tickEnd(PlaydateAPI* _pd, bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    if (m_dungeon.m_gameOver == 2) {
      //if (persist_exists(PERSIST_KEY_VICTORY)) s_wincount = persist_read_int(PERSIST_KEY_VICTORY);
      //persist_write_int(PERSIST_KEY_VICTORY, ++s_wincount);
      snprintf(s_victories, 16, "VICTORIES: %i", s_wincount);
    }
    return false;
  }

  if (s_state == 0) {
    setGameState(kAwaitInput);
    ++s_state;
  } else if (s_state == 1) { // restart
    generate(_pd);
  }

  return false;
}
