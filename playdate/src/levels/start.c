#include "start.h"

static uint16_t s_state = 0;

void updateProcStart(PlaydateAPI* _pd) {

  renderFloor(_pd, 0);

  static const char* _a = "YOU";
  static const char* _b = "CANNOT";
  static const char* _c = "GO";
  static const char* _d = "BACK";

  const static int _yoff = SIZE;
  PDRect _ra = {.x = 45, .y = 15+_yoff, .width = 80, .height = 100};
  PDRect _rb = {.x = 45, .y = 45+_yoff, .width = 80, .height = 100};
  PDRect _rc = {.x = 45, .y = 75+_yoff, .width = 80, .height = 100};
  PDRect _rd = {.x = 45, .y = 105+_yoff, .width = 80, .height = 100};

  static const int _offset = 2;
  renderBorderText(_pd, _ra, m_fontMain, _a, _offset, false);
  renderBorderText(_pd, _rb, m_fontMain, _b, _offset, true);
  renderBorderText(_pd, _rc, m_fontMain, _c, _offset, false);
  renderBorderText(_pd, _rd, m_fontMain, _d, _offset, true);
  renderClutter(_pd);
  renderPlayer(_pd);
  renderWalls(_pd, true, true, true, true);
  renderWallClutter(_pd);

  drawCBitmap(_pd, &m_flagstone[ m_dungeon.m_finalPuzzle ], 0, 9);

  renderArrows(_pd, 15, 4, 4);
}

bool tickStart(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position_x = 0;
    m_player.m_position_y = SIZE*9;
    addCluter(4, 0, 20); // Only left
    return false;
  }

  static bool _first = false; // Redundant on playdate
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
