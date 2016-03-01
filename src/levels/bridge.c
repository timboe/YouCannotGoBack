#include "bridge.h"

static uint16_t s_state = 0;
static int8_t s_choices[3] = {0};
static uint16_t s_correct = 0;

void updateProcBridge(GContext* _ctx) {

  renderFloor(_ctx, 1);

  int _breakB = 11;
  for (int _i = 0; _i < 3; ++_i) {
    drawBitmap(_ctx, m_bridge[0], 4,  5 + (_i * 4));
    drawBitmap(_ctx, m_bridge[2], 13, 5 + (_i * 4));

    for (int _x = 5; _x < 13; ++_x) {
      if (s_state == 6 && _i != s_correct && _x > 7 && _x < _breakB) continue;
      drawBitmap(_ctx, m_bridge[1], _x, 5 + (_i * 4));
    }
  }

  renderPlayer(_ctx);
  renderWalls(_ctx, true, true, true, true);

  for (int _s = 0; _s < 3; ++_s) {
    GRect _b = GRect(16*SIZE - 2, (5 + (_s*4))*SIZE - 2, 2*SIZE + 4, 2*SIZE + 4);
    graphics_context_set_fill_color(_ctx, GColorDarkGray);
    graphics_context_set_stroke_color(_ctx, GColorWhite);
    graphics_context_set_stroke_width(_ctx, 2);
    graphics_fill_rect(_ctx, _b, 0, 0);
    graphics_draw_rect(_ctx, GRect(_b.origin.x+2, _b.origin.y+2, _b.size.w-4, _b.size.h-4));
    graphics_context_set_text_color(_ctx, GColorBlack);
    if ( m_dungeon.m_roomNeedHint[ m_dungeon.m_level ][ m_dungeon.m_room] == kNumber ) {
      renderHintNumber(_ctx, GRect(16*SIZE - 1, (5 + (_s*4))*SIZE - 1, 16, 16), s_choices[_s]);
    } else {
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
    addCluter(2, 5, 0, 20);
    s_correct = randomiseChoices(s_choices);
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
    if (getPlayerChoice() == s_correct) s_state = 5;
    else s_state = 4;
    setGameState(kMovePlayer);
  } else if (s_state == 4) { // LOOSE
   m_dungeon.m_gameOver = true;
   m_dungeon.m_fallingDeath = true;
   s_state = 6;
 } else if (s_state == 5) {
   m_player.m_target.x = SIZE*17;
   setGameState(kMovePlayer);
   s_state = 6;
 } else if (s_state == 6) {
   setGameState(kFadeOut);
 }

  return false;
}
