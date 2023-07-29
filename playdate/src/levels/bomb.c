#include "bomb.h"
#include "common.h"
#include "../sound.h"

static uint16_t s_state = 0;
static int8_t s_bomb = -1;
static int8_t s_bomb2 = -1;
static uint16_t s_tick = 0;
const static uint16_t s_ticks[3] = {80, 60, 40};


void updateProcBomb(PlaydateAPI* _pd) {
  renderFloor(_pd, 0);
  renderClutter(_pd);
  renderPlayer(_pd);
  renderWalls(_pd, true, true, true, true);
  renderWallClutter(_pd);
  renderBomb(_pd, s_tick/s_ticks[ m_dungeon.m_level ], s_bomb);
  if (m_dungeon.m_level == 2) renderBomb(_pd, s_tick/s_ticks[ m_dungeon.m_level ], s_bomb2);
  renderArrows(_pd, 15, 5, 4);
}

void bombTimer() {
  if (s_state >= 4) return; 

  // Time out
  if (++s_tick > s_ticks[ m_dungeon.m_level ] * 3) {
    s_state = 4;
    setGameState(kLevelSpecific);
  }

  // Ran into bomb
  if (getGameState() == kMovePlayer 
      && m_player.m_position_x > SIZE*9
      && (getPlayerChoice() == s_bomb
          || (m_dungeon.m_level == 2 
              && getPlayerChoice() == s_bomb2
              )
          )
      ) 
  {
    s_state = 4;
    s_tick = s_ticks[ m_dungeon.m_level ] * 3;
    setGameState(kLevelSpecific);
  }
}

bool tickBomb(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    s_tick = 0;
    s_bomb = rand() % 3;
    if (m_dungeon.m_level == 2) {
      s_bomb2 = s_bomb;
      while (s_bomb2 == s_bomb) { s_bomb2 = rand() % 3; }
    }
    m_player.m_position_x = 0;
    m_player.m_position_y = SIZE*9;
    addCluter(8, 0, 20); // Only left half
    fuseSound(true);
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
    fuseSound(false);
  } else if (s_state == 4) {
    m_dungeon.m_gameOver = 1;
    setGameState(kFadeOut);
    fuseSound(false);
    boomSound();
  }

  return s_tick % s_ticks[ m_dungeon.m_level ] == 0;
}
