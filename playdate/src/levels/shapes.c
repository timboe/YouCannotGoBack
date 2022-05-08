#include "shapes.h"
#include "common.h"
#include "../sound.h"

static uint16_t s_state = 0;
static uint8_t s_win = 0;
static LCDBitmap* s_shapes[3] = {NULL};
static LCDBitmap* s_shapes_rot[5] = {NULL};
static uint8_t s_outer[3][4] = {0};
static uint8_t s_inner[3][4] = {0};

void updateProcShapes(PlaydateAPI* _pd) {
  renderFloor(_pd, 0);

  drawBitmapAbs(_pd, s_shapes_rot[0], 65, 25);
  drawBitmapAbs(_pd, s_shapes_rot[1], 77, 57);
  drawBitmapAbs(_pd, s_shapes_rot[2], 65, 89);
  drawBitmapAbs(_pd, s_shapes_rot[4], 25, 57);

  renderClutter(_pd);
  renderPlayer(_pd);
  renderWalls(_pd, true, true, true, true);
  renderWallClutter(_pd);

  if ((getGameState() == kAwaitInput || getGameState() == kLevelSpecificWButtons) && getFrameCount() < ANIM_FPS/2) {
    drawCBitmap(_pd, &m_arrow_u, 10, 5);
    drawCBitmap(_pd, &m_arrow_r, 12, 9);
    drawCBitmap(_pd, &m_arrow_d, 10, 13);
    //if (getFrameCount() == 0) beepSound();
  }

}


bool tickShapes(PlaydateAPI* _pd, bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    s_win = rand() % 3;
    m_player.m_position_x = 0;
    m_player.m_position_y = SIZE*9;
    addCluter(4, 0, 20); // Only left

    if (s_shapes_rot[3] == NULL) {
      s_shapes_rot[3] = _pd->graphics->newBitmap(45, 45, kColorClear);
      s_shapes_rot[4] = _pd->graphics->newBitmap(45, 45, kColorClear);
    }

    for (int i = 0; i < 3; ++i) {
      if (s_shapes[i] == NULL) {
        s_shapes[i] = _pd->graphics->newBitmap(32, 32, kColorWhite);
        s_shapes_rot[i] = _pd->graphics->newBitmap(45, 45, kColorClear);
      }
      _pd->graphics->pushContext(s_shapes[i]);
      _pd->graphics->fillRect(1, 1, 30, 30, kColorBlack);
      for (int j = 0; j < 4; ++j) {
        s_outer[i][j] = rand() % 3;
        s_inner[i][j] = rand() % 3;
      }
      _pd->graphics->fillRect(2,  2,  14, 14, getShieldColor(s_outer[i][0]));
      _pd->graphics->fillRect(16, 2,  14, 14, getShieldColor(s_outer[i][1]));
      _pd->graphics->fillRect(2,  16, 14, 14, getShieldColor(s_outer[i][2]));
      _pd->graphics->fillRect(16, 16, 14, 14, getShieldColor(s_outer[i][3]));
      _pd->graphics->fillEllipse(5,    5,    8, 8, 0.0f, 0.0f, getShieldColor(s_inner[i][0]));
      _pd->graphics->fillEllipse(14+5, 5,    8, 8, 0.0f, 0.0f, getShieldColor(s_inner[i][1]));
      _pd->graphics->fillEllipse(5,    14+5, 8, 8, 0.0f, 0.0f, getShieldColor(s_inner[i][2]));
      _pd->graphics->fillEllipse(14+5, 14+5, 8, 8, 0.0f, 0.0f, getShieldColor(s_inner[i][3]));
      _pd->graphics->popContext();
      _pd->graphics->pushContext(s_shapes_rot[i]);
      _pd->graphics->drawRotatedBitmap(s_shapes[i], 22, 22, 45.0f, 0.5f, 0.5f, 1.0f, 1.0f);
      _pd->graphics->popContext();
    }

    int _rot = m_dungeon.m_level > 0 ? rand() % 4 : 0;
    int _f = m_dungeon.m_level > 1 ? rand() % 3 : 0;
    LCDBitmapFlip _flip = kBitmapUnflipped;
    switch (_f) {
      case 1: _flip = kBitmapFlippedX; break;
      case 2: _flip = kBitmapFlippedY; break;
    }

    _pd->graphics->pushContext(s_shapes_rot[3]);
    _pd->graphics->drawRotatedBitmap(s_shapes_rot[s_win], 22, 22, (float)(90.0f*_rot), 0.5f, 0.5f, 1.0f, 1.0f);
    _pd->graphics->popContext();

    _pd->graphics->pushContext(s_shapes_rot[4]);
    _pd->graphics->drawBitmap(s_shapes_rot[3], 0, 0, _flip);
    _pd->graphics->popContext();

    #ifdef DEV
    _pd->system->logToConsole("Shapes win:%i, rot:%i, flip:%i",s_win,_rot,_f);
    #endif
  }


  if (s_state == 0) { // start initial move
    m_player.m_target_x = SIZE*5;
    m_player.m_target_y = SIZE*9;
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 1) { // initial move is done
    setGameState(kAwaitInput);
    ++s_state;
  } else if (s_state == 2) {
    moveToExit(&s_state);
  } else if (s_state == 3) {
    setGameState(kFadeOut);
  }


  return false;
}
