#include "end.h"
#include "../generate.h"

static uint16_t s_state = 0;

void updateProcEnd(GContext* _ctx) {

  if (m_dungeon.m_gameOver == 1) {
    static const char _end1[] = "OOOOH, NASTY...";
    static const char _end2[] = "RESTART";
    renderTextInFrame(_ctx, _end1, GRect(0, 0, 144, 45));
    renderTextInFrame(_ctx, _end2, GRect(28, 123, 88, 45));
    drawBitmapAbs(_ctx, m_grave, GPoint(50, 50));
  } else {
    static const char _end1[] = "NICELY DONE! DUNGEONEER";
    static const char _end2[] = "RESTART";
    renderTextInFrame(_ctx, _end1, GRect(0, 0, 144, 45));
    renderTextInFrame(_ctx, _end2, GRect(28, 123, 88, 45));
    drawBitmapAbs(_ctx, m_grave, GPoint(50, 50));
  }

  if (getGameState() == kAwaitInput && getFrameCount() < ANIM_FPS/2) {
    drawBitmap(_ctx, m_arrow, 3, 13);
    drawBitmap(_ctx, m_arrow, 13, 13);
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
    ++s_state;
  }

  return false;
}
