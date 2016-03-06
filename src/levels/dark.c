#include "dark.h"

static uint16_t s_state = 0;
static int8_t s_fire = -1;
static int8_t s_correct[3] = {0};
static int8_t s_colours[3] = {32,32,32}; // Defaults to gray
static int8_t s_ringSize = 0;

void updateProcDark(GContext* _ctx) {

  //APP_LOG(APP_LOG_LEVEL_INFO,"   !!! RS: %i", (int) s_state);

  if (s_state < 4) { // Render hint
    graphics_context_set_stroke_color(_ctx, GColorWhite);
    graphics_context_set_stroke_width(_ctx, 3);
    GPoint _p = GPoint((7 + 4*s_state)*SIZE, (6 + 4*s_correct[s_state])*SIZE);
#ifdef PBL_ROUND
    _p.x += ROUND_OFFSET_X;
    _p.y += ROUND_OFFSET_Y;
#endif
    graphics_draw_circle(_ctx, _p, s_ringSize);
    return;
  }

  renderPit(_ctx);

  renderStandingStoneGrid(_ctx, s_colours, s_colours, s_colours);

  renderPlayer(_ctx);
  renderWalls(_ctx, true, true, true, true);

  for (int _f = 0; _f < 3; ++_f) {  // Draw fires
    if (s_fire < _f) break;
    for (int _i = 0; _i < 3; ++_i) {
      if (s_correct[_f] == _i) continue;
      drawBitmap(_ctx, m_fire[0], 5 + (4 * _f) , 5 + (4 * _i));
      drawBitmap(_ctx, m_fire[1], 6 + (4 * _f) , 4 + (4 * _i));
    }
  }

  if (getGameState() == kAwaitInput && getFrameCount() < ANIM_FPS/2) {
    int _off = 0;
    if (s_state == 7) _off = 4;
    else if (s_state == 9) _off = 8;
    drawBitmap(_ctx, m_arrow, 6 + _off, 3);
    drawBitmap(_ctx, m_arrow, 6 + _off, 7);
    drawBitmap(_ctx, m_arrow, 6 + _off, 11);
  }

}

bool tickDark(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position = GPoint(SIZE*3, SIZE*9);
    s_fire = -1;
    s_ringSize = 0;
    for (int _i = 0; _i < 3; ++_i) {
      s_correct[_i] = rand() % 3;
    }
    setGameState(kLevelSpecific); // Overwrite "fade in"
    return false;
  }

  if (s_state < 3) { // start initial move
    s_ringSize += 1 + m_dungeon.m_level;
    if (s_ringSize > 20) {
      ++s_state;
      s_ringSize = 0;
    }
    return true; // re-render
  } else if (s_state == 3) {
    setGameState(kFadeIn);
    ++s_state;
  } else if (s_state == 4) {
    setGameState(kAwaitInput);
    ++s_state;
  } else {
    stonesCommon(&s_state, &s_fire, s_correct);
  }

  return false;
}
