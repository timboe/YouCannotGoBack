#include "bomb.h"
#ifdef YCGBv2

static uint16_t s_state = 0;
static int8_t s_bomb = -1;
static int8_t s_bomb2 = -1;
static uint8_t s_tick = 0;
const uint8_t s_ticks[3] = {80, 60, 40};

// #define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

void updateProcBomb(GContext* _ctx) {
  renderFloor(_ctx, 0);
  renderClutter(_ctx);
  renderPlayer(_ctx);
  renderWalls(_ctx, true, true, true, true);
  renderWallClutter(_ctx);
  renderBomb(_ctx, s_tick/s_ticks[ MIN(2, m_dungeon.m_difficulty) ], s_bomb);

  if (m_dungeon.m_difficulty >= 2) renderBomb(_ctx, s_tick/s_ticks[ MIN(2, m_dungeon.m_difficulty) ], s_bomb2);
  renderArrows(_ctx, 15, 5, 4);
}

void bombTimer() {
  if (s_state >= 4) return; 

  // Time out
  if (++s_tick > s_ticks[ MIN(2, m_dungeon.m_difficulty) ] * 3) {
    s_state = 4;
    vibes_long_pulse();
    return setGameState(kLevelSpecific);
  }

  // Ran into bomb
  if (getGameState() == kMovePlayer 
      && m_player.m_position.x > SIZE*9
      && (getPlayerChoice() == s_bomb
          || (m_dungeon.m_difficulty >= 2 
              && getPlayerChoice() == s_bomb2
              )
          )
      ) 
  {
    s_state = 4;
    s_tick = s_ticks[ MIN(2, m_dungeon.m_difficulty) ] * 3;
    vibes_long_pulse();
    setGameState(kLevelSpecific);
  }
}

bool tickBomb(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    s_tick = 0;
    s_bomb = rand() % 3;
    if (m_dungeon.m_difficulty >= 2) {
      s_bomb2 = s_bomb;
      while (s_bomb2 == s_bomb) { s_bomb2 = rand() % 3; }
    }
    m_player.m_position = GPoint(0, SIZE*9);
    addCluter(8, 0, 20); // Only left half
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
  } else if (s_state == 4) {
    m_dungeon.m_gameOver = 1;
    setGameState(kFadeOut);
  }

  return s_tick % s_ticks[ MIN(2, m_dungeon.m_difficulty) ] == 0;
}

#endif