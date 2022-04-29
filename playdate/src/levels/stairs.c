#include "start.h"

static uint16_t s_state = 0;

void updateProcStairs(PlaydateAPI* _pd) {

  renderFloor(_pd, 0);
  drawCBitmap(_pd, &m_stairs, 9, 8);
  drawCBitmap(_pd, &m_stairs, 9, 10);
  renderClutter(_pd);
  renderPlayer(_pd);
  renderWalls(_pd, true, false, false, false);
  renderWallClutter(_pd);
  drawCBitmap(_pd, getFloor(true), 11, 10); //Draw some more floor to cover the player
  drawCBitmap(_pd, getFloor(true), 11, 12);
  drawCBitmap(_pd, getFloor(true), 9, 12);

  if (getGameState() == kAwaitInput && getFrameCount() < ANIM_FPS/2) {
    drawCBitmap(_pd, &m_arrow_d, 9, 9);
  }

}

bool tickStairs(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position_x = 0;
    m_player.m_position_y = SIZE*9;
    addCluter(6, 5, 13);
    return false;
  }

  if (s_state == 0) { // start initial move
    enterRoom(&s_state);
  } else if (s_state == 1) { // initial move is done
    setGameState(kAwaitInput);
    ++s_state;
  } else if (s_state == 2) { // move onto stairs
    m_player.m_target_x = SIZE*9;
    m_player.m_target_y = SIZE*9;
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 3) { // move down stairs
    m_player.m_target_x = SIZE*11;
    m_player.m_target_y = SIZE*11;
    setGameState(kMovePlayer);
    //vibes_double_pulse();
    ++s_state;
  } else if (s_state == 4) {
    setGameState(kFadeOut);
  }

  return false;
}
