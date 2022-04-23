#include "death.h"

static uint16_t s_state = 0;

void updateProcDeath(GContext* _ctx) {
  renderPit(_ctx);
  renderPlayer(_ctx);
  renderWalls(_ctx, true, false, false, false);
}

bool tickDeath(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position = GPoint(0, SIZE*9);
    return false;
  }

  if (s_state == 0) { // start initial move
    enterRoom(&s_state);
  } else if (s_state == 1) { // initial move is done
    m_dungeon.m_gameOver = 1;
    m_dungeon.m_fallingDeath = true;
    vibes_long_pulse();
    setGameState(kFadeOut);
    ++s_state;
  }

  return false;
}
