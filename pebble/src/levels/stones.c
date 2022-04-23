#include "stones.h"

static uint16_t s_state = 0;
static int8_t s_fire = -1;
static int8_t s_correct[3] = {0};
static int8_t s_coloursA[3] = {0};
static int8_t s_coloursB[3] = {0};
static int8_t s_coloursC[3] = {0};

void updateProcStones(GContext* _ctx) {
  renderStonesCommon(_ctx, s_coloursA, s_coloursB, s_coloursC, s_correct, s_fire, s_state);
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
