#include "shapes.h"
#ifdef YCGBv2


static uint16_t s_state = 0;
static uint8_t s_win = 0;
static uint8_t s_outer[3][4] = {{0},{0},{0}};
static uint8_t s_inner[3][4] = {{0},{0},{0}};
static uint8_t s_rot = 0;
static uint8_t s_flip = 0;



void updateProcShapes(GContext* _ctx) {
  renderFloor(_ctx, 0);
  renderClutter(_ctx);

  renderFloorPuzzleShape(_ctx, GPoint(3*SIZE, 8*SIZE), s_inner[s_win], s_outer[s_win], 0, 0);

  renderFloorPuzzleShape(_ctx, GPoint(9*SIZE, 2*SIZE),  s_inner[0], s_outer[0], s_rot, s_flip);
  renderFloorPuzzleShape(_ctx, GPoint(9*SIZE, 8*SIZE),  s_inner[1], s_outer[1], s_rot, s_flip);
  renderFloorPuzzleShape(_ctx, GPoint(9*SIZE, 14*SIZE), s_inner[2], s_outer[2], s_rot, s_flip);
  
  renderPlayer(_ctx);
  renderWalls(_ctx, true, true, true, true);
  renderWallClutter(_ctx);
  renderArrows(_ctx, 11, 2, 6);

}

bool tickShapes(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    s_win = rand() % 3;
    m_player.m_position = GPoint(0, SIZE*9);
    addCluter(4, 0, 20); // Only left

    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 4; ++j) {
        s_outer[i][j] = rand() % 3;
        s_inner[i][j] = rand() % 3;
      }
    }
    s_flip = m_dungeon.m_difficulty > 0 ? rand() % 3 : 0;
    s_rot = m_dungeon.m_difficulty > 1 ? rand() % 4 : 0;
  }

  if (s_state == 0) { // start initial move
    m_player.m_target = GPoint(SIZE*5,SIZE*9);
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


#endif