#include "gamble.h"
#ifdef YCGBv2

#include "gamble.h"

static uint16_t s_state = 0;
static uint16_t s_spin = 0;
static int32_t s_angle = 0; 
static int32_t s_clack = 0; 
static uint8_t s_doClack = 0;
static int32_t s_v = 0;
static uint32_t s_slowdown = 0;

GambleOutcomes_t getGambleOutcome(void) {
  int16_t _angleDeg = TRIGANGLE_TO_DEG(s_angle);
  if (_angleDeg < 30) {
    switch (s_spin) {
      case 0: return kInstantDeath;
      case 1: return kDifficultyUp;
      case 2: return kEvilWind;
    } 
  } else if (_angleDeg < 150) {
    switch (s_spin) {
      case 0: case 1: return kClover;
      case 2: return kGiveClue;
    } 
  } else if (_angleDeg < 270) {
    return kUnlockShortcut;
  } else if (_angleDeg < 330) {
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

void updateProcGamble(GContext* _ctx) {
  renderFloor(_ctx, 0);
  renderClutter(_ctx);
  renderPlayer(_ctx);

  renderGamble(_ctx, s_spin, s_angle, s_doClack);
  if (s_doClack) --s_doClack;

  if (s_state < 10) renderPlayer(_ctx); // In front

  renderWalls(_ctx, true, true, false, true);
  renderWallClutter(_ctx);
  renderArrowsDetailed(_ctx, 15, 5, 4, 1, 0, 0, false);
  renderArrowsDetailed(_ctx, 9, 15, 0, 0, 0, 1, false);

  if (m_dungeon.m_gameOver) {
    drawBitmap(_ctx, m_fire[0], 5 + 3, 5 + 11);
    drawBitmap(_ctx, m_fire[1], 6 + 3, 4 + 11);
  }

  if (s_state == 5) {
    APP_LOG(APP_LOG_LEVEL_INFO,"Final angle %i, outcome: %i", TRIGANGLE_TO_DEG(s_angle), getGambleOutcome());
  }
}

bool checkShortcutRoom(uint8_t _l, uint8_t _r) {
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
  for (uint8_t _r = m_dungeon.m_room + 1; _r < m_dungeon.m_roomsPerLevel[_level]; ++_r) {
    if (checkShortcutRoom(_level, _r)) return;
  }
  for (uint8_t _l = _level + 1; _level < MAX_LEVELS; ++_l) {
    for (uint8_t _r = 0; _r < m_dungeon.m_roomsPerLevel[_l]; ++_r) {
      if (checkShortcutRoom(_l, _r)) return;
    }
  }
}

bool tickGamble(bool _doInit) {
  const Hints_t _hint = m_dungeon.m_roomNeedHint[m_dungeon.m_level][m_dungeon.m_room];
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position = GPoint(0, SIZE*9);
    addCluter(4, 0, 20); // Only left
    s_v = 5000;
    s_slowdown = 100 + rand() % 76; // 100, bit over two revolutions. 175, little over one revolution.
    s_angle = rand() % TRIG_MAX_ANGLE;
    s_clack = DEG_TO_TRIGANGLE((TRIGANGLE_TO_DEG(s_angle) + 30) % 60);
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
      m_player.m_target = GPoint(SIZE*3, SIZE*15);
      setGameState(kMovePlayer);
      s_state = 3;
    }
  } else if (s_state == 3) {
    m_player.m_target = GPoint(SIZE*9, SIZE*15);
    setGameState(kMovePlayer);
    s_state = 4;
  } else if (s_state == 4) {
    s_angle += s_v;
    if (s_angle > TRIG_MAX_ANGLE) s_angle -= TRIG_MAX_ANGLE;
    s_clack -= s_v;
    if (s_clack <= 0) {
      s_clack = DEG_TO_TRIGANGLE(60);
      s_doClack = 1;
    }
    s_v -= s_slowdown;
    if (s_slowdown > 50) --s_slowdown;
    if (s_v <= 0) s_state = 5;
  } else if (s_state == 5) {
    const GambleOutcomes_t _go = getGambleOutcome();
    if (_go == kEvilWind) {
      setDisplayMsg(txtEvilWind0());
    } else if (_go == kDifficultyUp) {
      setDisplayMsg(txtEvilBell0());
    } else if (_go == kClover) {
      setDisplayMsg(txtGoodFaerie0());
    } else if (_go == kUnlockShortcut) {
      setDisplayMsg(txtShortcut0());
    } else if (_go == kGiveClue) {
      setDisplayMsg(txtQuietVoice0());
    } else if (_go == kInstantDeath) {
      m_dungeon.m_gameOver = 1;
      vibes_long_pulse();
      setGameState(kFadeOut);
      return true;
    }
    setGameState(kDisplayMsg);
    ++s_state;
  } else if (s_state == 6) {
    const GambleOutcomes_t _go = getGambleOutcome();
    if (_go == kEvilWind) {
      setDisplayMsg(txtEvilWind1());
      if (m_dungeon.m_lives > 0) --m_dungeon.m_lives;
    } else if (_go == kDifficultyUp) {
      setDisplayMsg(txtEvilBell1());
      ++m_dungeon.m_difficulty;
    } else if (_go == kClover) {
      setDisplayMsg(txtGoodFaerie1());
      ++m_dungeon.m_lives;
    } else if (_go == kGiveClue) {
      setDisplayMsg(txtQuietVoice1());
    } else if (_go == kUnlockShortcut) {
      setDisplayMsg(txtShortcut1());
      doShortcut();
    }
    setGameState(kDisplayMsg);
    ++s_state;
  } else if (s_state == 7) {
    m_player.m_target = GPoint(SIZE*12, SIZE*15);
    setGameState(kMovePlayer);
    s_state = 8;
  } else if (s_state == 8) {
    m_player.m_target = GPoint(SIZE*17, SIZE*13);
    setGameState(kMovePlayer);
    s_state = 11;
  } else if (s_state == 10) {
    moveToExit(&s_state);
  } else if (s_state == 11) {
   setGameState(kFadeOut);
  }

  return true;
}

#endif