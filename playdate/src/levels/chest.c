#include "chest.h"
#include "../sound.h"

static uint16_t s_state = 0;
static int8_t s_bad = 0;

void updateProcChest(PlaydateAPI* _pd) {

  renderFloor(_pd, 0);

  for (int _i = 0; _i < 3; ++_i) {
    drawCBitmap(_pd, &m_table, 7, 5 + (_i * 4));
    drawCBitmap(_pd, &m_chest, 8, 4 + (_i * 4));
  }

  renderClutter(_pd);
  renderPlayer(_pd);
  renderWalls(_pd, true, true, true, true);
  renderWallClutter(_pd);
  renderArrows(_pd, 8, 3, 4);
}

bool tickChest(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position_x = 0;
    m_player.m_position_y = SIZE*9;
    addCluter(4, 0, 20); // Only left
    s_bad = rand() % 3;
    return false;
  }

  static const char _badA[] = "AN EVIL WIND!";
  static const char _badB[] = "YOU FEEL BAD";

  static const char _goodA[] = "A GOOD FAERIE!";
  static const char _goodB[] = "FEELING LUCKY";

  static const char _goodC[] = "A QUIET VOICE";
  static char _goodD[16];

  Hints_t _hint = m_dungeon.m_roomNeedHint[m_dungeon.m_level][m_dungeon.m_room];

  if (s_state == 0) { // start initial move
    enterRoom(&s_state);
  } else if (s_state == 1) { // initial move is done
    setGameState(kAwaitInput);
    ++s_state;
  } else if (s_state == 2) {
    m_player.m_target_x = SIZE*8;
    switch (getPlayerChoice()) {
      case 0: m_player.m_target_y = SIZE*6; break;
      case 1: m_player.m_target_y = SIZE*10; break;
      case 2: m_player.m_target_y = SIZE*14; break;
    }
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 3) {
    chestSound();
    if (getPlayerChoice() == s_bad) {
      setDisplayMsg(_badA);
      if (m_dungeon.m_lives > 0) --m_dungeon.m_lives;
    } else if ( _hint == kSpell) {
      setDisplayMsg(_goodC);
      snprintf(_goodD, 16, "REMEMBER %s", m_spellNames[ m_dungeon.m_roomNeedHintValue[m_dungeon.m_level][m_dungeon.m_room] ]);
    } else if (_hint == kNumber) {
      setDisplayMsg(_goodC);
      snprintf(_goodD, 16, "REMEMBER %i", m_dungeon.m_roomNeedHintValue[m_dungeon.m_level][m_dungeon.m_room]);
    } else {
      setDisplayMsg(_goodA);
      ++m_dungeon.m_lives;
    }
    setGameState(kDisplayMsg);
    ++s_state;
  } else if (s_state == 4) {
    if (getPlayerChoice() == s_bad) setDisplayMsg(_badB);
    else if (_hint == kSpell || _hint == kNumber) setDisplayMsg(_goodD);
    else setDisplayMsg(_goodB);
    setGameState(kDisplayMsg);
    ++s_state;
  } else if (s_state == 5) {
    moveToExit(&s_state);
  } else if (s_state == 6) {
    setGameState(kFadeOut);
  }

  return false;
}
