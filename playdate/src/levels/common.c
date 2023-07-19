#include "common.h"
#include "../render.h"
#include "../sound.h"

void moveToExit(uint16_t* _state) {
  m_player.m_target_x = SIZE*17;
  switch (getPlayerChoice()) {
    case 0: m_player.m_target_y = SIZE*5; break;
    case 1: m_player.m_target_y = SIZE*9; break;
    case 2: m_player.m_target_y = SIZE*13; break;
  }
  setGameState(kMovePlayer);
  ++(*_state);
}

void addCluter(int _xMax, int _yUp, int _yDn) {
  // Min clutter, if we are leaving a hint then we need at least 1
  int _minClutter = 0;
  Hints_t _hint = m_dungeon.m_roomGiveHint[m_dungeon.m_level][m_dungeon.m_room];
  if (_hint == kGreekLetter || _hint == kNumber) {
    _minClutter = 1;
  }

  int _nClutter = _minClutter + ( rand() % ( MAX_PLACE_CLUTTER - _minClutter + 1 )  ); // 0-5 or 1-5

#define N_X 5
#define N_Y 4
  static const int8_t _xCoords[] = {3, 5, 8, 10, 13};
  static const int8_t _yCoords[] = {2, 6, 12, 16};

  for (int _c = 0; _c < _nClutter; ++_c) {
    int _abort = 0;
    while (++_abort < 50) {
      int _x = _xCoords[ rand() % N_X ];
      int _y = _yCoords[ rand() % N_Y ];
      if (_x > _xMax && _y > _yUp && _y < _yDn) continue; // Check location
      bool _cont = false;
      for (int _check = 0; _check < m_clutter.m_nClutter; ++_check) { // Check does not exist
        if (m_clutter.m_position_x[_check] == _x && m_clutter.m_position_y[_check] == _y) _cont = true;
      }
      if (_cont == true) continue;
      //APP_LOG(APP_LOG_LEVEL_INFO,"   ~ Add c %i/%i at %i %i", m_clutter.m_nClutter, _nClutter, _x, _y);
      m_clutter.m_position_x[ m_clutter.m_nClutter ] = _x;
      m_clutter.m_position_y[ m_clutter.m_nClutter ] = _y;
      ++m_clutter.m_nClutter;
      break;
    }
  }
}

void enterRoom(uint16_t* _state) {
  m_player.m_target_x = SIZE*3;
  m_player.m_target_y = SIZE*9;
  setGameState(kMovePlayer);
  ++(*_state);
}

void shuffler(PlaydateAPI* _pd, int8_t* _choices, int _offset, int _rand) {
  #ifdef DEV
  _pd->system->logToConsole("Shuffler %i %i %i", _choices[0], _choices[1], _choices[2]);
  #endif
  for (int _c = 0; _c < 3; ++_c) {
    int _c1 = _c + 1, _c2 = _c + 2;
    if (_c1 >= 3) _c1 -= 3;
    if (_c2 >= 3) _c2 -= 3;
    while (_choices[_c] == -1 || _choices[_c] == _choices[_c1] || _choices[_c] == _choices[_c2]) {
      _choices[_c] = _offset + (rand() % _rand);
    }
  }
}

uint16_t randomiseChoices(PlaydateAPI* _pd, int8_t* _choices, int _stage) {
  // Have an array size 3, first choose correct location

  for (int _i = 0; _i < 3; ++_i) _choices[ _i ] = -1;
  uint16_t _c = rand() % 3;

  Hints_t _type = m_dungeon.m_roomNeedHint[ m_dungeon.m_level ][ m_dungeon.m_room ];
  int8_t _value = m_dungeon.m_roomNeedHintValue[ m_dungeon.m_level ][ m_dungeon.m_room ];

  if ( _type == kShield ) {
      if      (_stage == 0) _choices[_c] = getShieldA( _value );
      else if (_stage == 1) _choices[_c] = getShieldB( _value );
      else                  _choices[_c] = getShieldC( _value );
  } else {
    _choices[_c] = _value;
  }

  #ifdef DEV
  _pd->system->logToConsole("   [RC] T:%i V:%i Rnd:%i CHOICE:%i MAX:%i", _type, _value, _c, _choices[_c], getHintValueMax( _type ) );
  #endif
  shuffler(_pd, _choices, 0, getHintValueMax( _type ));
  return _c;

}

void stonesCommon(uint16_t* _state, int8_t* _fire, int8_t* _correct) {
  int _targetX, _step;
  switch (*_state) {
    case 5: case 6: _targetX = 6; _step = 0; break;
    case 7: case 8: _targetX = 10; _step = 1; break;
    case 9: case 10: _targetX = 14; _step = 2; break;
    case 11: default: _targetX = 17; _step = 2; break;
  }

  if ((*_state) == 13) {
    setGameState(kFadeOut);
  } else if ((*_state) % 2 == 1) { // ODD state
    m_player.m_target_x = SIZE*_targetX;
    switch (getPlayerChoice()) {
      case 0: m_player.m_target_y = SIZE*5; break;
      case 1: m_player.m_target_y = SIZE*9; break;
      case 2: m_player.m_target_y = SIZE*13; break;
    }
    setGameState(kMovePlayer);
    ++(*_state);

  } else { // EVEN

    if (getPlayerChoice() != _correct[_step]) {
      m_dungeon.m_gameOver = 1;
      setGameState(kFadeOut);
      //vibes_long_pulse();
    } else if ((*_state) < 10) {
      setGameState(kAwaitInput);
    }
    if (*_state <= 10) fireSound();
    ++(*_state); // On 6, draw first fires
    ++(*_fire);
    
  }
}

void renderStonesCommon(PlaydateAPI* _pd, int8_t* _coloursA, int8_t* _coloursB, int8_t* _coloursC, int8_t* _correct, int8_t _fire, int16_t _state) {
  renderPit(_pd);
  renderStandingStoneGrid(_pd, _coloursA, _coloursB, _coloursC);
  renderPlayer(_pd);
  renderWalls(_pd, true, true, true, true);

  for (int _f = 0; _f < 3; ++_f) {  // Draw fires
    if (_fire < _f) break;
    for (int _i = 0; _i < 3; ++_i) {
      if (_correct[_f] == _i) continue;
      drawCBitmap(_pd, &m_fire[0], 5 + (4 * _f) , 5 + (4 * _i));
      drawCBitmap(_pd, &m_fire[1], 6 + (4 * _f) , 4 + (4 * _i));
    }
  }

  int _off = 0;
  if (_state == 7) _off = 4;
  else if (_state == 9) _off = 8;
  renderArrows(_pd, 6 + _off, 5, 4);
}
