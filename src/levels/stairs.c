#include "start.h"

static uint16_t s_state = 0;

void initStairs() {
  s_state = 0;
  m_player.m_position = GPoint(0, SIZE*9);
}

void updateProcStairs(Layer* _thisLayer, GContext* _ctx) {

  renderFloor(_thisLayer, _ctx, 0);
  drawBitmap(_ctx, m_stairs, 9, 8);
  drawBitmap(_ctx, m_stairs, 9, 10);

  renderPlayer(_thisLayer, _ctx);
  renderWalls(_thisLayer, _ctx, true, false, false, false);

  // If we are 'going down' we need to draw the floor again, over the player, with a hole
  if (s_state == 4) renderFloor(_thisLayer, _ctx, 1);

  if (m_gameState == kAwaitInput && m_frame < ANIM_FPS/2) {
    drawBitmap(_ctx, m_arrow, 10, 8);
  }

}

void tickStairs() {

  if (s_state == 0) { // start initial move
    m_player.m_target = GPoint(SIZE*3, SIZE*9);
    m_gameState = kMovePlayer;
    ++s_state;
  } else if (s_state == 1) { // initial move is done
    m_gameState = kAwaitInput;
    // do not increment state - done in click start
  } else if (s_state == 2) { // move onto stairs
    m_player.m_target = GPoint(SIZE*9, SIZE*9);
    m_gameState = kMovePlayer;
    ++s_state;
  } else if (s_state == 3) { // move down stairs
    m_player.m_target = GPoint(SIZE*12, SIZE*12);
    m_gameState = kMovePlayer;
    ++s_state;
  } else if (s_state == 4) {
    m_gameState = kFadeOut;
  }
}

void clickStairs(ButtonId _button) { // Choice entered - no choice here!
  ++s_state;
  m_gameState = kLevelSpecific;
}
