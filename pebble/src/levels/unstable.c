#include "unstable.h"
#ifdef YCGBv2

static uint16_t s_state = 0;
static int8_t s_offsetX[6][8] = {{0},{0},{0},{0},{0},{0}};
static int8_t s_offsetY[6][8] = {{0},{0},{0},{0},{0},{0}};
static int16_t s_timer[6][8] = {{0},{0},{0},{0},{0},{0}};
static uint8_t s_choice[2] = {0};
static uint8_t s_playerChoice = 0;
static bool s_extraMove = false;

static int8_t s_S0 = -20; 
static int8_t s_S1 = -40; 
static int8_t s_S2 = -60; 
static int8_t s_S3 = -80; 

void updateProcUnstable(GContext* _ctx) {
  renderPit(_ctx);
  if (s_state >= 8) renderPlayer(_ctx);
  renderFloor(_ctx, kUnstable, -1, -1, s_offsetX, s_offsetY);
  rednerUnstableMarkers(_ctx);
  if (s_state < 8) renderPlayer(_ctx);
  renderWalls(_ctx, true, true, true, true);
  renderWarning(_ctx);
  if (s_state == 2) {
    renderArrows(_ctx, 9, 3, 12);
  } else if (s_state == 5 && s_playerChoice == 2) {
    renderArrowsDetailed(_ctx, 15, 4, 4, false, true, true, false);
  } else if (s_state == 5 && s_playerChoice == 0) {
    renderArrowsDetailed(_ctx, 15, 4, 4, true, true, false, false);
  }
}

