#include "dark.h"
#include "../sound.h"

static uint16_t s_state = 0;
static int8_t s_fire = -1;
static int8_t s_correct[3] = {0};

static int8_t s_id0[3] = {0};
static int8_t s_id1[3] = {0};
static int8_t s_id2[3] = {0};

static float s_angles1[3] = {0};
static float s_angles2[3] = {0};

void updateProcPattern(PlaydateAPI* _pd) {

  //_pd->system->logToConsole("   !!! RS: %i", (int) s_state);

  renderStonesCommon(_pd, s_id0, s_id1, s_id2, s_correct, s_fire, s_state, true);

  // Line: Hints
  if (s_state > 6) {
    renderPatternLine(_pd, 11, 6, s_angles1[0], kColorWhite, kColorBlack, 3, 1);
    renderPatternLine(_pd, 11, 10, s_angles1[1], kColorWhite, kColorBlack, 3, 1);
    renderPatternLine(_pd, 11, 14, s_angles1[2], kColorWhite, kColorBlack, 3, 1);
  }
  //
  if (s_state > 8) {
    renderPatternLine(_pd, 15, 6, s_angles2[0], kColorBlack, kColorWhite, 5, 3);
    renderPatternLine(_pd, 15, 10, s_angles2[1], kColorBlack, kColorWhite, 5, 3);
    renderPatternLine(_pd, 15, 14, s_angles2[2], kColorBlack, kColorWhite, 5, 3);

    renderPatternLine(_pd, 15, 6, s_angles1[s_correct[1]], kColorWhite, kColorBlack, 3, 1);
    renderPatternLine(_pd, 15, 10, s_angles1[s_correct[1]], kColorWhite, kColorBlack, 3, 1);
    renderPatternLine(_pd, 15, 14, s_angles1[s_correct[1]], kColorWhite, kColorBlack, 3, 1);
  }
      //x:7,11,15
      //y:6,10,14

  renderPlayer(_pd);
  renderWalls(_pd, true, true, true, true);

  // Answer box
  _pd->graphics->fillEllipse(0, 0, (SIZE*6) - 0, (SIZE*6) - 0, 0, 0, kColorBlack);
  _pd->graphics->fillEllipse(2, 2, (SIZE*6) - 4, (SIZE*6) - 4, 0, 0, kColorWhite);
  _pd->graphics->fillEllipse(4, 4, (SIZE*6) - 8, (SIZE*6) - 8, 0, 0, kColorBlack);

  // Answer
  renderStandingStone(_pd, 3, 3, kColorWhite, s_id0[s_correct[0]]);
  renderPatternLine(_pd, 3, 3, s_angles2[s_correct[2]], kColorBlack, kColorWhite, 5, 3);
  renderPatternLine(_pd, 3, 3, s_angles1[s_correct[1]], kColorWhite, kColorBlack, 3, 1);


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
      const int8_t _angleCorrect = s_correct[1];
      s_angles1[_angleCorrect] = (float)M_PI/180.0f * (rand()%180);
      const float _aMod = 60.0f / (float)(m_dungeon.m_level + 1);
      for (int _i = 0; _i < 3; ++_i) {
        if (_i == _angleCorrect) continue;
        int8_t _diff = s_id1[_i] - s_id1[_angleCorrect];
        float _aDiff = ((float)M_PI/180.0f) * _aMod * _diff;
        s_angles1[_i] = s_angles1[_angleCorrect] + _aDiff; 
      }
    }

    {
      const int8_t _angleCorrect = s_correct[2];
      do {
        s_angles2[s_correct[2]] = (float)M_PI/180.0f * (rand()%180);
      } while ( (float)fabs(s_angles2[s_correct[2]] - s_angles1[s_correct[1]]) < (float)M_PI/180.0f * 20.0f);
      const float _aMod = 60.0f / (float)(m_dungeon.m_level + 1);
      for (int _i = 0; _i < 3; ++_i) {
        if (_i == _angleCorrect) continue;
        int8_t _diff = s_id2[_i] - s_id2[_angleCorrect];
        float _aDiff = ((float)M_PI/180.0f) * _aMod * _diff;
        s_angles2[_i] = s_angles2[_angleCorrect] + _aDiff; 
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
