#include "end.h"
#include "../generate.h"

static uint16_t s_state = 0;
static uint16_t s_wincount = 0;
static char s_victories[16];

void updateProcEnd(GContext* _ctx) {

  if (m_dungeon.m_gameOver == 1) {
    static const char _end1[] = "OOOOH, NASTY...";
    renderTextInFrame(_ctx, _end1, GRect(0, 0, 144, 43));
    drawBitmapAbs(_ctx, m_grave, GPoint(50, 50));
    static const char _end2[] = "RESTART";
    renderTextInFrame(_ctx, _end2, GRect(28, 125, 88, 43));
    if (getGameState() == kAwaitInput && getFrameCount() < ANIM_FPS/2) {
      drawBitmap(_ctx, m_arrow, 3, 13);
      drawBitmap(_ctx, m_arrow, 13, 13);
    }
  } else {
    static const char _end1[] = "NICELY DONE! DUNGEONEER";
    renderTextInFrame(_ctx, _end1, GRect(0, 0, 144, 63));
    drawBitmapAbs(_ctx, m_treasureBanner, GPoint(19, 68));
    renderBorderText(_ctx, GRect(0, 145, 144, 20), fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), s_victories, 2, GTextAlignmentCenter, false);
    if (getGameState() == kAwaitInput && getFrameCount() < ANIM_FPS/2) {
      drawBitmap(_ctx, m_arrow, 8, 12);
    }
  }
}

bool tickEnd(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    if (m_dungeon.m_gameOver == 2) {
      if (persist_exists(PERSIST_KEY_VICTORY)) s_wincount = persist_read_int(PERSIST_KEY_VICTORY);
      persist_write_int(PERSIST_KEY_VICTORY, ++s_wincount);
      snprintf(s_victories, 16, "VICTORIES: %i", s_wincount);
    }
    return false;
  }

  if (s_state == 0) {
    setGameState(kAwaitInput);
    ++s_state;
  } else if (s_state == 1) { // restart
    generate();
  }

  return false;
}
