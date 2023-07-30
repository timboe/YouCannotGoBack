#include "start.h"

static uint16_t s_state = 0;

void updateProcStart(PlaydateAPI* _pd) {

  renderFloor(_pd, 0);
  drawBitmapAbs(_pd, m_ycgb, 36, 16);
  renderClutter(_pd);
  renderPlayer(_pd);
  renderWalls(_pd, true, true, true, true);
  renderWallClutter(_pd);

  drawCBitmap(_pd, &m_flagstone[ m_dungeon.m_finalPuzzle ], 0, 9);

  renderArrows(_pd, 13, 5, 4);
}

bool tickStart(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_rotation = 0.0f;
    m_player.m_position_x = 0;
    m_player.m_position_y = SIZE*9;
    addCluter(4, 0, 20); // Only left
    return false;
  }

  if (s_state == 0) { // start initial move
    enterRoom(&s_state);
  } else if (s_state == 1) { // initial move is done
    setGameState(kAwaitInput);
    s_state = 4;
  } else if (s_state == 3) {
    setGameState(kAwaitInput);
    ++s_state;
  } else if (s_state == 4) {
    moveToExit(&s_state);
  } else if (s_state == 5) {
    setGameState(kFadeOut);
  }

  return false;
}
