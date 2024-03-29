#include "pword.h"
#include "../sound.h"

static uint16_t s_state = 0;
static int8_t s_choices[3] = {0};
static uint16_t s_correct = 0;

void updateProcPword(PlaydateAPI* _pd) {

  renderFloor(_pd, 0);
  renderClutter(_pd);
  renderPlayer(_pd);
  renderWalls(_pd, true, false, true, false);

  drawCBitmap(_pd, &m_guardian, 5, 6);

  if (s_state < 7) {
    drawCBitmap(_pd, &m_blockWall[0], 9, 0);
    for (int _i = 2; _i < 18; _i += 2) drawCBitmap(_pd, &m_blockWall[1], 9, _i);
    drawCBitmap(_pd, &m_blockWall[2], 9, 18);
  }
  renderWallClutter(_pd);

  if (s_state == 5) {
    for (int _i = 0; _i < 3; ++_i) {
      if (m_dungeon.m_roomNeedHint[ m_dungeon.m_level ][ m_dungeon.m_room] == kNumber ) {
        PDRect _r = {.x = SIZE*10, .y = SIZE*(6 + 4*_i), .width = SIZE*6, .height = SIZE*2};
        renderHintNumber(_pd, _r, s_choices[_i], true);
      } else {
        PDRect _r = {.x = SIZE*10, .y = SIZE*(6 + 4*_i), .width = SIZE*6, .height = SIZE*2};
        renderBorderText(_pd, _r, m_fontMain, m_spellNames[s_choices[_i]], 2, false);
      }
    }
    renderArrows(_pd, 12, 4, 4);
  }

}

bool tickPword(PlaydateAPI* _pd, bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position_x = 0;
    m_player.m_position_y = SIZE*9;
    addCluter(4, 0, 20);
    s_correct = randomiseChoices(_pd, s_choices, 0);
    return false;
  }

  static const char _msg[] = "PASSWORD?";

  if (s_state == 0) { // start initial move
    enterRoom(&s_state);
  } else if (s_state == 1) { // initial move is done
    m_player.m_target_x = SIZE*3; // move down
    m_player.m_target_y = SIZE*12; // move down
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 2) {
    m_player.m_target_x = SIZE*6; // move right
    m_player.m_target_y = SIZE*12; // move right
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 3) { // display msg
    setDisplayMsg(_msg);
    setGameState(kDisplayMsg);
    passwordSound();
    ++s_state;
  } else if (s_state == 4) { // move down stairs
    setGameState(kAwaitInput);
    ++s_state;
  } else if (s_state == 5) {
    if (getPlayerChoice() == s_correct) {
      s_state = 7; // Yay
      reminderSound();
    } else {
      s_state = 6; // Oh noes!
      boomSound();
    }
  } else if (s_state == 6) { // DEATH MOVE
    m_player.m_position_y -= 1;
    if (m_player.m_position_y < 8*SIZE) {
      setGameState(kFadeOut);
      m_dungeon.m_gameOver = true;
    }
    return true;
  } else if (s_state == 7) { // WIN MOVE
    m_player.m_target_x = SIZE*9;
    m_player.m_target_y = SIZE*12;
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 8) { // WIN MOVE
    m_player.m_target_x = SIZE*16;
    m_player.m_target_y = SIZE*9;
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 9) {
    setGameState(kFadeOut);
  }

  return false;
}
