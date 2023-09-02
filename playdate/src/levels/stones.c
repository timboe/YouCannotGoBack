#include "stones.h"

static uint16_t s_state = 0;
static int8_t s_fire = -1;
static int8_t s_correct[3] = {0};
static int8_t s_coloursA[3] = {0};
static int8_t s_coloursB[3] = {0};
static int8_t s_coloursC[3] = {0};

void updateProcStones(PlaydateAPI* _pd) {
  renderPit(_pd);
  renderStandingStoneFloor(_pd);
  renderStandingStoneGrid(_pd, s_coloursA, s_coloursB, s_coloursC, s_correct, s_state, false);
  renderPlayer(_pd);
  renderStonesCommonFire(_pd, s_correct, s_fire, s_state);
  renderWalls(_pd, true, true, true, true);
}

bool tickStones(PlaydateAPI* _pd, bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position_x = 0;
    m_player.m_position_y = SIZE*9;
    s_fire = -1;
    s_correct[0] = randomiseChoices(_pd, s_coloursA, 0);
    s_correct[1] = randomiseChoices(_pd, s_coloursB, 1);
    s_correct[2] = randomiseChoices(_pd, s_coloursC, 2);
    return false;
  }

  if (s_state == 0) { // start initial move
    enterRoom(&s_state);
  } else if (s_state == 1) { // initial move is done
    setGameState(kAwaitInput);
    s_state = 5; // for consistency w dark.c
  } else {
    stonesCommon(&s_state, &s_fire, s_correct, /*dark*/ false);
  }

  return false;
}
