#include "start.h"

static uint16_t s_state = 0;
static uint16_t s_choice = 0;

void initStart() {
  s_state = 0;
  s_choice = 0;
  m_player.m_position = GPoint(0, SIZE*9);
  addCluter(2, 4, 0, 20); // Only left
}

void updateProcStart(GContext* _ctx) {

  renderFloor(_ctx, 0);

  static const char* _a = "YOU";
  static const char* _b = "CANNOT";
  static const char* _c = "GO";
  static const char* _d = "BACK!";

  renderBorderText(_ctx, GRect(45, 15, 80, 100),  fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD), _a, 2, GTextAlignmentCenter);
  renderBorderText(_ctx, GRect(45, 45, 80, 100),  fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD), _b, 2, GTextAlignmentCenter);
  renderBorderText(_ctx, GRect(45, 75, 80, 100),  fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD), _c, 2, GTextAlignmentCenter);
  renderBorderText(_ctx, GRect(45, 105, 80, 100), fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD), _d, 2, GTextAlignmentCenter);

  renderPlayer(_ctx);
  renderWalls(_ctx, true, true, true, true);
  renderClutter(_ctx);

  if (getGameState() == kAwaitInput && getFrameCount() < ANIM_FPS/2) {
    drawBitmap(_ctx, m_arrow, 15, 4);
    drawBitmap(_ctx, m_arrow, 15, 8);
    drawBitmap(_ctx, m_arrow, 15, 12);
  }

}

void tickStart() {

  if (s_state == 0) { // start initial move
    enterRoom(&s_state);
  } else if (s_state == 1) { // initial move is done
    setGameState(kAwaitInput);  // do not increment state - done in click start
  } else if (s_state == 2) {
    switch (s_choice) {
      case 0: m_player.m_target = GPoint(SIZE*7, SIZE*5); break;
      case 1: m_player.m_target = GPoint(SIZE*7, SIZE*9); break;
      case 2: m_player.m_target = GPoint(SIZE*7, SIZE*13); break;
    }
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 3) {
    switch (s_choice) {
      case 0: m_player.m_target = GPoint(SIZE*17, SIZE*5); break;
      case 1: m_player.m_target = GPoint(SIZE*17, SIZE*9); break;
      case 2: m_player.m_target = GPoint(SIZE*17, SIZE*13); break;
    }
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 4) {
    setGameState(kFadeOut);
  }
}

void clickStart(ButtonId _button) { // Choice entered

  ++s_state;
  setGameState(kLevelSpecific);

  if (BUTTON_ID_UP == _button) s_choice = 0;
  else if (BUTTON_ID_SELECT == _button) s_choice = 1;
  else if (BUTTON_ID_DOWN == _button) s_choice = 2;

}
