#include "chest.h"

static uint16_t s_state = 0;
static int8_t s_bad = 0;

void updateProcChest(GContext* _ctx) {

  renderFloor(_ctx, 0);

  for (int _i = 0; _i < 3; ++_i) {
    drawBitmap(_ctx, m_table, 7, 5 + (_i * 4));
    drawBitmap(_ctx, m_chest, 8, 4 + (_i * 4));
  }

  renderPlayer(_ctx);
  renderWalls(_ctx, true, false, true, false);
  renderClutter(_ctx);

  if (getGameState() == kAwaitInput && getFrameCount() < ANIM_FPS/2) {
    drawBitmap(_ctx, m_arrow, 8, 2);
    drawBitmap(_ctx, m_arrow, 8, 6);
    drawBitmap(_ctx, m_arrow, 8, 10);
  }

}

bool tickChest(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position = GPoint(0, SIZE*9);
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
    switch (getPlayerChoice()) {
      case 0: m_player.m_target = GPoint(SIZE*8, SIZE*6); break;
      case 1: m_player.m_target = GPoint(SIZE*8, SIZE*10); break;
      case 2: m_player.m_target = GPoint(SIZE*8, SIZE*14); break;
    }
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 3) {
    if (getPlayerChoice() == s_bad) {
      setDisplayMsg(_badA);
      if (m_dungeon.m_lives > 0) --m_dungeon.m_lives;
    } else if ( _hint == kSpell) {
      setDisplayMsg(_goodC);
      snprintf(_goodD, 16, "REMEMBER, %s", m_spellNames[ m_dungeon.m_roomNeedHintValue[m_dungeon.m_level][m_dungeon.m_room] ]);
    } else if (_hint == kNumber) {
      setDisplayMsg(_goodC);
      snprintf(_goodD, 16, "REMEMBER, %i", m_dungeon.m_roomNeedHintValue[m_dungeon.m_level][m_dungeon.m_room]);
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
    m_player.m_target = GPoint(SIZE*16, SIZE*9);
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 6) {
    setGameState(kFadeOut);
  }

  return false;
}
