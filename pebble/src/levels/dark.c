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

  renderStonesCommon(_ctx, s_colours, s_colours, s_colours, s_correct, s_fire, s_state);
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
