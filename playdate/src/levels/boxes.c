#include "stones.h"

static uint16_t s_state = 0;
static int8_t s_correct = 0;
static int8_t s_coloursA[3] = {0}; // top row
static int8_t s_coloursB[3] = {0}; // middle row
static int8_t s_coloursC[3] = {0}; // bottom row
static int8_t s_offset[9] = {0};

void updateProcBoxes(PlaydateAPI* _pd) {
  renderFloor(_pd, 0);
  renderBoxGrid(_pd, s_coloursA, s_coloursB, s_coloursC, s_offset);
  renderPlayer(_pd);
  renderWalls(_pd, true, true, true, true);
  renderArrows(_pd, 15, 5, 4);
}

bool tickBoxes(PlaydateAPI* _pd, bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position_x = 0;
    m_player.m_position_y = SIZE*9;
    s_correct = rand() % 3;

    const int8_t _answer = m_dungeon.m_roomNeedHintValue[ m_dungeon.m_level ][ m_dungeon.m_room ];
    int8_t _wrong1 = _answer;
    int8_t _wrong2 = _answer;
    while (_wrong1 == _answer) _wrong1 = rand() % kNShieldTypes;
    while (_wrong2 == _answer || _wrong2 == _wrong1) _wrong2 = rand() % kNShieldTypes;

    int8_t _a, _b, _c;
    switch (s_correct) {
      case 0: _a = _answer; _b = _wrong1; _c = _wrong2; break;
      case 1: _a = _wrong1; _b = _answer; _c = _wrong2; break;
      case 2: _a = _wrong1; _b = _wrong2; _c = _answer; break;
    }

    s_coloursA[0] = getShieldA(_a); s_coloursA[1] = getShieldA(_b); s_coloursA[2] = getShieldA(_c);
    s_coloursB[0] = getShieldB(_a); s_coloursB[1] = getShieldB(_b); s_coloursB[2] = getShieldB(_c);
    s_coloursC[0] = getShieldC(_a); s_coloursC[1] = getShieldC(_b), s_coloursC[2] = getShieldC(_c);

    for (int _i = 0; _i < 9; ++_i) s_offset[_i] = -4 + (rand() % 9);

    #ifdef DEV
    _pd->system->logToConsole("SOLUTION:%i", s_correct);
    #endif

    return false;
  }

  if (s_state == 0) { // start initial move
    enterRoom(&s_state);
  } else if (s_state == 1) { // initial move is done
    setGameState(kAwaitInput);
    ++s_state;
  } else if (s_state == 2) {
    if (getPlayerChoice() != s_correct) { //Wrong solution!
      if (m_dungeon.m_lives > 0) --m_dungeon.m_lives;
      else m_dungeon.m_rooms[ m_dungeon.m_level ][ m_dungeon.m_room + 1 ] = kDeath;
      #ifdef DEV
      _pd->system->logToConsole("WRONG");
      #endif
    }
    moveToExit(&s_state);
  } else if (s_state == 3) {
    setGameState(kFadeOut);
  }


  return false;
}
