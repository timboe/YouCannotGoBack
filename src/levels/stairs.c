#include "start.h"

static uint16_t s_state = 0;

void updateProcStairs(GContext* _ctx) {

  renderFloor(_ctx, 0);
  drawBitmap(_ctx, m_stairs, 9, 8);
  drawBitmap(_ctx, m_stairs, 9, 10);
  renderClutter(_ctx);
  renderPlayer(_ctx);
  renderWalls(_ctx, true, false, false, false);
  renderWallClutter(_ctx);
  drawBitmap(_ctx, getFloor(true), 11, 10); //Draw some more floor to cover the player
  drawBitmap(_ctx, getFloor(true), 11, 12);
  drawBitmap(_ctx, getFloor(true), 9, 12);

  if (getGameState() == kAwaitInput && getFrameCount() < ANIM_FPS/2) {
    drawBitmap(_ctx, m_arrow, 9, 8);
  }

}

bool tickStairs(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position = GPoint(0, SIZE*9);
    addCluter(6, 5, 13);
    return false;
  }

  if (s_state == 0) { // start initial move
    enterRoom(&s_state);
  } else if (s_state == 1) { // initial move is done
    setGameState(kAwaitInput);
    ++s_state;
  } else if (s_state == 2) { // move onto stairs
    m_player.m_target = GPoint(SIZE*9, SIZE*9);
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 3) { // move down stairs
    m_player.m_target = GPoint(SIZE*11, SIZE*11);
    setGameState(kMovePlayer);
    vibes_double_pulse();
    ++s_state;
  } else if (s_state == 4) {
    setGameState(kFadeOut);
  }

  return false;
}
