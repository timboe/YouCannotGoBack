#include "bridge.h"

static uint16_t s_state = 0;
static int8_t s_choices[3] = {0};
static uint16_t s_correct = 0;
static uint16_t s_breakPoint = 0;

void updateProcBridge(GContext* _ctx) {

  renderFloor(_ctx, 1);

  for (int _i = 0; _i < 3; ++_i) {
    drawBitmap(_ctx, m_bridge[0], 4,  5 + (_i * 4));
    drawBitmap(_ctx, m_bridge[2], 13, 5 + (_i * 4));

    for (int _x = 5; _x < 13; ++_x) {
      if (s_state == 6 && _i != s_correct && _x > 6 && _x < s_breakPoint) continue;
      drawBitmap(_ctx, m_bridge[1], _x, 5 + (_i * 4));
    }
  }

  renderPlayer(_ctx);
  renderWalls(_ctx, true, true, true, true);

  for (int _s = 0; _s < 3; ++_s) {
    Hints_t _hint = m_dungeon.m_roomNeedHint[ m_dungeon.m_level ][ m_dungeon.m_room];
    if ( _hint == kNumber) {
      GRect _b = GRect(16*SIZE - 2, (5 + (_s*4))*SIZE - 2, 2*SIZE + 4, 2*SIZE + 4);
      renderFrame(_ctx, _b);
      renderHintNumber(_ctx, GRect(16*SIZE - 1, (5 + (_s*4))*SIZE - 1, 16, 16), s_choices[_s], true);
    } else if ( _hint == kSymbol ) {
      drawBitmap(_ctx, m_symbol[ s_choices[_s] ], 16, 5 + (_s*4));
    } else if ( _hint == kGreek ) {
      GRect _b = GRect(16*SIZE - 2, (5 + (_s*4))*SIZE - 2, 2*SIZE + 4, 2*SIZE + 4);
      renderFrame(_ctx, _b);
      drawBitmapAbs(_ctx, m_greek[ s_choices[_s] ], GPoint(16*SIZE + 4, (5 + (_s*4))*SIZE + 4));
    }
  }

  renderClutter(_ctx);

  if (getGameState() == kAwaitInput && getFrameCount() < ANIM_FPS/2) {
    drawBitmap(_ctx, m_arrow, 8, 4);
    drawBitmap(_ctx, m_arrow, 8, 8);
    drawBitmap(_ctx, m_arrow, 8, 12);
  }

}

bool tickBridge(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position = GPoint(0, SIZE*9);
    addCluter(4, 0, 20);
    s_correct = randomiseChoices(s_choices, 0);
    return false;
  }

  if (s_state == 0) { // start initial move
    enterRoom(&s_state);
  } else if (s_state == 1) { // initial move is done
     setGameState(kAwaitInput);
     ++s_state;
  } else if (s_state == 2) { // Move to chosen bridge
    switch (getPlayerChoice()) {
      case 0: m_player.m_target = GPoint(SIZE*3, SIZE*5); break;
      case 1: m_player.m_target = GPoint(SIZE*3, SIZE*9); break;
      case 2: m_player.m_target = GPoint(SIZE*3, SIZE*13); break;
    }
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 3) { // move to center of bridge
    m_player.m_target.x = SIZE*8;
    s_breakPoint = 11;
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
   vibes_long_pulse();
   s_state = 6;
 } else if (s_state == 5) {
   m_player.m_target.x = SIZE*16;
   setGameState(kMovePlayer);
   s_state = 6;
 } else if (s_state == 6) {
   setGameState(kFadeOut);
 }

  return false;
}
