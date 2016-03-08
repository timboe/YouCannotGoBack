#include "empty.h"

static uint16_t s_state = 0;

void updateProcEmpty(GContext* _ctx) {

  renderFloor(_ctx, 0);

  renderPlayer(_ctx);
  renderWalls(_ctx, true, true, true, true);
  renderClutter(_ctx);

  if (getGameState() == kAwaitInput && getFrameCount() < ANIM_FPS/2) {
    drawBitmap(_ctx, m_arrow, 15, 4);
    drawBitmap(_ctx, m_arrow, 15, 8);
    drawBitmap(_ctx, m_arrow, 15, 12);
  }

}

bool tickEmpty(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position = GPoint(0, SIZE*9);
    addCluter(20, 20, 0);
    return false;
  }

  if (s_state == 0) { // start initial move
    enterRoom(&s_state);
  } else if (s_state == 1) { // initial move is done
    setGameState(kAwaitInput);
    ++s_state;
  } else if (s_state == 2) {
    switch (getPlayerChoice()) {
      case 0: m_player.m_target = GPoint(SIZE*16, SIZE*5); break;
      case 1: m_player.m_target = GPoint(SIZE*16, SIZE*9); break;
      case 2: m_player.m_target = GPoint(SIZE*16, SIZE*13); break;
    }
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 3) {
    setGameState(kFadeOut);
  }

  return false;
}
