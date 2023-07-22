#include "spikeball.h"
#include "../sprite.h"
#include "../sound.h"

static uint16_t s_state = 0, s_frame = 0;

static uint16_t s_x[] = {7, 12};
static uint16_t s_y[] = {5, 10, 15};
static float s_r[2][3] = {{0,0,0},{0,0,0}}; // rotation
static uint16_t s_s[2][3] = {{0,0,0},{0,0,0}}; // state

// activation order
static uint8_t s_a0[2][3] = {{0,0,0},{0,0,0}};
static uint8_t s_a1[2][3] = {{0,0,0},{0,0,0}};
static uint8_t s_a2[2][3] = {{0,0,0},{0,0,0}};
static uint8_t s_a3[2][3] = {{0,0,0},{0,0,0}};


static uint8_t m_location = 1; // 0=top, 1=middle, 2=bottom

void renderSpikeball(PlaydateAPI* _pd, int8_t _x, int8_t _y) {

  renderStandingStone(_pd, s_x[_x], s_y[_y], kColorBlack);

  const uint16_t _status = s_s[_x][_y];
  if (_status == 0) {
    // noop
  } else if (_status <= 2) {
    drawBitmap(_pd, m_target, s_x[_x]-2, s_y[_y]-2);
  } else if (_status <= 4) {
    // noop
  } else if (_status <= 6) {
    drawBitmap(_pd, m_target, s_x[_x]-2, s_y[_y]-2);
  } else if (_status <= 8) {
    // noop
  } else if (_status <= 10) {
    drawBitmap(_pd, m_target, s_x[_x]-2, s_y[_y]-2);
  } else if (_status <= 12) {
    // noop
  } else if (_status <= 14) {
    drawBitmap(_pd, m_target, s_x[_x]-2, s_y[_y]-2);
  } else if (_status <= (14 + 16)) {
    const int16_t _y_off = 32 - (_status + 10) * 2;
    drawBitmapAbs(_pd, m_ball, (s_x[_x]-2) * SIZE, (s_y[_y]-8) * SIZE - _y_off);
  } else if (_status <= 32) {
    drawBitmap(_pd, m_bomb[3], s_x[_x]-4, s_y[_y]-4);
  } else if (_status <= 34) {
    _pd->graphics->setDrawMode(kDrawModeInverted);
    drawBitmap(_pd, m_bomb[3], s_x[_x]-4, s_y[_y]-4);
    _pd->graphics->setDrawMode(kDrawModeCopy);
  } else {
    drawBitmapAbsRot(_pd, m_hole, s_x[_x] * SIZE, s_y[_y] * SIZE, s_r[_x][_y]);
  }

}


void updateProcSpikeball(PlaydateAPI* _pd) {
  renderFloor(_pd, 0);

  for (int _x = 0; _x < 2; ++_x) {
    for (int _y = 0; _y < 3; ++_y) {
      renderSpikeball(_pd, _x, _y);
    }
  }

  drawCBitmap(_pd, &m_block, 13, 7);
  drawCBitmap(_pd, &m_block, 12, 7);

  drawCBitmap(_pd, &m_block, 13, 11);
  drawCBitmap(_pd, &m_block, 12, 11);

  renderClutter(_pd);
  renderPlayer(_pd);
  renderWalls(_pd, true, true, true, true);
  renderWallClutter(_pd);

  drawCBitmap(_pd, &m_block, 14, 7);
  drawCBitmap(_pd, &m_block, 14, 11);

  if (s_state <= 2) {
    renderArrows(_pd, 6, 4, 5);
  } else if (s_state <= 4) {
    renderArrows2(_pd, 11, 4, 5, m_location <= 1, 1, m_location >= 1);
  }

}

