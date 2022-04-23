#include "empty.h"

static uint16_t s_state = 0;

void updateProcEmpty(GContext* _ctx) {
  renderFloor(_ctx, 0);
  renderClutter(_ctx);
  renderPlayer(_ctx);
  renderWalls(_ctx, true, true, true, true);
  renderWallClutter(_ctx);
  renderArrows(_ctx, 15, 4, 4);
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
    moveToExit(&s_state);
  } else if (s_state == 3) {
    setGameState(kFadeOut);
  }

  return false;
}
