#include "final.h"

static uint16_t s_state = 0;
static GPoint s_fire[2];

void updateProcFinal(GContext* _ctx) {

  renderFloor(_ctx, 0);


  for (int _i = 0; _i < 3; ++_i) {
    drawBitmap(_ctx, m_flagstone[_i], 5, 5 + (_i * 4));
  }


  renderFinalPit(_ctx);

  if (s_state > 5) {
    drawBitmap(_ctx, m_innerCorner[3], 7, 6);
    drawBitmap(_ctx, m_innerCorner[0], 7, 12);
    drawBitmap(_ctx, m_innerWall[3], 9, 6);
    drawBitmap(_ctx, m_innerWall[2], 9, 12);
    drawBitmap(_ctx, getFloor(), 7, 8);
    drawBitmap(_ctx, getFloor(), 7, 10);
    drawBitmap(_ctx, getFloor(), 9, 8);
    drawBitmap(_ctx, getFloor(), 9, 10);
  } else {
    for (int _i = 0; _i < 4; ++_i) rand(); // keep the wall the same
  }

  drawBitmap(_ctx, m_treasure[0], 11, 7);
  drawBitmap(_ctx, m_treasure[2], 14, 8);
  drawBitmap(_ctx, m_chest, 12, 9);


  renderPlayer(_ctx);
  renderWalls(_ctx, true, false, true, false);

  drawBitmap(_ctx, m_treasure[2], 11, 11);
  drawBitmap(_ctx, m_treasure[1], 13, 10);

  drawBitmapAbs(_ctx, m_fire[0], s_fire[0]);
  drawBitmapAbs(_ctx, m_fire[1], GPoint(s_fire[0].x + SIZE, s_fire[0].y - SIZE));
  drawBitmapAbs(_ctx, m_fire[0], s_fire[1]);
  drawBitmapAbs(_ctx, m_fire[1], GPoint(s_fire[1].x + SIZE, s_fire[1].y - SIZE));

  if (getGameState() == kAwaitInput && getFrameCount() < ANIM_FPS/2) {
    drawBitmap(_ctx, m_arrow, 5, 4);
    drawBitmap(_ctx, m_arrow, 5, 8);
    drawBitmap(_ctx, m_arrow, 5, 12);
  }

}

bool tickFinal(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position = GPoint(0, SIZE*9);
    s_fire[0] = GPoint(1*SIZE, 2*SIZE);
    s_fire[1] = GPoint(1*SIZE, 16*SIZE);
    return false;
  }

  static const char _msgA[] = "*CLICK* *CLICK*";
  static const char _msgB[] = "TREASURE!";

  if (s_state == 0) { // start initial move
    enterRoom(&s_state);
  } else if (s_state == 1) { // initial move is done
    setGameState(kAwaitInput);
    ++s_state;
  } else if (s_state == 2) {
    switch (getPlayerChoice()) {
      case 0: m_player.m_target = GPoint(SIZE*5, SIZE*5); break;
      case 1: m_player.m_target = GPoint(SIZE*5, SIZE*9); break;
      case 2: m_player.m_target = GPoint(SIZE*5, SIZE*13); break;
    }
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 3) {
    setDisplayMsg(_msgA);
    setGameState(kDisplayMsg);
    ++s_state;
  } else if (s_state == 4) {
    if (getPlayerChoice() != m_dungeon.m_finalPuzzle) s_state = 5; // Doom
    else s_state = 6; // win
  } else if (s_state == 5) {
    // LOOSE move fires
    for (int _f = 0; _f < 2; ++_f) {
      bool _move = false;
      if (s_fire[_f].x < m_player.m_position.x - SIZE) {
        ++s_fire[_f].x;
        _move = true;
      }
      if (s_fire[_f].y < m_player.m_position.y) {
        ++s_fire[_f].y;
        _move = true;
      } else if (s_fire[_f].y > m_player.m_position.y) {
        --s_fire[_f].y;
        _move = true;
      }
      if (_move == false) {
        m_dungeon.m_gameOver = 1;
        setGameState(kFadeOut);
      }
    }
    return true; //redraw
  } else if (s_state == 6) {
    m_player.m_target = GPoint(SIZE*12, SIZE*9);
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 7) {
    setDisplayMsg(_msgB);
    setGameState(kDisplayMsg);
    ++s_state;
  } else if (s_state == 8) {
    m_player.m_target.x = SIZE*15;
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 9) {
    m_dungeon.m_gameOver = 2; // Woo hoo!
    setGameState(kFadeOut);
  }

  return false;
}
