#include "chest.h"

static uint16_t s_state = 0;
static int8_t s_bad = 0;

void updateProcChest(GContext* _ctx) {

  renderFloor(_ctx, kEmpty, -1, -1, NULL, NULL);

  // repeat on a 8s loop
  const uint16_t _t = m_dungeon.m_ticksInLevel % (ANIM_FPS * 8);
  const uint8_t _d = m_dungeon.m_difficulty;
  for (int _i = 0; _i < 3; ++_i) {
    bool _badChest = (_i == s_bad);
    if (_d >= 1) {
      if (_t < 2*ANIM_FPS) _badChest = false; // Show later
      if (_t > 4*ANIM_FPS && _d >= 2) _badChest = false; // Then hide
    }
    drawBitmap(_ctx, m_table, 7, 5 + (_i * 4));
    drawBitmap(_ctx, m_chest[_badChest ? 1 : 0], 8, 4 + (_i * 4));
  }

  renderClutter(_ctx);
  renderPlayer(_ctx);
  renderWalls(_ctx, true, true, true, true);
  renderWallClutter(_ctx);
  renderArrows(_ctx, 8, 2, 4);
}

bool tickChest(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position = GPoint(0, SIZE*9);
    addCluter(4, 0, 20); // Only left
    s_bad = rand() % 3;
    return false;
  }
  
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
      setDisplayMsg(txtEvilWind0());
      if (m_dungeon.m_lives > 0) --m_dungeon.m_lives;
    } else if ( _hint == kSpell || _hint == kNumber ) {
      setDisplayMsg(txtQuietVoice0());
    } else {
      setDisplayMsg(txtGoodFaerie0());
      ++m_dungeon.m_lives;
    }
    setGameState(kDisplayMsg);
    ++s_state;
  } else if (s_state == 4) {
    if (getPlayerChoice() == s_bad) setDisplayMsg(txtEvilWind1());
    else if (_hint == kSpell || _hint == kNumber) setDisplayMsg(txtQuietVoice1());
    else setDisplayMsg(txtGoodFaerie1());
    setGameState(kDisplayMsg);
    ++s_state;
  } else if (s_state == 5) {
    moveToExit(&s_state);
  } else if (s_state == 6) {
    setGameState(kFadeOut);
  }

  return false;
}
