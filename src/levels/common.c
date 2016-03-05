#include "common.h"


void addCluter(int _max, int _x, int _yUp, int _yDn) {
  // Min clutter, if we are leaving a hint then we need at least 1
  int _minClutter = 0;
  if (m_dungeon.m_roomGiveHint[m_dungeon.m_level][m_dungeon.m_room] == kNumber
   || m_dungeon.m_roomGiveHint[m_dungeon.m_level][m_dungeon.m_room] == kGreek ) {
    _minClutter = 1;
  }

  int _nClutter = _minClutter + ( rand() % ( _max - _minClutter + 1 )  ); // 0-5 or 1-5

  for (int _c = 0; _c < _nClutter; ++_c) {
    int _abort = 0;
    while (++_abort < 50) {
      GPoint _p = GPoint(3 + rand() % 10, 2 + rand() % 14);
      if (_p.y > 6 && _p.y < 11) continue; // Avoid player entry area
      if (_p.x > _x && _p.y > _yUp && _p.y < _yDn) {
        APP_LOG(APP_LOG_LEVEL_INFO,"   ~ VETO c %i/%i at %i %i (%i %i %i)", _c+1, _nClutter, _p.x, _p.y, _x, _yUp, _yDn);
        continue; // Check location
      }
      for (int _check = 0; _check < m_clutter.m_nClutter; ++_check) { // Check does not exist
        if (m_clutter.m_position[_check].x == _p.x && m_clutter.m_position[_check].y == _p.y) continue;
      }
      APP_LOG(APP_LOG_LEVEL_INFO,"   ~ Add c %i/%i at %i %i", _c+1, _nClutter, _p.x, _p.y);
      m_clutter.m_position[_c] = _p;
      m_clutter.m_nClutter = _c+1;
      break;
    }
  }
}

void enterRoom(uint16_t* _state) {
  m_player.m_target = GPoint(SIZE*3, SIZE*9);
  setGameState(kMovePlayer);
  ++(*_state);
}

uint16_t randomiseChoices(int8_t* _choices, int _stage) {
  APP_LOG(APP_LOG_LEVEL_INFO,"   [In RandChoice] stage:%i", _stage);
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

  int _max = getHintValueMax( _type );

  APP_LOG(APP_LOG_LEVEL_INFO,"   [RC] T:%i V:%i Rnd:%i CHOICE:%i MAX:%i", _type, _value, _c, _choices[_c], _max );


  for (int _v = 0; _v < 3; ++_v) {
    int _v1 = _v + 1;
    int _v2 = _v + 2;
    if (_v1 >= 3) _v1 -= 3;
    if (_v2 >= 3) _v2 -= 3;
    while (_choices[_v] == -1 || _choices[_v] == _choices[_v1] || _choices[_v] == _choices[_v2]) {
      _choices[_v] = rand() % _max;
    }
  }

  return _c;

}
