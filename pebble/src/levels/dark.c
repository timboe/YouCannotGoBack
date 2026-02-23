#include "dark.h"

static uint16_t s_state = 0;
static int8_t s_fire = -1;
static int8_t s_correct[3] = {0};
static int8_t s_colours[3] = {32,32,32}; // Defaults to gray
static int8_t s_ringSize = 0;

void updateProcDark(GContext* _ctx) {

  //APP_LOG(APP_LOG_LEVEL_INFO,"   !!! RS: %i", (int) s_state);

  if (s_state < 4) { // Render hint // TODO - move to render file
    graphics_context_set_stroke_color(_ctx, GColorWhite);
    graphics_context_set_stroke_width(_ctx, 3);
    GPoint _p = GPoint((7 + 4*s_state)*SIZE, (6 + 4*s_correct[s_state])*SIZE);
    _p.x += GLOBAL_OFFSET_X;
    _p.y += GLOBAL_OFFSET_Y;
    graphics_draw_circle(_ctx, _p, s_ringSize);
    return;
  }

  renderPit(_ctx);
  renderStandingStoneFloor(_ctx);
  renderStandingStoneGrid(_ctx, s_colours, s_colours, s_colours, s_correct, s_state, false); // s_correct & s_state not used
  renderPlayer(_ctx);
  renderStonesCommonFire(_ctx, s_correct, s_fire, s_state);
  renderWalls(_ctx, true, true, true, true);
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
    s_ringSize += 1 + MIN(4, m_dungeon.m_difficulty);
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
