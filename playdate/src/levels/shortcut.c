#include <math.h>
#include "shortcut.h"
#include "../sound.h"

static uint16_t s_state = 0; // game state

void updateProcShortcut(PlaydateAPI* _pd) {

  renderShortcutFloor(_pd);
  renderPlayer(_pd);
  renderShortcutWalls(_pd);

  PDRect _b = {.x = 0, .y = SIZE*12, .width = SIZE*20, .height = 16};
  renderText(_pd, "Shortcut!", _b, kDrawModeFillWhite);
}

bool tickShortcut(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position_x = -2*SIZE;
    m_player.m_position_y = SIZE*8;
    return false;
  }

  if (s_state == 0) { // start initial move
     m_player.m_target_x = SIZE*8;
     m_player.m_target_y = SIZE*8;
     setGameState(kMovePlayer);
     ++s_state;
  } else if (s_state == 1) {
    reminderSound();
    setGameState(kFadeOut);
  }

  return true;
}
