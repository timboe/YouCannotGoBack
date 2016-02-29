#include "bridge.h"

static uint16_t s_state = 0;

void updateProcBridge(GContext* _ctx) {

  renderFloor(_ctx, 1);

  int _breakA = 7+rand()%3, _breakB = 12-rand()%3;
  for (int _i = 0; _i < 3; ++_i) {
    drawBitmap(_ctx, m_bridge[0], 4,  5 + (_i * 4));
    drawBitmap(_ctx, m_bridge[2], 13, 5 + (_i * 4));

    for (int _x = 5; _x < 13; ++_x) {
      if (s_state > 4 && _x > _breakA && _x < _breakB) continue;
      drawBitmap(_ctx, m_bridge[1], _x, 5 + (_i * 4));
    }
  }

  renderPlayer(_ctx);
  renderWalls(_ctx, true, true, true, true);

  renderClutter(_ctx);

  if (getGameState() == kAwaitInput && getFrameCount() < ANIM_FPS/2) {
    drawBitmap(_ctx, m_arrow, 8, 4);
    drawBitmap(_ctx, m_arrow, 8, 8);
    drawBitmap(_ctx, m_arrow, 8, 12);
  }

}

bool tickBridge(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position = GPoint(0, SIZE*9);
    addCluter(1, 6, 5, 13);
    return false;
  }

  if (s_state == 0) { // start initial move
    enterRoom(&s_state);
  } else if (s_state == 1) { // initial move is done
     setGameState(kAwaitInput);
     ++s_state;
  } else if (s_state == 2) {
    switch (getPlayerChoice()) {
      case 0: m_player.m_target = GPoint(SIZE*3, SIZE*5); break;
      case 1: m_player.m_target = GPoint(SIZE*3, SIZE*9); break;
      case 2: m_player.m_target = GPoint(SIZE*3, SIZE*13); break;
    }
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 3) {
    switch (getPlayerChoice()) {
      case 0: m_player.m_target = GPoint(SIZE*8, SIZE*5); break;
      case 1: m_player.m_target = GPoint(SIZE*8, SIZE*9); break;
      case 2: m_player.m_target = GPoint(SIZE*8, SIZE*13); break;
    }
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 4) { // LOOSE
   setGameState(kFadeOut);
   setGameOver(true);
   m_dungeon.m_fallingDeath = true;
   ++s_state;
 }
   // move onto stairs
  //   m_player.m_target = GPoint(SIZE*9, SIZE*9);
  //   setGameState(kMovePlayer);
  //   ++s_state;
  // } else if (s_state == 3) { // move down stairs
  //   m_player.m_target = GPoint(SIZE*11, SIZE*11);
  //   setGameState(kMovePlayer);
  //   ++s_state;
  // } else if (s_state == 4) {
  //   setGameState(kFadeOut);
  // }

  return false;
}
