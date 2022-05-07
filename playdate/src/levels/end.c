#include "end.h"
#include "../generate.h"
#include "../sound.h"

static uint16_t s_state = 0;
static char s_timeDisplay[16];

void updateProcEnd(PlaydateAPI* _pd, bool _isRotated) {

  renderClear(_pd, false);
  int _x = (_isRotated ? SIZE*2 : 0);
  int _w = (_isRotated ? 144-(SIZE*4) : 144);
  if (m_dungeon.m_gameOver == 1) {
    static const char _end1[] = "OOOH, NASTY...";
    static const char _end1_short[] = "OOH, NASTY..";  
    PDRect _rect = {.x = _x, .y = 0, .width = _w, .height = 43};
    renderTextInFrame(_pd, _isRotated ? _end1_short : _end1, _rect);
    drawBitmapAbs(_pd, m_grave, 50, 50);
    static const char _end2[] = "RESTART";
    PDRect _rect2 = {.x = 28, .y = 125, .width = 88, .height = 43};
    renderTextInFrame(_pd, _end2, _rect2);
    if (getGameState() == kAwaitInput && getFrameCount() < ANIM_FPS/2) {
      drawCBitmap(_pd, &m_arrow_d, 3, 13);
      drawCBitmap(_pd, &m_arrow_d, 13, 13);
    }
  } else {
    static const char _end1[] = "NICELY DONE!\n DUNGEONEER";
    PDRect _rect = {.x = _x, .y = 0, .width = _w, .height = 63};
    renderTextInFrame(_pd, _end1, _rect);
    drawBitmapAbs(_pd, m_treasureBanner, 19, 68);
    PDRect _rect2 = {.x = 0, .y = 145, .width = 144, .height = 20};
    renderBorderText(_pd, _rect2, m_fontMain, s_timeDisplay, 2, false);
    if (getGameState() == kAwaitInput && getFrameCount() < ANIM_FPS/2) {
      drawCBitmap(_pd, &m_arrow_d, 8, 12);
    }
  }
}

bool tickEnd(PlaydateAPI* _pd, bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    if (m_dungeon.m_gameOver == 2) {
      snprintf(s_timeDisplay, 16, "TIME %is", m_dungeon.m_ticksTotal / ANIM_FPS);
    }
    return false;
  }

  if (s_state == 0) {
    setGameState(kAwaitInput);
    if (m_dungeon.m_gameOver == 1) {
      looseSound();
    } else {
      winSound();
    }
    ++s_state;
  } else if (s_state == 1) { // restart
    generate(_pd);
  }

  return false;
}
