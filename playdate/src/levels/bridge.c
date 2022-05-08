#include "bridge.h"
#include "../sound.h"

static uint16_t s_state = 0;
static int8_t s_choices[3] = {0};
static uint16_t s_correct = 0;
static uint16_t s_breakPoint = 0;

void updateProcBridge(PlaydateAPI* _pd) {

  renderFloor(_pd, 1);

  for (int _i = 0; _i < 3; ++_i) {
    drawCBitmap(_pd, &m_bridge[0], 4,  5 + (_i * 4));
    drawCBitmap(_pd, &m_bridge[2], 13, 5 + (_i * 4));

    for (int _x = 5; _x < 13; ++_x) {
      if (s_state == 6 && _i != s_correct && _x > 6 && _x < s_breakPoint) continue;
      drawCBitmap(_pd, &m_bridge[1], _x, 5 + (_i * 4));
    }
  }

  renderClutter(_pd);
  renderPlayer(_pd);
  renderWalls(_pd, true, true, true, true);
  renderWallClutter(_pd);

  for (int _s = 0; _s < 3; ++_s) {
    Hints_t _hint = m_dungeon.m_roomNeedHint[ m_dungeon.m_level ][ m_dungeon.m_room];
    if ( _hint == kNumber) {
      PDRect _b = {.x = 16*SIZE - 2, .y = (5 + (_s*4))*SIZE - 2, .width = 2*SIZE + 4, .height = 2*SIZE + 4};
      renderFrame(_pd, _b);
      PDRect _bHint = {.x = 16*SIZE - 1, .y = (5 + (_s*4))*SIZE - 1, .width = 16, .height = 16};
      renderHintNumber(_pd, _bHint, s_choices[_s], true);
    } else if ( _hint == kSymbol ) {
      drawCBitmap(_pd, &m_symbol[ s_choices[_s] ], 16, 5 + (_s*4));
    } else if ( _hint == kGreek ) {
      PDRect _b = {.x = 16*SIZE - 2, .y = (5 + (_s*4))*SIZE - 2, .width = 2*SIZE + 4, .height = 2*SIZE + 4};
      renderFrame(_pd, _b);
      drawCBitmapAbs(_pd, &m_greek[ s_choices[_s] ], 16*SIZE + 4, (5 + (_s*4))*SIZE + 4);
    }
  }

  renderArrows(_pd, 8, 5, 4);
}

bool tickBridge(PlaydateAPI* _pd, bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position_x = 0;
    m_player.m_position_y = SIZE*9;
    addCluter(4, 0, 20);
    s_correct = randomiseChoices(_pd, s_choices, 0);
    return false;
  }

  if (s_state == 0) { // start initial move
    enterRoom(&s_state);
  } else if (s_state == 1) { // initial move is done
     setGameState(kAwaitInput);
     ++s_state;
  } else if (s_state == 2) { // Move to chosen bridge
    m_player.m_target_x = SIZE*3;
    switch (getPlayerChoice()) {
      case 0: m_player.m_target_y = SIZE*5; break;
      case 1: m_player.m_target_y = SIZE*9; break;
      case 2: m_player.m_target_y = SIZE*13; break;
    }
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 3) { // move to center of bridge
    m_player.m_target_x = SIZE*8;
    s_breakPoint = 11;
    hitSound();
    if (getPlayerChoice() == s_correct) s_state = 5;
    else { // Wrong choice!
      if (m_dungeon.m_lives > 0) {
        --m_dungeon.m_lives;
        s_state = 5;
        s_breakPoint = 9; // Saved by early bridge break (but user sees they were wrong)
      } else {
        s_state = 4; // doom
      }
    }
    setGameState(kMovePlayer);
  } else if (s_state == 4) { // LOOSE
   m_dungeon.m_gameOver = 1;
   m_dungeon.m_fallingDeath = true;
   fallSound();
   s_state = 6;
 } else if (s_state == 5) {
   moveToExit(&s_state);
 } else if (s_state == 6) {
   setGameState(kFadeOut);
 }

  return false;
}
