#include "pword.h"

static uint16_t s_state = 0;
static uint16_t s_choice = 0;

void initPword() {
  s_state = 0;
  m_player.m_position = GPoint(0, SIZE*9);
  addCluter(2, 5, 0, 20);
}

void updateProcPword(GContext* _ctx) {

  renderFloor(_ctx, 0);
  drawBitmap(_ctx, m_guardian, 5, 6);

  renderPlayer(_ctx);
  renderWalls(_ctx, true, false, false, false);

  drawBitmap(_ctx, m_blockWall[0], 11, 0);
  for (int _i = 2; _i < 18; _i += 2) drawBitmap(_ctx, m_blockWall[1], 11, _i);
  drawBitmap(_ctx, m_blockWall[2], 11, 18);

  renderClutter(_ctx);

  // if (getGameState() == kAwaitInput && getFrameCount() < ANIM_FPS/2) {
  //   drawBitmap(_ctx, m_arrow, 9, 8);
  // }

}

void tickPword() {

  if (s_state == 0) { // start initial move
    enterRoom(&s_state);
  } else if (s_state == 1) { // initial move is done
    m_player.m_target = GPoint(SIZE*11, SIZE*11);
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 2) { // move onto stairs
    setGameState(kAwaitInput);  // do not increment state - done in click start
  } else if (s_state == 3) { // move down stairs
    //do desc.
  } else if (s_state == 4) {
    setGameState(kFadeOut);
  }
}

void clickPword(ButtonId _button) { // Choice entered - no choice here!
  ++s_state;
  setGameState(kLevelSpecific);

  if (BUTTON_ID_UP == _button) s_choice = 0;
  else if (BUTTON_ID_SELECT == _button) s_choice = 1;
  else if (BUTTON_ID_DOWN == _button) s_choice = 2;
}