bool tickSpikeball(PlaydateAPI* _pd, bool _doInit) {
  if (_doInit == true) {
    for (int _x = 0; _x < 2; ++_x) {
      for (int _y = 0; _y < 3; ++_y) {
        s_s[_x][_y] = 0;
        s_r[_x][_y] = rand() % 360;
        s_a0[_x][_y] = 0;
        s_a1[_x][_y] = 0;
        s_a2[_x][_y] = 0;
        s_a3[_x][_y] = 0;
      }
    } 
    s_state = 0;
    s_frame = 0;
    m_player.m_position_x = 0;
    m_player.m_position_y = SIZE*9;
    m_player.m_target_x = SIZE*3;
    m_player.m_target_y = SIZE*9;
    addCluter(4, 0, 20); // Only left

    uint8_t r;
    if (m_dungeon.m_level == 0) {
      // Left
      s_a0[0][rand() % 3] = 1;

      r = rand() % 3;
      while (s_a0[0][r]) r = rand() % 3;
      s_a1[0][r] = 1;

      r = rand() % 3;
      while (s_a0[0][r] || s_a1[0][r]) r = rand() % 3;
      s_a2[0][r] = 1;

      // Right
      s_a1[1][rand() % 3] = 1;

      r = rand() % 3;
      while (s_a1[1][r]) r = rand() % 3;
      s_a2[1][r] = 1;

      r = rand() % 3;
      while (s_a1[1][r] || s_a2[1][r]) r = rand() % 3;
      s_a3[1][r] = 1;
    }

    return false;
  }

  // Spike animation control loop
  s_frame++;

  uint8_t (*_activate)[2][3];
  _activate = NULL;
  switch (s_frame) {
    case 16: s_state = 1; _activate = &s_a0; break;
    case 16+(1*22): _activate = &s_a1; break;
    case 16+(2*22): _activate = &s_a2; break;
    case 16+(3*22): _activate = &s_a3; break;
  }
  if (_activate) {
    #ifdef DEV
    _pd->system->logToConsole("Activate");
    #endif
    for (int _x = 0; _x < 2; ++_x) {
      for (int _y = 0; _y < 3; ++_y) {
        if ((*_activate)[_x][_y]) s_s[_x][_y] = 1;
      }
    }
  }

  // Progress
  for (int _x = 0; _x < 2; ++_x) {
    for (int _y = 0; _y < 3; ++_y) {
      if (s_s[_x][_y]) s_s[_x][_y]++;
    }
  }

  // Check death
  for (int _x = 0; _x < 2; ++_x) {
    for (int _y = 0; _y < 3; ++_y) {
      if (s_s[_x][_y] >= 32) {
        const float _dx = m_player.m_position_x - (s_x[_x]-1)*SIZE;
        const float _dy = m_player.m_position_y - (s_y[_y]-1)*SIZE;
        const float _d = sqrt( _dx*_dx + _dy*_dy );
        if (_d < 6.0f && s_state != 99) {
          m_dungeon.m_gameOver = 1;
          setGameState(kFadeOut);
          s_state == 99;
          if (s_s[_x][_y] > 34) { // Explosion finished
            m_dungeon.m_fallingDeath = true;
            fallSound();
          }
        }
      }
    }
  }

  // Player control
  if (s_state == 1) {
    // Manual move
    movePlayer();
    if (atDestination()) s_state = 2; // move complete

  } else if (s_state == 2) {
    resetPlayerChoice();
    setGameState(kLevelSpecificWButtons);
    s_state = 3; 

  } else if (s_state == 3 && getPlayerChoice() != -1) {
    // Initial move
    m_player.m_target_x = (s_x[0]-1)*SIZE;
    switch (getPlayerChoice()) {
      case 0: m_player.m_target_y = (s_y[0]-1)*SIZE; m_location = 0; break;
      case 1: m_player.m_target_y = (s_y[1]-1)*SIZE; m_location = 1; break;
      case 2: m_player.m_target_y = (s_y[2]-1)*SIZE; m_location = 2; break;
    }
    resetPlayerChoice();
    s_state = 4;

  } else if (s_state == 4) {
    // Manual move two
    movePlayer();
    if (atDestination()) {
      setGameState(kLevelSpecificWButtons);
      s_state = 5; // move complete
    }

  } else if (s_state == 5 && getPlayerChoice() != -1) {
    // Second move

    // Forbid moving two spaces
    int _pc = getPlayerChoice(); 
    if ((_pc == 0 && m_location == 2) || (_pc == 2 && m_location == 0)) {
      _pc = 1; // Set to the middle instead
    }

    m_player.m_target_x = (s_x[1]-1)*SIZE;
    switch (_pc) {
      case 0: m_player.m_target_y = (s_y[0]-1)*SIZE; m_location = 0; break;
      case 1: m_player.m_target_y = (s_y[1]-1)*SIZE; m_location = 1; break;
      case 2: m_player.m_target_y = (s_y[2]-1)*SIZE; m_location = 2; break;
    }
    resetPlayerChoice();
    s_state = 6;

  } else if (s_state == 6) {
    // Manual move three
    movePlayer();
    if (atDestination()) s_state = 7; // move complete

  } else if (s_state == 7) {
    // Set end destination

    m_player.m_target_x = SIZE*17;
    switch (m_location) {
      case 0: m_player.m_target_y = SIZE*5; break;
      case 1: m_player.m_target_y = SIZE*9; break;
      case 2: m_player.m_target_y = SIZE*13; break;
    }
    s_state = 8; 

  } else if (s_state == 8) {
    // Manual move four
    movePlayer();
    if (atDestination()) s_state = 9; // move complete

  } else if (s_state == 9) {
    setGameState(kFadeOut);
  }

  return true;
}
