#include "start.h"

static uint16_t s_state = 0;
static uint16_t s_choice = 0;

void initStart() {
  s_state = 0;
  s_choice = 0;
  m_player.m_position = GPoint(0, SIZE*9);
}

void updateProcStart(Layer* _thisLayer, GContext* _ctx) {

  renderFloor(_thisLayer, _ctx, 0);

  static const char* _a = "YOU";
  static const char* _b = "CANNOT";
  static const char* _c = "GO";
  static const char* _d = "BACK!";

  renderBorderText(_ctx, GRect(40, 15, 80, 100),  fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD), _a, 2, GTextAlignmentLeft);
  renderBorderText(_ctx, GRect(45, 45, 80, 100),  fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD), _b, 2, GTextAlignmentLeft);
  renderBorderText(_ctx, GRect(50, 75, 80, 100),  fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD), _c, 2, GTextAlignmentLeft);
  renderBorderText(_ctx, GRect(55, 105, 80, 100), fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD), _d, 2, GTextAlignmentLeft);

  renderPlayer(_thisLayer, _ctx);
  renderWalls(_thisLayer, _ctx, true, true, true, true);

  if (m_gameState == kAwaitInput && m_frame < ANIM_FPS/2) {
    drawBitmap(_ctx, m_arrow, 15, 4);
    drawBitmap(_ctx, m_arrow, 15, 8);
    drawBitmap(_ctx, m_arrow, 15, 12);
  }

}

void tickStart() {

  if (s_state == 0) { // start initial move
    m_player.m_target = GPoint(SIZE*3, SIZE*9);
    m_gameState = kMovePlayer;
    ++s_state;
  } else if (s_state == 1) { // initial move is done
    m_gameState = kAwaitInput;
    // do not increment state - done in click start
  } else if (s_state == 2) {
    switch (s_choice) {
      case 0: m_player.m_target = GPoint(SIZE*7, SIZE*5); break;
      case 1: m_player.m_target = GPoint(SIZE*7, SIZE*9); break;
      case 2: m_player.m_target = GPoint(SIZE*7, SIZE*13); break;
    }
    m_gameState = kMovePlayer;
    ++s_state;
  } else if (s_state == 3) {
    switch (s_choice) {
      case 0: m_player.m_target = GPoint(SIZE*17, SIZE*5); break;
      case 1: m_player.m_target = GPoint(SIZE*17, SIZE*9); break;
      case 2: m_player.m_target = GPoint(SIZE*17, SIZE*13); break;
    }
    m_gameState = kMovePlayer;
    ++s_state;
  } else if (s_state == 4) {
    m_gameState = kFadeOut;
  }
}

void clickStart(ButtonId _button) { // Choice entered

  ++s_state;
  m_gameState = kLevelSpecific;

  if (BUTTON_ID_UP == _button) s_choice = 0;
  else if (BUTTON_ID_SELECT == _button) s_choice = 1;
  else if (BUTTON_ID_DOWN == _button) s_choice = 2;

}
