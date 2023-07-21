#include "spikeball.h"
#include "../sprite.h"

static uint16_t s_state = 0;

static uint16_t s_x[] = {7*SIZE, 12*SIZE};
static uint16_t s_y[] = {5*SIZE, 10*SIZE, 15*SIZE};
static float s_r[] = {0,0,0,0,0,0};

static uint8_t m_location = 1; // 0=top, 1=middle, 2=bottom


void updateProcSpikeball(PlaydateAPI* _pd) {
  renderFloor(_pd, 0);
  drawBitmapAbsRot(_pd, m_hole, s_x[0], s_y[0], s_r[0]);
  drawBitmapAbsRot(_pd, m_hole, s_x[0], s_y[1], s_r[1]);
  drawBitmapAbsRot(_pd, m_hole, s_x[0], s_y[2], s_r[2]);

  drawBitmapAbsRot(_pd, m_hole, s_x[1], s_y[0], s_r[3]);
  drawBitmapAbsRot(_pd, m_hole, s_x[1], s_y[1], s_r[4]);
  drawBitmapAbsRot(_pd, m_hole, s_x[1], s_y[2], s_r[5]); 

  renderClutter(_pd);
  renderPlayer(_pd);
  renderWalls(_pd, true, true, true, true);
  renderWallClutter(_pd);

  if (s_state <= 2) {
    renderArrows(_pd, 6, 4, 5);
  } else if (s_state <= 4) {
    renderArrows2(_pd, 11, 4, 5, m_location <= 1, 1, m_location >= 1);
  } else if (s_state <= 6) {
    renderArrows2(_pd, 15, 5, 4, m_location <= 1, 1, m_location >= 1);
  }
 

}

bool tickSpikeball(PlaydateAPI* _pd, bool _doInit) {
  if (_doInit == true) {
    for (int _i = 0; _i < 6; ++_i) s_r[_i] = rand() % 360; 
    s_state = 0;
    m_player.m_position_x = 0;
    m_player.m_position_y = SIZE*9;
    addCluter(4, 0, 20); // Only left
    return false;
  }


  if (s_state == 0) {
    enterRoom(&s_state);

  } else if (s_state == 1) {
    setGameState(kAwaitInput);
    ++s_state;

  } else if (s_state == 2) {
    m_player.m_target_x = s_x[0] - SIZE;
    switch (getPlayerChoice()) {
      case 0: m_player.m_target_y = SIZE*4; m_location = 0; break;
      case 1: m_player.m_target_y = SIZE*9; m_location = 1; break;
      case 2: m_player.m_target_y = SIZE*14; m_location = 2; break;
    }
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 3) {
    setGameState(kAwaitInput);
    ++s_state;

  } else if (s_state == 4) {
    // Forbid moving two
    if ((getPlayerChoice() == 0 && m_location == 2) || (getPlayerChoice() == 2 && m_location == 0)) {
      --s_state;
      return false;
    } 

    m_player.m_target_x = s_x[1] - SIZE;
    switch (getPlayerChoice()) {
      case 0: m_player.m_target_y = SIZE*4; m_location = 0; break;
      case 1: m_player.m_target_y = SIZE*9; m_location = 1; break;
      case 2: m_player.m_target_y = SIZE*14; m_location = 2; break;
    }
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 5) {
    setGameState(kAwaitInput);
    ++s_state;

  } else if (s_state == 6) {
    // Forbid moving two
    if ((getPlayerChoice() == 0 && m_location == 2) || (getPlayerChoice() == 2 && m_location == 0)) {
      --s_state;
      return false;
    } 

    m_player.m_target_x = SIZE*17;
    switch (getPlayerChoice()) {
      case 0: m_player.m_target_y = SIZE*5; break;
      case 1: m_player.m_target_y = SIZE*9; break;
      case 2: m_player.m_target_y = SIZE*13; break;
    }
    setGameState(kMovePlayer);
    ++s_state;

  } else if (s_state == 7) {
    setGameState(kFadeOut);
  }

  return false;
}
