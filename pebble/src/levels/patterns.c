#include "patterns.h"
#ifdef YCGBv2

static uint16_t s_state = 0;
static int8_t s_fire = -1;
static int8_t s_correct[3] = {0};

static int8_t s_id0[3] = {0};
static int8_t s_id1[3] = {0};
static int8_t s_id2[3] = {0};

static uint16_t s_angles[3] = {0};

#define L_1 4
#define L_2 1

void updateProcPattern(GContext* _ctx) {

  renderPit(_ctx);
  renderStandingStoneFloor(_ctx);

  if (s_state < 12) renderWalls(_ctx, true, true, true, true);

  // Under: Hints
  if (s_state > 6) {
    renderPatternUnder(_ctx, GPoint(11*SIZE, 6*SIZE),  s_id0[s_correct[0]], s_id1[0]);
    renderPatternUnder(_ctx, GPoint(11*SIZE, 10*SIZE), s_id0[s_correct[0]], s_id1[1]);
    renderPatternUnder(_ctx, GPoint(11*SIZE, 14*SIZE), s_id0[s_correct[0]], s_id1[2]);
  } 

  renderStandingStoneGrid(_ctx, s_id0, s_id1, s_id2, s_correct, s_state, true);

  // Line: Hints
  if (s_state > 8) {
    renderPatternLine(_ctx, GPoint(15*SIZE, 6*SIZE),  s_angles[0], GColorWhite, GColorBlack, L_1, L_2);
    renderPatternLine(_ctx, GPoint(15*SIZE, 10*SIZE), s_angles[1], GColorWhite, GColorBlack, L_1, L_2);
    renderPatternLine(_ctx, GPoint(15*SIZE, 14*SIZE), s_angles[2], GColorWhite, GColorBlack, L_1, L_2);
  }

  renderPlayer(_ctx);

  renderStonesCommonFire(_ctx, s_correct, s_fire, s_state);

  // Only put the wall on tope while we are exiting
  if (s_state >= 12) renderWalls(_ctx, true, true, true, true);

  // Answer box
  #ifdef PBL_ROUND
    const GPoint _aPoint = GPoint(4*SIZE, 3*SIZE);
  #else
    const GPoint _aPoint = GPoint(3*SIZE + SIZE/2, 2*SIZE + SIZE/2);
  #endif

  renderPatternAnswerBox(_ctx, _aPoint);
  // Answer
  renderPatternUnder(_ctx, _aPoint, s_id0[s_correct[0]], s_id1[s_correct[1]]);
  renderStandingStone(_ctx, _aPoint, GColorLightGray, s_id0[s_correct[0]]);
  renderPatternLine(_ctx, _aPoint, s_angles[s_correct[2]], GColorWhite, GColorBlack, L_1, L_2);

}

void randAssign(int8_t* _id) {
  _id[0] = rand() % 3;
  int8_t _r = rand() % 3;
  while (_r == _id[0]) _r = rand() % 3;
  _id[1] = _r;
  while (_r == _id[0] || _r == _id[1]) _r = rand() % 3;
  _id[2] = _r;
}

bool tickPattern(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position = GPoint(SIZE*3, SIZE*9);
    s_fire = -1;
    for (int _i = 0; _i < 3; ++_i) {
      s_correct[_i] = rand() % 3;
    }
    randAssign(s_id0);
    randAssign(s_id1);
    randAssign(s_id2);

    // Choose angles
    {
      const int8_t _angleCorrect = s_correct[2];
      const uint16_t _a = 20 * (rand() % 9);
      s_angles[_angleCorrect] = DEG_TO_TRIGANGLE(_a);
      const uint16_t _aMod = 60 / (m_dungeon.m_difficulty + 1);
      for (int _i = 0; _i < 3; ++_i) {
        if (_i == _angleCorrect) continue;
        const int8_t _diff = s_id1[_i] - s_id1[_angleCorrect];
        const int16_t _aDiff = _aMod * _diff;
        s_angles[_i] = DEG_TO_TRIGANGLE(_a + _aDiff); 
      }
    }

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

#endif