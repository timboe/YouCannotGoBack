#include "dark.h"
#include "../sound.h"

static uint16_t s_state = 0;
static int8_t s_fire = -1;
static int8_t s_correct[3] = {0};

static int8_t s_id0[3] = {0};
static int8_t s_id1[3] = {0};
static int8_t s_id2[3] = {0};

static float s_angles[3] = {0};

#define L_1 4
#define L_2 1

void updateProcPattern(PlaydateAPI* _pd) {

  renderPit(_pd);
  renderStandingStoneFloor(_pd);

  if (s_state < 13) renderWalls(_pd, true, true, true, true);

  // Under: Hints
  if (s_state > 6) {
    renderPatternUnder(_pd, 11, 6,  s_id0[s_correct[0]], s_id1[0]);
    renderPatternUnder(_pd, 11, 10, s_id0[s_correct[0]], s_id1[1]);
    renderPatternUnder(_pd, 11, 14, s_id0[s_correct[0]], s_id1[2]);
  } 

  renderStandingStoneGrid(_pd, s_id0, s_id1, s_id2, s_correct, s_state, true);

  // Line: Hints
  if (s_state > 8) {
    renderPatternLine(_pd, 15, 6,  s_angles[0], kColorWhite, kColorBlack, L_1, L_2);
    renderPatternLine(_pd, 15, 10, s_angles[1], kColorWhite, kColorBlack, L_1, L_2);
    renderPatternLine(_pd, 15, 14, s_angles[2], kColorWhite, kColorBlack, L_1, L_2);
  }

  renderPlayer(_pd);

  renderStonesCommonFire(_pd, s_correct, s_fire, s_state);

  // Only put the wall on tope while we are exiting
  if (s_state == 13) renderWalls(_pd, true, true, true, true);

  // Answer box
  _pd->graphics->fillEllipse(3*SIZE + 0, 0, (SIZE*5) - 0, (SIZE*5) - 0, 0, 0, kColorBlack);
  _pd->graphics->fillEllipse(3*SIZE + 1, 1, (SIZE*5) - 2, (SIZE*5) - 2, 0, 0, kColorWhite);
  _pd->graphics->fillEllipse(3*SIZE + 2, 2, (SIZE*5) - 4, (SIZE*5) - 4, 0, 0, kColorBlack);

  // Answer
  renderPatternUnder(_pd, 5.5f, 2.5f, s_id0[s_correct[0]], s_id1[s_correct[1]]);
  renderStandingStone(_pd, 5.5f, 2.5f, kColorWhite, s_id0[s_correct[0]]);
  renderPatternLine(_pd, 5.5f, 2.5f, s_angles[s_correct[2]], kColorWhite, kColorBlack, L_1, L_2);

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
    m_player.m_position_x = SIZE*3;
    m_player.m_position_y = SIZE*9;
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
      s_angles[_angleCorrect] = 20.0f * (float)M_PI/180.0f * (rand()%9);
      const float _aMod = 60.0f / (float)(m_dungeon.m_difficulty + 1);
      for (int _i = 0; _i < 3; ++_i) {
        if (_i == _angleCorrect) continue;
        int8_t _diff = s_id1[_i] - s_id1[_angleCorrect];
        float _aDiff = ((float)M_PI/180.0f) * _aMod * _diff;
        s_angles[_i] = s_angles[_angleCorrect] + _aDiff; 
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
