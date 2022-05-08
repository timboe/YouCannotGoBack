#include "death.h"
#include "../sound.h"

static uint16_t s_state = 0;

void updateProcDeath(PlaydateAPI* _pd) {
  renderPit(_pd);
  renderPlayer(_pd);
  renderWalls(_pd, true, false, false, false);
}

bool tickDeath(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position_x = 0;
    m_player.m_position_y = SIZE*9;
    return false;
  }

  if (s_state == 0) { // start initial move
    enterRoom(&s_state);
  } else if (s_state == 1) { // initial move is done
    m_dungeon.m_gameOver = 1;
    m_dungeon.m_fallingDeath = true;
    fallSound();
    setGameState(kFadeOut);
    ++s_state;
  }

  return false;
}
