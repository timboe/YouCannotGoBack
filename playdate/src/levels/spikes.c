#include "spikes.h"

static uint16_t s_state = 0;


void updateProcSpikes(PlaydateAPI* _pd) {
  renderFloor(_pd, 2 /*mode spikes*/);
  renderClutter(_pd);
  renderPlayer(_pd);
  renderWalls(_pd, true, false, true, false);
  renderWallClutter(_pd);
  if (getFrameCount() < ANIM_FPS/2) {
    switch (s_state) {
      case 2: drawCBitmap(_pd, &m_arrow_r,  7, 9); break;
      case 4: drawCBitmap(_pd, &m_arrow_r, 11, 9); break;
      case 6: drawCBitmap(_pd, &m_arrow_r, 15, 9); break;
    }
  }
}

bool tickSpikes(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position_x = 0;
    m_player.m_position_y = SIZE*9;
    addCluter(4, 0, 20); // Only left

    return false;
  }

  if (s_state == 0) { // start initial move
    enterRoom(&s_state);
  } else if (s_state == 1) { // initial move is done
    resetPlayerChoice();
    setGameState(kLevelSpecific);
    s_state = 2; 
  } else if (s_state == 10) {
    setGameState(kFadeOut);
  } else {
    setGameState(kLevelSpecificWButtons);
    
    if (s_state % 2 == 0 && getPlayerChoice() == 1) {
      ++s_state;
      resetPlayerChoice();
    }

    if (s_state % 2 == 1) {
      if (getFrameCount() % 3 == 0 && ++m_player.m_playerFrame == MAX_FRAMES) m_player.m_playerFrame = 0;
      m_player.m_position_x += PLAYER_SPEED;
      switch (s_state) {
        case 3: if (m_player.m_position_x > SIZE*7  - SIZE/2) ++s_state; break;
        case 5: if (m_player.m_position_x > SIZE*11 - SIZE/2) ++s_state; break;
        case 7: if (m_player.m_position_x > SIZE*14 - SIZE/2) s_state += 2; break; // Don't want to wait for user-input here
        case 9: if (m_player.m_position_x > SIZE*17) ++s_state; break;
      }
    }

  }

  return true;
}
