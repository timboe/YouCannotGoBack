#include "dark.h"

static uint16_t s_state = 0;
static int8_t s_fire = -1;
static int8_t s_correct[3] = {0};
static int8_t s_colours[3] = {32,32,32}; // Defaults to gray
static int8_t s_ringSize = 0;

void updateProcDark(PlaydateAPI* _pd) {

  //_pd->system->logToConsole("   !!! RS: %i", (int) s_state);

  if (s_state < 4) { // Render hint
    // TODO
    //graphics_context_set_stroke_color(_pd, GColorWhite);
    //graphics_context_set_stroke_width(_pd, 3);
    //GPoint _p = GPoint((7 + 4*s_state)*SIZE, (6 + 4*s_correct[s_state])*SIZE);
    //graphics_draw_circle(_pd, _p, s_ringSize);
    return;
  }

  renderStonesCommon(_pd, s_colours, s_colours, s_colours, s_correct, s_fire, s_state);
}

bool tickDark(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position_x = SIZE*3;
    m_player.m_position_y = SIZE*9;
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
