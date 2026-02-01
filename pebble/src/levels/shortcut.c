#include "shortcut.h"
#ifdef YCGBv2

static uint16_t s_state = 0; // game state

void updateProcShortcut(GContext* _ctx) {

  renderShortcutFloor(_ctx);
  renderPlayer(_ctx);
  renderShortcutWalls(_ctx);

  static const char _sc[] = "Shortcut!";
  renderBorderText(_ctx, GRect(0, SIZE*12, PBL_DISPLAY_WIDTH, SIZE), fonts_get_system_font(FONT_KEY_SMALL), _sc, 2, GTextAlignmentCenter, true);
}

bool tickShortcut(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position = GPoint(-2*SIZE, SIZE*8);
    return false;
  }

  if (s_state == 0) { // start initial move
     m_player.m_target = GPoint(SIZE*8, SIZE*8);
     setGameState(kMovePlayer);
     ++s_state;
  } else if (s_state == 1) {
    setGameState(kFadeOut);
  }

  return true;
}


#endif