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

void updateProcMaths(PlaydateAPI* _pd) {

  renderFloor(_pd, 0);

  drawBitmap(_pd, m_block, 5, 9);
  drawBitmap(_pd, m_block, 13, 9);

  for (int _s = 0; _s < 3; ++_s) {
    PDRect _b = {.x = (7 + _s*2)*SIZE, .y = 9*SIZE, .width = 16, .height = 16};
    renderHintNumber(_pd, _b, s_sequence[_s], false);   // sequence
  }

  renderClutter(_pd);
  renderPlayer(_pd);
  renderWalls(_pd, true, true, true, true);
  renderWallClutter(_pd);

  for (int _s = 0; _s < 3; ++_s) {
    PDRect _b = {.x = 16*SIZE - 2, .y = (5 + (_s*4))*SIZE - 2, .width = 2*SIZE + 4, .height = 2*SIZE + 4};   // Choices
    renderFrame(_pd, _b);
    PDRect _b2 = {.x = 16*SIZE - 1, .y = (5 + (_s*4))*SIZE - 1, .width = 16, .height = 16};
    renderHintNumber(_pd, _b2, s_choices[_s], false);
  }
  renderArrows(_pd, 14, 4, 4);

}

bool tickMaths(PlaydateAPI* _pd, bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position_x = 0;
    m_player.m_position_y = SIZE;
    addCluter(5, 20, 1);

    s_puzzle = rand() % kNMathsPuzzles; // Choose seq
    s_sequence[0] = (10 * m_dungeon.m_level) + rand()%10; // Choose starting
    uint16_t _modA = 1 + rand()%5; // Chose modifier
    uint16_t _modB = 0;
    if (m_dungeon.m_level > 0) _modB = 1 + rand()%4;
    for (int _i = 1; _i < 4; ++_i) {
      switch (s_puzzle) {
        case kAdd: s_sequence[_i] = s_sequence[_i - 1] + _modA + (_i * _modB); break; // Do seq
        default: break;
      }
    }

    s_choices[ 0 ] = -1;
    s_choices[ 1 ] = -1;
    s_choices[ 2 ] = -1;

    s_correct = rand() % 3;
    s_choices[ s_correct ] = s_sequence[3];

    shuffler(_pd, s_choices, s_sequence[3] - 10, 20);

    return false;
  }

  if (s_state == 0) {
    enterRoom(&s_state);
  } else if (s_state == 1) {
    setGameState(kAwaitInput);
    ++s_state;
  } else if (s_state == 2) { // check answer and move to chosen door
    if (getPlayerChoice() != s_correct) {
      if (m_dungeon.m_lives > 0) --m_dungeon.m_lives;
      else m_dungeon.m_rooms[ m_dungeon.m_level ][ m_dungeon.m_room + 1 ] = kDeath;
    }
    switch (getPlayerChoice()) {
      case 0: m_player.m_target_x = SIZE*17; m_player.m_target_y = SIZE*5; break;
      case 1: m_player.m_target_x = SIZE*13; m_player.m_target_y = SIZE*11; break;
      case 2: m_player.m_target_x = SIZE*17; m_player.m_target_y = SIZE*13; break;
    }
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 3) {
    if (getPlayerChoice() == 1) {
      m_player.m_target_x = SIZE*17;
      m_player.m_target_y = SIZE*9;
      setGameState(kMovePlayer);
    }
    ++s_state;
  } else if (s_state == 4) {
    setGameState(kFadeOut);
  }

  return false;
}
