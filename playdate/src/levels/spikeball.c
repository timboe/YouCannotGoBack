#include "spikeball.h"
#include "../sprite.h"
#include "../sound.h"

static uint16_t s_state = 0, s_frame = 0;

static uint16_t s_x[] = {4, 7, 12};
static uint16_t s_y[] = {5, 10, 15};
static float s_r[3][3] = {{0,0,0},{0,0,0},{0,0,0}}; // rotation
static uint16_t s_s[3][3] = {{0,0,0},{0,0,0},{0,0,0}}; // state

// activation order
static uint8_t s_a0[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
static uint8_t s_a1[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
static uint8_t s_a2[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
static uint8_t s_a3[3][3] = {{0,0,0},{0,0,0},{0,0,0}};


static uint8_t m_location = 1; // 0=top, 1=middle, 2=bottom

void renderSpikeballLower(PlaydateAPI* _pd, int8_t _x, int8_t _y) {
  if (_x>0) renderStandingStone(_pd, s_x[_x], s_y[_y], kColorBlack, kCircle);

  const uint16_t _status = s_s[_x][_y];
  if (_status > 35) {
    drawBitmapAbsRot(_pd, m_hole, s_x[_x] * SIZE, s_y[_y] * SIZE, s_r[_x][_y]);
  }
}

void renderSpikeballUpper(PlaydateAPI* _pd, int8_t _x, int8_t _y) {
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
  }
}


void updateProcSpikeball(PlaydateAPI* _pd) {
  renderFloor(_pd, 0);
  renderClutter(_pd);

  //Left two blocks, under other things
  drawCBitmap(_pd, &m_block, 13, 7);
  drawCBitmap(_pd, &m_block, 12, 7);
  drawCBitmap(_pd, &m_block, 13, 11);
  drawCBitmap(_pd, &m_block, 12, 11);

  for (int _x = 0; _x < 3; ++_x) {
    for (int _y = 0; _y < 3; ++_y) {
      renderSpikeballLower(_pd, _x, _y);
    }
  }

  renderPlayer(_pd);

  // Draw the walls on top of the player when exit/enter
  // but draw the spike balls on top when in the room
  bool doneWalls = false; 
  if (!(s_state <= 1 || s_state >= 8)) {
    doneWalls = true;
    renderWalls(_pd, true, true, true, true);
    renderWallClutter(_pd);
  }

  for (int _x = 0; _x < 3; ++_x) {
    for (int _y = 0; _y < 3; ++_y) {
      renderSpikeballUpper(_pd, _x, _y);
    }
  }

  if (!doneWalls) {
    renderWalls(_pd, true, true, true, true);
    renderWallClutter(_pd);
  }

  // Rightmost block, on top of wall
  drawCBitmap(_pd, &m_block, 14, 7);
  drawCBitmap(_pd, &m_block, 14, 11);

  // Movement arrows - always shown 
  if (s_state <= 3) {
    renderArrows2(_pd, 6, 4, 5, true, true, true, true);
  } else if (s_state <= 5) {
    renderArrows2(_pd, 11, 4, 5, m_location <= 1, 1, m_location >= 1, true);
  }

  // Danger warning
  // #define D_X 25
  // #define D_Y 40
  // if (s_frame < 8) {
  //   drawBitmapAbs(_pd, m_danger, D_X, D_Y);
  // } else if (s_frame < 16) {
  //   // noop
  // } else if (s_frame < 24) {
  //   drawBitmapAbs(_pd, m_danger, D_X, D_Y);
  // } else if (s_frame < 32) {
  //   // noop
  // } else if (s_frame < 40) {
  //   drawBitmapAbs(_pd, m_danger, D_X, D_Y);
  // }

}

bool tickSpikeball(PlaydateAPI* _pd, bool _doInit) {
  if (_doInit == true) {
    for (int _x = 0; _x < 3; ++_x) {
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

    // Choose spike drop order
    uint8_t r;
    const uint8_t _level = m_dungeon.m_level;
    if (_level == 0) {
      // Slow Left
      s_a0[1][rand() % 3] = 1;

      r = rand() % 3;
      while (s_a0[1][r]) r = rand() % 3;
      s_a1[1][r] = 1;

      r = rand() % 3;
      while (s_a0[1][r] || s_a1[1][r]) r = rand() % 3;
      s_a2[1][r] = 1;
    } else {
      // Fast Left
      s_a0[1][rand() % 3] = 1;
      r = rand() % 3;
      while (s_a0[1][r]) r = rand() % 3;
      s_a0[1][r] = 1;

      r = rand() % 3;
      while (s_a0[1][r] || s_a0[1][r]) r = rand() % 3;
      s_a1[1][r] = 1;
    }

    if (_level < 2) {
      // Slow Right
      s_a1[2][rand() % 3] = 1;

      r = rand() % 3;
      while (s_a1[2][r]) r = rand() % 3;
      s_a2[2][r] = 1;

      r = rand() % 3;
      while (s_a1[2][r] || s_a2[2][r]) r = rand() % 3;
      s_a3[2][r] = 1;
    } else {
      // Fast Right
      // If we also are "Fast Left", and the middle is gone, then we must force the middle
      // to be kept here else we can reach an unwinable situatuon
      const bool keepMiddleFree = s_a0[1][1];
      if (keepMiddleFree) {
        s_a1[2][0] = 1;
        s_a1[2][2] = 1;
      } else {
        s_a1[2][rand() % 3] = 1;
        r = rand() % 3;
        while (s_a1[2][r]) r = rand() % 3;
        s_a1[2][r] = 1;
      }

      r = rand() % 3;
      while (s_a1[2][r] || s_a1[2][r]) r = rand() % 3;
      s_a2[2][r] = 1;
    }

    return false;
  }

  // Spike animation control loop
  s_frame++;

  uint8_t (*_activate)[3][3];
  _activate = NULL;
  // Was 50 when there was a warning too
  #define F_START 10
  switch (s_frame) {
    case F_START: s_state = 1; _activate = &s_a0; break;
    case F_START+(1*22): _activate = &s_a1; break;
    case F_START+(3*22): _activate = &s_a2; break;
    case F_START+(5*22): _activate = &s_a3; break;
    case F_START+(7*22): s_s[0][1] = 1; break; // In case the player didn't move
  }
  if (_activate) {
    #ifdef DEV
    _pd->system->logToConsole("Activate");
    #endif
    for (int _x = 0; _x < 3; ++_x) {
      for (int _y = 0; _y < 3; ++_y) {
        if ((*_activate)[_x][_y]) s_s[_x][_y] = 1;
      }
    }
  }

  // Progress
  for (int _x = 0; _x < 3; ++_x) {
    for (int _y = 0; _y < 3; ++_y) {
      if (s_s[_x][_y]) s_s[_x][_y]++;
    }
  }

  // Check death
  for (int _x = 0; _x < 3; ++_x) {
    for (int _y = 0; _y < 3; ++_y) {
      if (s_s[_x][_y] >= 32) {
        const float _dx = m_player.m_position_x - (s_x[_x]-1)*SIZE;
        const float _dy = m_player.m_position_y - (s_y[_y]-1)*SIZE;
        const float _d = sqrt( _dx*_dx + _dy*_dy );
        if (_d < 6.0f && s_state != 99) {
          m_dungeon.m_gameOver = 1;
          setGameState(kFadeOut);
          s_state == 99;
          m_dungeon.m_spinningDeath = true;
          fallSound();
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
    m_player.m_target_x = (s_x[1]-1)*SIZE;
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

    m_player.m_target_x = (s_x[2]-1)*SIZE;
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
