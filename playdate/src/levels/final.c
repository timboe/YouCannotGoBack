#include "final.h"
#include "../sound.h"

static uint16_t s_state = 0;
static uint16_t s_fire_x[2];
static uint16_t s_fire_y[2];

void updateProcFinal(PlaydateAPI* _pd) {

  renderFloor(_pd, 0);

  for (int _i = 0; _i < 3; ++_i) {
    drawCBitmap(_pd, &m_flagstone[_i], 5, 5 + (_i * 4));
  }

  renderFinalPit(_pd);

  if (s_state > 5) {
    const int8_t _level = m_dungeon.m_level;
    drawCBitmap(_pd, &m_innerCorner[3], 7, 6);
    drawCBitmap(_pd, &m_innerCorner[0], 7, 12);
    drawCBitmap(_pd, &m_innerWall[3], 9, 6);
    drawCBitmap(_pd, &m_innerWall[2], 9, 12);
    drawCBitmap(_pd, getFloor(true, _level), 7, 8);
    drawCBitmap(_pd, getFloor(true, _level), 7, 10);
    drawCBitmap(_pd, getFloor(true, _level), 9, 8);
    drawCBitmap(_pd, getFloor(true, _level), 9, 10);
  } else {
    for (int _i = 0; _i < 4; ++_i) rand(); // keep the wall the same
  }

  drawCBitmap(_pd, &m_treasure[0], 11, 7);
  drawCBitmap(_pd, &m_treasure[2], 14, 8);
  drawCBitmap(_pd, &m_chest[1], 12, 9);

  renderPlayer(_pd);
  renderWalls(_pd, true, false, true, false);

  drawCBitmap(_pd, &m_treasure[2], 11, 11);
  drawCBitmap(_pd, &m_treasure[1], 13, 10);

  drawCBitmapAbs(_pd, &m_fire[0], s_fire_x[0], s_fire_y[0]);
  drawCBitmapAbs(_pd, &m_fire[1], s_fire_x[0] + SIZE, s_fire_y[0] - SIZE);
  drawCBitmapAbs(_pd, &m_fire[0], s_fire_x[1], s_fire_y[1]);
  drawCBitmapAbs(_pd, &m_fire[1], s_fire_x[1] + SIZE, s_fire_y[1] - SIZE);

  renderArrows(_pd, 5, 5, 4);
}

bool tickFinal(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position_x = 0;
    m_player.m_position_y = SIZE*9;
    s_fire_x[0] = 1*SIZE;
    s_fire_y[0] = 2*SIZE;
    s_fire_x[1] = 1*SIZE;
    s_fire_y[1] = 16*SIZE;
    return false;
  }

  static const char _msgA[] = "*CLICK CLICK*";
  static const char _msgB[] = "TREASURE!";

  if (s_state == 0) { // start initial move
    enterRoom(&s_state);
  } else if (s_state == 1) { // initial move is done
    setGameState(kAwaitInput);
    ++s_state;
  } else if (s_state == 2) {
    m_player.m_target_x = SIZE*5;
    switch (getPlayerChoice()) {
      case 0: m_player.m_target_y = SIZE*5; break;
      case 1: m_player.m_target_y = SIZE*9; break;
      case 2: m_player.m_target_y = SIZE*13; break;
    }
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 3) {
    setDisplayMsg(_msgA);
    setGameState(kDisplayMsg);
    chestSound();
    ++s_state;
  } else if (s_state == 4) {
    if (getPlayerChoice() != m_dungeon.m_finalPuzzle) s_state = 5; // Doom
    else s_state = 6; // win
  } else if (s_state == 5) {
    // LOOSE move fires
    for (int _f = 0; _f < 2; ++_f) {
      bool _move = false;
      if (s_fire_x[_f] < m_player.m_position_x - SIZE) {
        ++s_fire_x[_f];
        _move = true;
      }
      if (s_fire_y[_f] < m_player.m_position_y) {
        ++s_fire_y[_f];
        _move = true;
      } else if (s_fire_y[_f] > m_player.m_position_y) {
        --s_fire_y[_f];
        _move = true;
      }
      if (_move == false) {
        m_dungeon.m_gameOver = 1;
        setGameState(kFadeOut);
        fireSound();
      }
    }
    return true; //redraw
  } else if (s_state == 6) {
    m_player.m_target_x = SIZE*12;
    m_player.m_target_y = SIZE*9;
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 7) {
    setDisplayMsg(_msgB);
    setGameState(kDisplayMsg);
    bufSound();
    ++s_state;
  } else if (s_state == 8) {
    m_player.m_target_x = SIZE*15;
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 9) {
    m_dungeon.m_gameOver = 2; // Woo hoo!
    setGameState(kFadeOut);
  }

  return false;
}
