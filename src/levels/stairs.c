#include "start.h"

static uint16_t s_state = 0;

void initStairs() {
  s_state = 0;
  m_player.m_position = GPoint(0, SIZE*9);
  addCluter(5, 6, 5, 13);
}

void updateProcStairs(GContext* _ctx) {

  renderFloor(_ctx, 0);
  drawBitmap(_ctx, m_stairs, 9, 8);
  drawBitmap(_ctx, m_stairs, 9, 10);

  renderPlayer(_ctx);
  renderWalls(_ctx, true, false, false, false);
  drawBitmap(_ctx, getFloor(), 11, 10); //Draw some more floor to cover the player
  drawBitmap(_ctx, getFloor(), 11, 12);
  drawBitmap(_ctx, getFloor(), 9, 12);
  renderClutter(_ctx);

  if (getGameState() == kAwaitInput && getFrameCount() < ANIM_FPS/2) {
    drawBitmap(_ctx, m_arrow, 9, 8);
  }

}

void tickStairs() {

  if (s_state == 0) { // start initial move
    enterRoom(&s_state);
  } else if (s_state == 1) { // initial move is done
    setGameState(kAwaitInput);  // do not increment state - done in click start
  } else if (s_state == 2) { // move onto stairs
    m_player.m_target = GPoint(SIZE*9, SIZE*9);
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 3) { // move down stairs
    m_player.m_target = GPoint(SIZE*11, SIZE*11);
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 4) {
    setGameState(kFadeOut);
  }
}

void clickStairs(ButtonId _button) { // Choice entered - no choice here!
  ++s_state;
  setGameState(kLevelSpecific);
}
