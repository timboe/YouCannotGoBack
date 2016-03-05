#include "stones.h"

static uint16_t s_state = 0;
static int8_t s_correct[3] = {0};
static int8_t s_input[3] = {0};
static int8_t s_coloursA[3] = {0};
static int8_t s_coloursB[3] = {0};
static int8_t s_coloursC[3] = {0};

void updateProcStones(GContext* _ctx) {

  renderPit(_ctx);

  renderLinePath(_ctx, 4, 10,   7, 6); // to upper
  renderLinePath(_ctx, 4, 10,   15, 10); // straight accross
  renderLinePath(_ctx, 4, 10,   7, 14); // to lower

  renderLinePath(_ctx, 7, 6,    15, 6); //upper accross
  renderLinePath(_ctx, 7, 14,   15, 14); // lower accross

  renderLinePath(_ctx, 11, 6,    11, 14); // middle down

  renderLinePath(_ctx, 7, 6,    15, 14); // cross \.
  renderLinePath(_ctx, 7, 14,   15, 6); // cross /

  renderLinePath(_ctx, 7, 10,   11, 6);  //diamond
  renderLinePath(_ctx, 7, 10,   11, 14);

  renderLinePath(_ctx, 11, 6,   15, 10);
  renderLinePath(_ctx, 11, 14,  15, 10);

  renderLinePath(_ctx, 7, 6,    11, 14);
  renderLinePath(_ctx, 7, 14,   11, 6);

  renderLinePath(_ctx, 11, 6,   15, 14);
  renderLinePath(_ctx, 11, 14,  15, 6);

  renderStandingStone(_ctx, 4, 10, GColorLightGray);

  renderStandingStone(_ctx, 7, 6, getShieldColor(s_coloursA[0])); // Top row
  renderStandingStone(_ctx, 7, 10, getShieldColor(s_coloursA[1]));
  renderStandingStone(_ctx, 7, 14, getShieldColor(s_coloursA[2]));

  renderStandingStone(_ctx, 11, 6, getShieldColor(s_coloursB[0])); // Middle row
  renderStandingStone(_ctx, 11, 10, getShieldColor(s_coloursB[1]));
  renderStandingStone(_ctx, 11, 14, getShieldColor(s_coloursB[2]));

  renderStandingStone(_ctx, 15, 6, getShieldColor(s_coloursC[0])); // Bottom row
  renderStandingStone(_ctx, 15, 10, getShieldColor(s_coloursC[1]));
  renderStandingStone(_ctx, 15, 14, getShieldColor(s_coloursC[2]));

  renderPlayer(_ctx);
  renderWalls(_ctx, true, true, true, true);
  //

  if (s_state >= 4) {
    for (int _i = 0; _i < 3; ++_i) {
      if (s_correct[0] == _i) continue;
      drawBitmap(_ctx, m_fire, 6, 5 + (4*_i));
    }
  }
  if (s_state >= 6) {
    for (int _i = 0; _i < 3; ++_i) {
      if (s_correct[1] == _i) continue;
      drawBitmap(_ctx, m_fire, 10, 5 + (4*_i));
    }
  }
  if (s_state >= 8) {
    for (int _i = 0; _i < 3; ++_i) {
      if (s_correct[2] == _i) continue;
      drawBitmap(_ctx, m_fire, 14, 5 + (4*_i));
    }
  }

  if (getGameState() == kAwaitInput && getFrameCount() < ANIM_FPS/2) {
    int _off = 0;
    if (s_state == 4) _off = 4;
    else if (s_state == 6) _off = 8;
    drawBitmap(_ctx, m_arrow, 6 + _off, 3);
    drawBitmap(_ctx, m_arrow, 6 + _off, 7);
    drawBitmap(_ctx, m_arrow, 6 + _off, 11);
  }

}

bool tickStones(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position = GPoint(0, SIZE*9);

    s_correct[0] = randomiseChoices(s_coloursA, 0);
    s_correct[1] = randomiseChoices(s_coloursB, 1);
    s_correct[2] = randomiseChoices(s_coloursC, 2);
    return false;
  }

  if (s_state == 0) { // start initial move
    enterRoom(&s_state);
  } else if (s_state == 1) { // initial move is done
    setGameState(kAwaitInput);
    ++s_state;
  } else if (s_state == 2) { // first choice

    switch (getPlayerChoice()) {
      case 0: m_player.m_target = GPoint(SIZE*6, SIZE*5); break;
      case 1: m_player.m_target = GPoint(SIZE*6, SIZE*9); break;
      case 2: m_player.m_target = GPoint(SIZE*6, SIZE*13); break;
    }
    setGameState(kMovePlayer);
    ++s_state;

  } else if (s_state == 3) {

    if (getPlayerChoice() != s_correct[0]) {
      m_dungeon.m_gameOver = 1;
      setGameState(kFadeOut);
    } else {
      setGameState(kAwaitInput);
    }
    ++s_state; // On 4, draw first fires

  } else if (s_state == 4) { // first choice

    switch (getPlayerChoice()) {
      case 0: m_player.m_target = GPoint(SIZE*10, SIZE*5); break;
      case 1: m_player.m_target = GPoint(SIZE*10, SIZE*9); break;
      case 2: m_player.m_target = GPoint(SIZE*10, SIZE*13); break;
    }
    setGameState(kMovePlayer);
    ++s_state;

  } else if (s_state == 5) {

    if (getPlayerChoice() != s_correct[1]) {
      m_dungeon.m_gameOver = 1;
      setGameState(kFadeOut);
    } else {
      setGameState(kAwaitInput);
    }
    ++s_state; // On 6, draw second fires

  } else if (s_state == 6) { // first choice

    switch (getPlayerChoice()) {
      case 0: m_player.m_target = GPoint(SIZE*14, SIZE*5); break;
      case 1: m_player.m_target = GPoint(SIZE*14, SIZE*9); break;
      case 2: m_player.m_target = GPoint(SIZE*14, SIZE*13); break;
    }
    setGameState(kMovePlayer);
    ++s_state;

  } else if (s_state == 7) {

     if (getPlayerChoice() != s_correct[2]) {
       m_dungeon.m_gameOver = 1;
       setGameState(kFadeOut);
     } else {
       m_player.m_target.x = SIZE*17;
       setGameState(kMovePlayer);
     }
     ++s_state; // On 8, draw final fires

  } if (s_state == 8) {

    setGameState(kFadeOut);

  }

  return false;
}
