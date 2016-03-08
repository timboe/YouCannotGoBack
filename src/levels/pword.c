#include "pword.h"

static uint16_t s_state = 0;
static int8_t s_choices[3] = {0};
static uint16_t s_correct = 0;

void updateProcPword(GContext* _ctx) {

  renderFloor(_ctx, 0);

  renderPlayer(_ctx);
  renderWalls(_ctx, true, false, true, false);

  drawBitmap(_ctx, m_guardian, 5, 6);

  if (s_state < 7) {
    drawBitmap(_ctx, m_blockWall[0], 9, 0);
    for (int _i = 2; _i < 18; _i += 2) drawBitmap(_ctx, m_blockWall[1], 9, _i);
    drawBitmap(_ctx, m_blockWall[2], 9, 18);
  }

  renderClutter(_ctx);

  if (s_state == 5) {
    for (int _i = 0; _i < 3; ++_i) {
      if (m_dungeon.m_roomNeedHint[ m_dungeon.m_level ][ m_dungeon.m_room] == kNumber ) {
        GRect _r = GRect(SIZE*10, SIZE*(5 + 4*_i),  SIZE*6, SIZE*2);
        renderHintNumber(_ctx, _r, s_choices[_i], true);
      } else {
        GRect _r = GRect(SIZE*10, SIZE*(4 + 4*_i),  SIZE*6, SIZE*2);
        renderBorderText(_ctx, _r, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), m_spellNames[s_choices[_i]], 2, GTextAlignmentCenter, false);
      }
    }
    if (getGameState() == kAwaitInput && getFrameCount() < ANIM_FPS/2) {
      drawBitmap(_ctx, m_arrow, 12, 3);
      drawBitmap(_ctx, m_arrow, 12, 7);
      drawBitmap(_ctx, m_arrow, 12, 11);
    }
  }

}

bool tickPword(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position = GPoint(0, SIZE*9);
    addCluter(4, 0, 20);
    s_correct = randomiseChoices(s_choices, 0);
    return false;
  }

  static const char _msg[] = "PASSWORD?";

  if (s_state == 0) { // start initial move
    enterRoom(&s_state);
  } else if (s_state == 1) { // initial move is done
    m_player.m_target = GPoint(SIZE*3, SIZE*12); // move down
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 2) {
    m_player.m_target = GPoint(SIZE*6, SIZE*12); // move right
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 3) { // display msg
    setDisplayMsg(_msg);
    setGameState(kDisplayMsg);
    vibes_double_pulse();
    ++s_state;
  } else if (s_state == 4) { // move down stairs
    setGameState(kAwaitInput);
    ++s_state;
  } else if (s_state == 5) {
    if (getPlayerChoice() == s_correct) {
      s_state = 7; // Yay
    } else {
      s_state = 6; // Oh noes!
      vibes_long_pulse();
    }
  } else if (s_state == 6) { // DEATH MOVE
    m_player.m_position.y -= 1;
    if (m_player.m_position.y < 8*SIZE) {
      setGameState(kFadeOut);
      m_dungeon.m_gameOver = true;
    }
    return true;
  } else if (s_state == 7) { // WIN MOVE
    m_player.m_target = GPoint(SIZE*9, SIZE*12);
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 8) { // WIN MOVE
    m_player.m_target = GPoint(SIZE*16, SIZE*9);
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 9) {
    setGameState(kFadeOut);
  }

  return false;
}
