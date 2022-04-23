#include "start.h"

static uint16_t s_state = 0;

void updateProcStart(GContext* _ctx) {

  renderFloor(_ctx, 0);

  static const char* _a = "YOU";
  static const char* _b = "CANNOT";
  static const char* _c = "GO";
  static const char* _d = "BACK!";

  renderBorderText(_ctx, GRect(45, 15, 80, 100),  fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD), _a, 2, GTextAlignmentCenter, false);
  renderBorderText(_ctx, GRect(45, 45, 80, 100),  fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD), _b, 2, GTextAlignmentCenter, true);
  renderBorderText(_ctx, GRect(45, 75, 80, 100),  fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD), _c, 2, GTextAlignmentCenter, false);
  renderBorderText(_ctx, GRect(45, 105, 80, 100), fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD), _d, 2, GTextAlignmentCenter, true);
  renderClutter(_ctx);
  renderPlayer(_ctx);
  renderWalls(_ctx, true, true, true, true);
  renderWallClutter(_ctx);

  drawBitmap(_ctx, m_flagstone[ m_dungeon.m_finalPuzzle ], 0, 9);

  renderArrows(_ctx, 15, 4, 4);
}

bool tickStart(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position = GPoint(0, SIZE*9);
    addCluter(4, 0, 20); // Only left
    return false;
  }

  static bool _first = true;
  static const char _msgA[] = "SHAKE TO TURN...";
  static const char _msgB[] = "ON BACKLIGHT";

  if (s_state == 0) { // start initial move
    enterRoom(&s_state);
  } else if (s_state == 1) { // initial move is done
    if (_first == true) {
      setDisplayMsg(_msgA);
      setGameState(kDisplayMsg);
      s_state = 2;
      _first = false;
    } else {
      setGameState(kAwaitInput);
      s_state = 4;
    }
  } else if (s_state == 2) {
    setDisplayMsg(_msgB);
    setGameState(kDisplayMsg);
    ++s_state;
  } else if (s_state == 3) {
    setGameState(kAwaitInput);
    ++s_state;
  } else if (s_state == 4) {
    moveToExit(&s_state);
  } else if (s_state == 5) {
    setGameState(kFadeOut);
  }

  return false;
}
