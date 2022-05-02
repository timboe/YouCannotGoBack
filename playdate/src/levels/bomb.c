#include "bomb.h"
#include "../sound.h"

static uint16_t s_state = 0;
static int8_t s_bomb = -1;
static uint16_t s_tick = 0;
const static s_ticks[3] = {60, 40, 20};


void updateProcBomb(PlaydateAPI* _pd) {
  renderFloor(_pd, 0);
  renderClutter(_pd);
  renderPlayer(_pd);
  renderWalls(_pd, true, true, true, true);
  renderWallClutter(_pd);
  rednerBomb(_pd, s_tick/s_ticks[ m_dungeon.m_level ], s_bomb);
  renderArrows(_pd, 15, 4, 4);
}

bool tickBomb(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position_x = 0;
    m_player.m_position_y = SIZE*9;
    addCluter(15, 20, 0);
    s_bomb = rand() % 3;
    fuseSound(true);
    return false;
  }

  // TODO - THIS WILL NOT WORK HERE... REFACTOR THIS
  // Time out
  if (++s_tick > s_ticks[ m_dungeon.m_level ] * 3) {
    s_state = 4;
  }

  // Ran into bomb
  // TODO - THIS WILL NOT WORK HERE... REFACTOR INTO TWO MOVES
  if (getGameState() == kMovePlayer 
    && m_player.m_position_x > SIZE*6
    && getPlayerChoice() == s_bomb) 
  {
    s_state = 4;
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

  return false;
}
