#include "gamble.h"
#include "../sound.h"

static uint16_t s_state = 0;
static uint16_t s_spin = 0;
static float s_angle = 0; 
static float s_clack = 0; 
static uint8_t s_doClack = 0;
static float s_v = 0;
static float s_slowdown = 0;

static const char _badA[] = "AN EVIL WIND!";
static const char _badB[] = "YOU FEEL BAD";

static const char _badC[] = "AN EVIL BELL!";
static const char _badD[] = "YOU FEEL HEAVY";

static const char _goodA[] = "A GOOD OMEN!";
static const char _goodB[] = "FEELING LUCKY";

static const char _goodC[] = "AN AGED SIGN";
static const char _goodD[] = "SHORTCUT FOUND";

  static const char _remindA[] = "A QUIET VOICE";
  static char _remindB[16];

void updateProcGamble(PlaydateAPI* _pd) {
  renderFloor(_pd, 0);
  renderClutter(_pd);
  renderPlayer(_pd);

  drawBitmapAbsRot(_pd, m_spin[s_spin], 79, 73, s_angle);
  drawBitmapAbs(_pd, m_wheel, 38, 29);

  uint8_t _cID = 0;
  if (s_doClack) {
    --s_doClack;
    _cID = 1;
  }
  drawBitmapAbs(_pd, _pd->graphics->getTableBitmap(m_clack, _cID), 70, 29);

  if (s_state < 10) renderPlayer(_pd); // In front

  renderWalls(_pd, true, true, false, true);
  renderWallClutter(_pd);
  renderArrows2(_pd, 15, 5, 4, 1, 0, 0, false);
  renderArrows2(_pd, 9, 15, 0, 0, 0, 1, false);

  if (m_dungeon.m_gameOver) {
    drawCBitmap(_pd, &m_fire[0], 5 + 3, 5 + 11);
    drawCBitmap(_pd, &m_fire[1], 6 + 3, 4 + 11);
  }
}

GambleOutcomes_t getGambleOutcome(void) {
  if (s_angle < 30.0f) {
    switch (s_spin) {
      case 0: return kInstantDeath;
      case 1: return kDifficultyUp;
      case 2: return kEvilWind;
    } 
  } else if (s_angle < 150.0f) {
    switch (s_spin) {
      case 0: case 1: return kClover;
      case 2: return kGiveClue;
    } 
  } else if (s_angle < 270.0f) {
    return kUnlockShortcut;
  } else if (s_angle < 330.0f) {
    switch (s_spin) {
      case 0: case 1: return kDifficultyUp;
      case 2: return kEvilWind;
    }
  } else {
    switch (s_spin) {
      case 0: return kInstantDeath;
      case 1: return kDifficultyUp;
      case 2: return kEvilWind;
    }
  }
  return kClover;
}

bool checkShortcutRoom(int _l, int _r) {
  if (m_dungeon.m_roomGiveHint[_l][_r]) return false; // Don't skip clue rooms
  if (m_dungeon.m_roomNeedHint[_l][_r]) return false; // Don't skip clue rooms
  if (m_dungeon.m_rooms[_l][_r] == kStairs) return false; // Don't skip level transitions
  if (m_dungeon.m_rooms[_l][_r] == kShortcut) return false; // Don't skip other shortcuts
  if (m_dungeon.m_rooms[_l][_r] == kFinal) return false; // Don't skip the final chamber
  m_dungeon.m_rooms[_l][_r] = kShortcut;
  return true;
}

void doShortcut(void) {
  const int8_t _level = m_dungeon.m_level;
  for (int _r = m_dungeon.m_room + 1; _r < m_dungeon.m_roomsPerLevel[_level]; ++_r) {
    if (checkShortcutRoom(_level, _r)) return;
  }
  for (int _l = _level + 1; _level < MAX_LEVELS; ++_l) {
    for (int _r = 0; _r < m_dungeon.m_roomsPerLevel[_l]; ++_r) {
      if (checkShortcutRoom(_l, _r)) return;
    }
  }
}

