#include "end.h"
#include "../generate.h"

static uint16_t s_state = 0;
static uint16_t s_wincount = 0;
static char s_victories[16];

void updateProcEnd(GContext* _ctx) {

  if (m_dungeon.m_gameOver == 1) {
    uint8_t _height = 43;
    #ifdef HIGH_RES
      _height = 56;
    #endif
    static const char _end1[] = "OOOH, NASTY...";
    renderTextInFrameNoCorrection(_ctx, _end1, GRect(0, 0, PBL_DISPLAY_WIDTH, _height));
    // Grave sprite is 40x68
    drawBitmapAbsNoCorrection(_ctx, m_grave, GPoint(PBL_DISPLAY_WIDTH/2 - 20, PBL_DISPLAY_HEIGHT/2 - 34));
    static const char _end2[] = "RESTART";
    renderTextInFrameNoCorrection(_ctx, _end2, 
      GRect(PBL_DISPLAY_WIDTH/5, PBL_DISPLAY_HEIGHT - _height,
            PBL_DISPLAY_WIDTH - (2*PBL_DISPLAY_WIDTH/5), _height));
    if (getGameState() == kAwaitInput && getFrameCount() < ANIM_FPS/2) {
      drawBitmap(_ctx, m_arrow, 3, 13);
      drawBitmap(_ctx, m_arrow, 13, 13);
    }
  } else {
    uint8_t _height = 63;
    uint8_t _n = 2;
    #ifdef HIGH_RES
      _height = 75;
    #endif
    #ifdef PBL_ROUND
      _n = 4;
    #endif
    static const char _end1[] = "NICELY DONE! DUNGEONEER";
    renderTextInFrameNoCorrection(_ctx, _end1, GRect(0, 0, PBL_DISPLAY_WIDTH, _height));
    // 107x76
    drawBitmapAbsNoCorrection(_ctx, m_treasureBanner, GPoint(PBL_DISPLAY_WIDTH/2 - 54, PBL_DISPLAY_HEIGHT/2 - 38 + 3*SIZE));
    renderBorderTextNoCorrection(_ctx, GRect(0, PBL_DISPLAY_HEIGHT - _n*SIZE, PBL_DISPLAY_WIDTH, _n*SIZE), fonts_get_system_font(FONT_KEY_SMALL), s_victories, 2, GTextAlignmentCenter, false);
    if (getGameState() == kAwaitInput && getFrameCount() < ANIM_FPS/2) {
      drawBitmap(_ctx, m_arrow, 8, 13);
    }
  }
}

// TODO - port best time feature, display this info here too

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
