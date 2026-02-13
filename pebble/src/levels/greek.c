#include "greek.h"
#ifdef YCGBv2

static uint16_t s_state = 0;
static uint16_t s_correct = 0;
static int8_t s_choices[3] = {0};
static uint8_t s_lettersThisLevel = 0;

static uint8_t s_strA[TOTAL_LETTERS] = {0};
static uint8_t s_strB[TOTAL_LETTERS] = {0};
static uint8_t s_strC[TOTAL_LETTERS] = {0};

void updateProcGreek(GContext* _ctx) {
  renderFloor(_ctx, 0, -1, -1);
  renderClutter(_ctx);
  renderGreekFrames(_ctx, s_strA, s_strB, s_strC, s_lettersThisLevel);
  renderPlayer(_ctx);
  renderWalls(_ctx, true, true, true, true);
  renderWallClutter(_ctx);
  renderArrows(_ctx, 15, 5, 4);
}

void populateScroll(uint8_t* _target, const bool _correct) {
  const int8_t _clue = m_dungeon.m_roomNeedHintValue[ m_dungeon.m_level ][ m_dungeon.m_room ];
  //
  const uint8_t _cluePoint0 = rand() % s_lettersThisLevel;
  bool _secondCluePoint = false;
  for (int _c = 0; _c < s_lettersThisLevel - 1; ++_c) _secondCluePoint |= rand() % MAX_GREEK;
  int8_t _cluePoint1 = -1;
  if (_secondCluePoint) {
    do {
      _cluePoint1 = rand() % s_lettersThisLevel;
    } while (_cluePoint0 == _cluePoint1);
  }
  //
  for (int _c = 0; _c < s_lettersThisLevel; ++_c) {
    uint8_t _randSymbol = 0;
    do {
      _randSymbol = rand() % MAX_GREEK;
    } while (_randSymbol == _clue);
    if (_correct && (_c == _cluePoint0 || _c == _cluePoint1)) {
      _target[_c] = _clue;
    } else {
      _target[_c] = _randSymbol;
    }
  }
}

bool tickGreek(bool _doInit) {
  if (_doInit == true) {
    s_correct = randomiseChoices(s_choices, 0);
    switch(m_dungeon.m_difficulty) {
      case 0: s_lettersThisLevel = 3; break;
      case 1: s_lettersThisLevel = 4; break;
      default: s_lettersThisLevel = TOTAL_LETTERS;
    }

    populateScroll(s_strA, s_correct == 0);
    populateScroll(s_strB, s_correct == 1);
    populateScroll(s_strC, s_correct == 2);

    s_state = 0;
    m_player.m_position = GPoint(0, SIZE*9);
    addCluter(6, 0, 20); // Only left
    return false;
  }

 if (s_state == 0) {
   enterRoom(&s_state);
 } else if (s_state == 1) {
   setGameState(kAwaitInput);
   ++s_state;
 } else if (s_state == 2) {
   if (getPlayerChoice() != s_correct) {
     if (m_dungeon.m_lives > 0) --m_dungeon.m_lives;
     else m_dungeon.m_rooms[ m_dungeon.m_level ][ m_dungeon.m_room + 1 ] = kDeath;
   }
   moveToExit(&s_state);
 } else if (s_state == 3) {
   setGameState(kFadeOut);
 }
  return false;
}

#endif