bool tickGamble(bool _doInit) {
  const Hints_t _hint = m_dungeon.m_roomNeedHint[m_dungeon.m_level][m_dungeon.m_room];
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position_x = 0;
    m_player.m_position_y = SIZE*9;
    addCluter(4, 0, 20); // Only left
    s_v = 50.0f;
    // 0.5 - 1.5 is a little over one full revolution,
    // hence all possible end points are accessible
    s_slowdown = 0.5f + (0.01f * (rand()%100)); 
    // On top of this, start at a random angle too
    s_angle = rand() % 365;
    s_clack = (int)(s_angle + 30) % 60;
    // Unusually, prefer the easier wheel (w/o instant death) at higher levels
    s_spin = rand() % 2;
    if (m_dungeon.m_difficulty == 1) s_spin = rand() % 3;
    else if (m_dungeon.m_difficulty >= 2) s_spin = 1 + (rand() % 2);
    // Don't allow the third wheel if there is no clue
    if (_hint == kNoHint && s_spin == 2) --s_spin;
    return false; 
  }

  if (s_state == 0) { // start initial move
   enterRoom(&s_state);
  } else if (s_state == 1) {
   setGameState(kAwaitInput);
   ++s_state;
  } else if (s_state == 2) {
    if (getPlayerChoice() == 0) { 
      s_state = 10; // leave  
    } else {
      m_player.m_target_x = SIZE*3;
      m_player.m_target_y = SIZE*15;
      setGameState(kMovePlayer);
      s_state = 3;
    }
  } else if (s_state == 3) {
    m_player.m_target_x = SIZE*9;
    m_player.m_target_y = SIZE*15;
    setGameState(kMovePlayer);
    s_state = 4;
  } else if (s_state == 4) {
    s_angle += s_v;
    s_clack -= s_v;
    if (s_clack <= 0.0f) {
      s_clack = 60.0f;
      s_doClack = 1;
      clackSound();
    }
    if (s_angle > 365.0f) s_angle -= 365.0f;
    s_v *= 0.97f;
    s_v -= s_slowdown;
    if (s_v <= 0) s_state = 5;
  } else if (s_state == 5) {
    const GambleOutcomes_t _go = getGambleOutcome();
    if (_go == kEvilWind) {
      setDisplayMsg(_badA);
      hitSound();
    } else if (_go == kDifficultyUp) {
      setDisplayMsg(_badC);
      bellSound();
    } else if (_go == kClover) {
      setDisplayMsg(_goodA);
      hitSound();
    } else if (_go == kUnlockShortcut) {
      setDisplayMsg(_goodC);
      hitSound();
    } else if (_go == kGiveClue) {
      setDisplayMsg(_remindA);
      hitSound();
      if ( _hint == kSpell) {
        snprintf(_remindB, 16, "REMEMBER %s", m_spellNames[ m_dungeon.m_roomNeedHintValue[m_dungeon.m_level][m_dungeon.m_room] ]);
      } else if (_hint == kNumber) {
        snprintf(_remindB, 16, "REMEMBER %i", m_dungeon.m_roomNeedHintValue[m_dungeon.m_level][m_dungeon.m_room]);
      }
    } else if (_go == kInstantDeath) {
      m_dungeon.m_gameOver = 1;
      setGameState(kFadeOut);
      fireSound();
      return true;
    }
    setGameState(kDisplayMsg);
    ++s_state;
  } else if (s_state == 6) {
    const GambleOutcomes_t _go = getGambleOutcome();
    if (_go == kEvilWind) {
      setDisplayMsg(_badB);
      debufSound();
      if (m_dungeon.m_lives > 0) --m_dungeon.m_lives;
    } else if (_go == kDifficultyUp) {
      setDisplayMsg(_badD);
      ++m_dungeon.m_difficulty;
    } else if (_go == kClover) {
      setDisplayMsg(_goodB);
      bufSound();
      ++m_dungeon.m_lives;
    } else if (_go == kGiveClue) {
      setDisplayMsg(_remindB);
      reminderSound();
    } else if (_go == kUnlockShortcut) {
      bufSound();
      setDisplayMsg(_goodD);
      doShortcut();
    }
    setGameState(kDisplayMsg);
    ++s_state;
  } else if (s_state == 7) {
    m_player.m_target_x = SIZE*12;
    m_player.m_target_y = SIZE*15;
    setGameState(kMovePlayer);
    s_state = 8;
  } else if (s_state == 8) {
    m_player.m_target_x = SIZE*17;
    m_player.m_target_y = SIZE*13;
    setGameState(kMovePlayer);
    s_state = 11;

  } else if (s_state == 10) {
    moveToExit(&s_state);
  } else if (s_state == 11) {
   setGameState(kFadeOut);
  }

  return true;
}
