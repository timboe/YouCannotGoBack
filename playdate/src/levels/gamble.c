#include "gamble.h"

static uint16_t s_state = 0;

void updateProcGamble(PlaydateAPI* _pd) {
  renderFloor(_pd, 0);
  renderClutter(_pd);
  renderPlayer(_pd);
  renderWalls(_pd, true, true, true, true);
  renderWallClutter(_pd);
  renderArrows(_pd, 15, 5, 4);
}

bool tickGamble(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position_x = 0;
    m_player.m_position_y = SIZE*9;
    addCluter(20, 20, 0);
    return false;
  }

  if (s_state == 0) { // start initial move
    enterRoom(&s_state);
  } else if (s_state == 1) { // initial move is done
    setGameState(kAwaitInput);
    ++s_state;
  } else if (s_state == 2) {
    moveToExit(&s_state);
  } else if (s_state == 3) {
    setGameState(kFadeOut);
  }

  return false;
}