bool tickUnstable(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    s_extraMove = false;
    m_player.m_position = GPoint(0, SIZE*9);
    for (uint8_t _x = 0; _x < 6; ++_x) {
      for (uint8_t _y = 0; _y < 8; ++_y) {
        s_timer[_x][_y] = -32000;
        s_offsetX[_x][_y] = 0;
        s_offsetY[_x][_y] = 0;
      }
    }
    s_choice[0] = rand() % 2;
    s_choice[1] = rand() % 2;
    if (m_dungeon.m_difficulty == 0) {
      s_S0 = -40;
      s_S1 = -70;
      s_S2 = -100;
      s_S3 = -128;
    } else if (m_dungeon.m_difficulty == 1) {
      s_S0 = -30;
      s_S1 = -55;
      s_S2 = -80;
      s_S3 = -105;
    } else {
      s_S0 = -20; 
      s_S1 = -40; 
      s_S2 = -60; 
      s_S3 = -80; 
    }
    // Set entry pattern 
    s_timer[0][0] = s_S0; s_timer[1][0] = s_S1; s_timer[2][0] = s_S2;
    s_timer[0][1] = s_S1; s_timer[1][1] = s_S2;
    s_timer[0][2] = s_S2;
    s_timer[0][3] = s_S3;
    s_timer[0][4] = s_S3;
    s_timer[0][5] = s_S2;
    s_timer[0][6] = s_S1; s_timer[1][6] = s_S2;
    s_timer[0][7] = s_S0; s_timer[1][7] = s_S1; s_timer[2][7] = s_S2;
    // Set initial pattern
    if (s_choice[0]) {
      s_timer[3][1] = -5;
      s_timer[3][0] = -30;
      s_timer[2][1] = -30;
      s_timer[2][2] = -30;
      s_timer[1][3] = -50;
      s_timer[1][2] = -50;
    } else {
      s_timer[3][6] = -30;
      s_timer[3][7] = -5;
      s_timer[2][6] = -30;
      s_timer[2][5] = -30;
      s_timer[1][5] = -50;
      s_timer[1][4] = -50;
    }
    return false;
  }

  if (s_state == 0) { // start initial move
    enterRoom(&s_state);
  } else if (s_state == 1) { // initial move is done
    setGameState(kLevelSpecificWButtons);
    ++s_state;
  } else if (s_state == 2) {
     switch (getPlayerChoice()) {
       case 0: m_player.m_target = GPoint(SIZE*9, SIZE*3); ++s_state; s_playerChoice = 0; break;
       case 1: resetPlayerChoice(); setGameState(kLevelSpecificWButtons); break;
       case 2: m_player.m_target = GPoint(SIZE*9, SIZE*15); ++s_state; s_playerChoice = 2; break;
     }
  } else if (s_state == 3) {
    movePlayer();
    if (atDestination()) {
      setGameState(kLevelSpecific);
      resetPlayerChoice();
      ++s_state;
    }
  } else if (s_state == 4) {
    // Set exit pattern
    s_timer[5][0] = s_S0;
    s_timer[5][7] = s_S0;
    s_timer[4][0] = s_S1;
    s_timer[4][7] = s_S1;
    if (s_choice[0]) {
      s_timer[3][7] = s_S2;
      s_timer[3][6] = s_S2;
      // set second round
      if (s_choice[1]) {
        s_timer[5][3] = -1;
        s_timer[5][4] = -1;
      } else {
        s_timer[5][5] = -1;
        s_timer[5][6] = -1;
      }
      s_timer[5][1] = -1;
      s_timer[5][2] = -1;
    } else {
      s_timer[3][0] = s_S2;
      s_timer[3][1] = s_S2;
      // set second round
      if (s_choice[1]) {
        s_timer[5][1] = -1;
        s_timer[5][2] = -1;
      } else {
        s_timer[5][3] = -1;
        s_timer[5][4] = -1;
      }
      s_timer[5][5] = -1;
      s_timer[5][6] = -1;
    }
    setGameState(kLevelSpecificWButtons);
    ++s_state;
  } else if (s_state == 5) {
    if (s_playerChoice == 2) { 
      switch (getPlayerChoice()) {
        case 0:
        case 1: m_player.m_target = GPoint(SIZE*11, SIZE*9); ++s_state; s_extraMove = true; break; // Needs another step
        case 2: m_player.m_target = GPoint(SIZE*17, SIZE*13); ++s_state; break;
      }
    } else { 
      switch (getPlayerChoice()) {
        case 0: m_player.m_target = GPoint(SIZE*17, SIZE*5); ++s_state; break;
        case 1: case 2: m_player.m_target = GPoint(SIZE*11, SIZE*9); s_extraMove = true; ++s_state; break; // Needs another step
      }
    } 
  } else if (s_state == 6) {
    movePlayer();
    if (atDestination()) {
      setGameState(kLevelSpecific);
      resetPlayerChoice();
      ++s_state;
    }
  } else if (s_state == 7) {
    if (s_extraMove) {
      s_extraMove = false;
      m_player.m_target = GPoint(SIZE*17, SIZE*9);
      --s_state;
    } else {
      setGameState(kFadeOut);
    }
  } else if (s_state == 8) {
    m_dungeon.m_gameOver = 1;
    m_dungeon.m_fallingDeath = true;
    vibes_long_pulse();
    setGameState(kFadeOut);
    ++s_state;
  }

  for (uint8_t _x = 0; _x < 6; ++_x) for (uint8_t _y = 0; _y < 8; ++_y) ++s_timer[_x][_y];

  #define START_WOBBLE 0
  #define INCREASE_WOBBLE 20
  #define FLOOR_FALLS 30

  if (m_dungeon.m_ticksInLevel % 2 == 0) {
    srand(m_dungeon.m_ticksInLevel);
    for (uint8_t _x = 0; _x < 6; ++_x) {
      for (uint8_t _y = 0; _y < 8; ++_y) {
        if (++s_timer[_x][_y] > START_WOBBLE) {
          if (s_timer[_x][_y] > FLOOR_FALLS) {
            s_offsetX[_x][_y] = 64;
          } else {
            const int8_t _range = (s_timer[_x][_y] > INCREASE_WOBBLE ? 4 : 2);
            s_offsetX[_x][_y] = rand() % _range - (_range/2);
            s_offsetY[_x][_y] = rand() % _range - (_range/2);
          }
        }
      }
    }
  }

  const GPoint _player = GPoint((m_player.m_position.x - 3*SIZE)/(2*SIZE), (m_player.m_position.y - 2*SIZE)/(2*SIZE) + 1);
  if (_player.x >= 0 && _player.x < 6 && s_timer[_player.x][_player.y] > FLOOR_FALLS && s_state < 8) {
    if      (_player.x == 0 && _player.y == 5) {}
    else if (_player.x == 1 && _player.y == 6) {}
    else if (_player.x == 2 && _player.y == 7) {}
    else s_state = 8;
  }

  return true;
}


#endif