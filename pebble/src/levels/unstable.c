#include "unstable.h"
#ifdef YCGBv2

static uint16_t s_state = 0;
static int8_t _offsetX[6][8] = {{0},{0},{0},{0},{0},{0}};
static int8_t _offsetY[6][8] = {{0},{0},{0},{0},{0},{0}};

    // }

void updateProcUnstable(GContext* _ctx) {
  renderFloor(_ctx, kUnstable, -1, -1, _offsetX, _offsetY);
  rednerUnstableMarkers(_ctx);
  renderPlayer(_ctx);
  renderWalls(_ctx, true, true, true, true);
  renderWallClutter(_ctx);
  renderArrows(_ctx, 9, 3, 12);
}

bool tickUnstable(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position = GPoint(0, SIZE*9);
    addCluter(20, 20, 0);
    return false;
  }

  if (s_state == 0) { // start initial move
    enterRoom(&s_state);
  } else if (s_state == 1) { // initial move is done
    setGameState(kLevelSpecificWButtons);
    ++s_state;
  } else if (s_state == 2) {

     switch (getPlayerChoice()) {
       case 0: m_player.m_target = GPoint(SIZE*9, SIZE*3); setGameState(kMovePlayer); ++s_state; break;
       case 1: break;
       case 2: m_player.m_target = GPoint(SIZE*9, SIZE*15); setGameState(kMovePlayer); ++s_state; break;
     }
  } else if (s_state == 3) {
    // setGameState(kFadeOut);
  }

  if (m_dungeon.m_ticksInLevel % 2 == 0) {
    srand(m_dungeon.m_ticksInLevel);
    for (uint8_t _x = 0; _x < 6; ++_x) {
      for (uint8_t _y = 0; _y < 8; ++_y) {
        _offsetX[_x][_y] = rand() % 4 - 2;
        _offsetY[_x][_y] = rand() % 4 - 2;
      }
    }
  }

  return true;
}


#endif