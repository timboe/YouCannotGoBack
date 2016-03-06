#include "stones.h"

static uint16_t s_state = 0;
static int8_t s_fire = -1;
static int8_t s_correct[3] = {0};
static int8_t s_coloursA[3] = {0};
static int8_t s_coloursB[3] = {0};
static int8_t s_coloursC[3] = {0};

void updateProcStones(GContext* _ctx) {

  renderPit(_ctx);

  renderStandingStoneGrid(_ctx, s_coloursA, s_coloursB, s_coloursC);

  renderPlayer(_ctx);
  renderWalls(_ctx, true, true, true, true);
  //

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
    if (s_state == 9) _off = 4;
    else if (s_state == 11) _off = 8;
    drawBitmap(_ctx, m_arrow, 6 + _off, 3);
    drawBitmap(_ctx, m_arrow, 6 + _off, 7);
    drawBitmap(_ctx, m_arrow, 6 + _off, 11);
  }

}

bool tickStones(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position = GPoint(0, SIZE*9);
    s_fire = -1;
    s_correct[0] = randomiseChoices(s_coloursA, 0);
    s_correct[1] = randomiseChoices(s_coloursB, 1);
    s_correct[2] = randomiseChoices(s_coloursC, 2);
    return false;
  }

  if (s_state == 0) { // start initial move
    enterRoom(&s_state);
  } else if (s_state == 1) { // initial move is done
    setGameState(kAwaitInput);
    s_state = 5; // for consistency w dark.c
  } else {
    stonesCommon(&s_state, &s_fire, s_correct);
  }

  return false;
}
