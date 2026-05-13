#include "end.h"
#include "../generate.h"

static uint16_t s_state = 0;
static uint16_t s_wincount = 0;
static uint16_t s_besttime_stored = 0;
static char s_victories[20];
static char s_thistime[20];
static char s_besttime[20];

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

    uint8_t _bannerHeight = 63;
    uint8_t _textHeightN = 2;
    uint8_t _imgHeightN = 3;
    uint8_t _arrowHeightN = 13;
    #ifdef HIGH_RES
      _bannerHeight = 75;
      _textHeightN = 6;
      _imgHeightN = 1;
      _arrowHeightN = 11;
    #endif
    #ifdef PBL_ROUND
      _imgHeightN = 2;
      _textHeightN = 4;
      _arrowHeightN = 12;
      #ifdef HIGH_RES
        _imgHeightN = 0;
        _bannerHeight = 75;
        _textHeightN = 8;
        _arrowHeightN = 9;
      #endif
    #endif
    static const char _end1[] = "NICELY DONE!\nDUNGEONEER";
    renderTextInFrameNoCorrection(_ctx, _end1, GRect(0, 0, PBL_DISPLAY_WIDTH, _bannerHeight));
    // 107x76
    drawBitmapAbsNoCorrection(_ctx, m_treasureBanner, GPoint(PBL_DISPLAY_WIDTH/2 - 54, PBL_DISPLAY_HEIGHT/2 - 38 + _imgHeightN*SIZE));
    if (getGameState() == kAwaitInput && getFrameCount() < ANIM_FPS/2) {
      drawBitmap(_ctx, m_arrow, 8, _arrowHeightN);
    }
    #ifdef HIGH_RES
      renderBorderTextNoCorrection(_ctx, GRect(0, PBL_DISPLAY_HEIGHT - _textHeightN*SIZE, PBL_DISPLAY_WIDTH, 2*SIZE), fonts_get_system_font(FONT_KEY_SMALL), s_victories, 2, GTextAlignmentCenter, false);
      renderBorderTextNoCorrection(_ctx, GRect(0, PBL_DISPLAY_HEIGHT - (_textHeightN-2)*SIZE, PBL_DISPLAY_WIDTH, 2*SIZE), fonts_get_system_font(FONT_KEY_SMALL), s_thistime, 2, GTextAlignmentCenter, false);
      renderBorderTextNoCorrection(_ctx, GRect(0, PBL_DISPLAY_HEIGHT - (_textHeightN-4)*SIZE, PBL_DISPLAY_WIDTH, 2*SIZE), fonts_get_system_font(FONT_KEY_SMALL), s_besttime, 2, GTextAlignmentCenter, false);
    #else
      static uint16_t _counter = 0;
      if (!getFrameCount()) ++_counter;
      if (_counter % 3 == 0) renderBorderTextNoCorrection(_ctx, GRect(0, PBL_DISPLAY_HEIGHT - _textHeightN*SIZE, PBL_DISPLAY_WIDTH, 2*SIZE), fonts_get_system_font(FONT_KEY_SMALL), s_victories, 2, GTextAlignmentCenter, false);
      if (_counter % 3 == 1) renderBorderTextNoCorrection(_ctx, GRect(0, PBL_DISPLAY_HEIGHT - _textHeightN*SIZE, PBL_DISPLAY_WIDTH, 2*SIZE), fonts_get_system_font(FONT_KEY_SMALL), s_thistime, 2, GTextAlignmentCenter, false);
      if (_counter % 3 == 2) renderBorderTextNoCorrection(_ctx, GRect(0, PBL_DISPLAY_HEIGHT - _textHeightN*SIZE, PBL_DISPLAY_WIDTH, 2*SIZE), fonts_get_system_font(FONT_KEY_SMALL), s_besttime, 2, GTextAlignmentCenter, false);      
    #endif
  
  }
}

bool tickEnd(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    if (m_dungeon.m_gameOver == 1) {
      #ifdef PBL_RGB_BACKLIGHT
        light_set_color(GColorRed);
      #endif
    } else if (m_dungeon.m_gameOver == 2) {
      if (persist_exists(PERSIST_KEY_VICTORY)) s_wincount = persist_read_int(PERSIST_KEY_VICTORY);
      persist_write_int(PERSIST_KEY_VICTORY, ++s_wincount);
      snprintf(s_victories, 20, "VICTORIES: %i", s_wincount);

      const uint16_t _thisrun = (m_dungeon.m_ticksTotal / ANIM_FPS);
      if (persist_exists(PERSIST_KEY_BESTTIME)) s_besttime_stored = persist_read_int(PERSIST_KEY_BESTTIME);
      else s_besttime_stored = 9999;
      if (_thisrun < s_besttime_stored) s_besttime_stored = _thisrun;
      persist_write_int(PERSIST_KEY_BESTTIME, s_besttime_stored);
      snprintf(s_thistime, 20, "TIME: %im %is", _thisrun / 60, _thisrun % 60);
      snprintf(s_besttime, 20, "BEST: %im %is", s_besttime_stored / 60, s_besttime_stored % 60);
    }
    return false;
  }

  if (s_state == 0) {
    setGameState(kAwaitInput);
    ++s_state;
  } else if (s_state == 1) { // restart
    #ifdef PBL_RGB_BACKLIGHT
      light_set_system_color();
    #endif
    generate();
  }

  return false;
}
