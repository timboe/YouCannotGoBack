#include "maths.h"

typedef enum {
  kAdd,
  kNMathsPuzzles
} MathsPuzzle_t;

static uint16_t s_state = 0;

static MathsPuzzle_t s_puzzle;
static uint16_t s_sequence[4] = {0};

static int8_t s_choices[3] = {0};
static int8_t s_correct = 0;

void updateProcMaths(GContext* _ctx) {

  renderFloor(_ctx, 0);

  drawBitmap(_ctx, m_block, 5, 9);
  drawBitmap(_ctx, m_block, 13, 9);

  for (int _s = 0; _s < 3; ++_s) {
    renderHintNumber(_ctx, GRect((7 + _s*2)*SIZE, 9*SIZE, 16, 16), s_sequence[_s], false);   // sequence
  }

  renderPlayer(_ctx);
  renderWalls(_ctx, true, true, true, true);
  renderClutter(_ctx);

  for (int _s = 0; _s < 3; ++_s) {
    GRect _b = GRect(16*SIZE - 2, (5 + (_s*4))*SIZE - 2, 2*SIZE + 4, 2*SIZE + 4);   // Choices
    renderFrame(_ctx, _b);
    renderHintNumber(_ctx, GRect(16*SIZE - 1, (5 + (_s*4))*SIZE - 1, 16, 16), s_choices[_s], false);
  }

  if (getGameState() == kAwaitInput && getFrameCount() < ANIM_FPS/2) {
    drawBitmap(_ctx, m_arrow, 14, 4);
    drawBitmap(_ctx, m_arrow, 14, 8);
    drawBitmap(_ctx, m_arrow, 14, 12);
  }

}

bool tickMaths(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position = GPoint(0, SIZE*9);
    addCluter(5, 20, 1);

    s_puzzle = rand() % kNMathsPuzzles; // Choose seq
    s_sequence[0] = 30 + rand()%50; // Choose starting
    uint16_t _mod = 5 + rand()%5; // Chose modifier
    for (int _i = 1; _i < 4; ++_i) {
      switch (s_puzzle) {
        case kAdd: s_sequence[_i] = s_sequence[_i - 1] + _mod; break; // Do seq
        default: break;
      }
    }

    s_choices[ 0 ] = -1;
    s_choices[ 1 ] = -1;
    s_choices[ 2 ] = -1;

    s_correct = rand() % 3;
    s_choices[ s_correct ] = s_sequence[3];

    shuffler(s_choices, s_sequence[3] - 10, 20);

    return false;
  }

  if (s_state == 0) {
    enterRoom(&s_state);
  } else if (s_state == 1) { //get input
    setGameState(kAwaitInput);
    ++s_state;
  } else if (s_state == 2) { // check answer and move to chosen door
    if (getPlayerChoice() != s_correct) {
      if (m_dungeon.m_lives > 0) --m_dungeon.m_lives;
      else m_dungeon.m_rooms[ m_dungeon.m_level ][ m_dungeon.m_room + 1 ] = kDeath;
    }
    switch (getPlayerChoice()) {
      case 0: m_player.m_target = GPoint(SIZE*16, SIZE*5); break;
      case 1: m_player.m_target = GPoint(SIZE*13, SIZE*11); break; // further down
      case 2: m_player.m_target = GPoint(SIZE*16, SIZE*13); break;
    }
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 3) {
    m_player.m_target = GPoint(SIZE*16, SIZE*9);
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 4) {
    setGameState(kFadeOut);
  }

  return false;
}
