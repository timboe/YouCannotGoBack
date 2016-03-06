#include "end.h"
#include "../generate.h"

static uint16_t s_state = 0;

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
    drawBitmapAbs(_ctx, m_treasureBanner, GPoint(19, 75));
    if (getGameState() == kAwaitInput && getFrameCount() < ANIM_FPS/2) {
      drawBitmap(_ctx, m_arrow, 16, 19);
    }
  }
}

bool tickEnd(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
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
