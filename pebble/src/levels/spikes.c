#include "spikes.h"
#ifdef YCGBv2

static uint16_t s_state = 0;

static int8_t s_off[3] = {0};
static int8_t s_scale[3] = {0};

static uint8_t s_countdown[3] = {0};
static uint8_t s_ctotal = 0;


void updateProcSpikes(GContext* _ctx) {

  renderWalls(_ctx, true, false, true, false);


  renderFloor(_ctx, 2 /*mode spikes*/, 2, 4); // 2 - 18
  renderSpikes(_ctx, s_off, 0);

  renderFloor(_ctx, 2 /*mode spikes*/, 4, 8); // 2 - 18
  renderSpikeHoleBottom(_ctx, 0);
  renderSpikes(_ctx, s_off, 1);


  // renderClutter(_pd);
  // if (s_state < 9) {
  //   renderWalls(_ctx, true, false, true, false);
  //   renderWallClutter(_ctx);
  // }

  renderFloor(_ctx, 2 /*mode spikes*/, 8, 12); // 2 - 18
  renderSpikeHoleBottom(_ctx, 1);

  renderPlayer(_ctx);
  renderSpikes(_ctx, s_off, 2);

  renderFloor(_ctx, 2 /*mode spikes*/, 12, 16); // 2 - 18
  renderSpikeHoleBottom(_ctx, 2);
  renderSpikes(_ctx, s_off, 3);

  renderFloor(_ctx, 2 /*mode spikes*/, 16, 18); // 2 - 18
  renderSpikeHoleBottom(_ctx, 3);
  renderBottomWall(_ctx);
  renderWallClutter(_ctx);

  // Player should go under
  // if (s_state >= 9) {
  //   renderWalls(_ctx, true, false, true, false);
  //   renderWallClutter(_ctx);
  // }

  // if (getFrameCount() < ANIM_FPS/2) {
  //   switch (s_state) {
  //     case 2: drawCBitmap(_pd, &m_arrow_r,  7, 9); break;
  //     case 4: drawCBitmap(_pd, &m_arrow_r, 11, 9); break;
  //     case 6: drawCBitmap(_pd, &m_arrow_r, 15, 9); break;
  //   }
  // }
}

bool tickSpikes(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position = GPoint(0, SIZE*9);
    addCluter(4, 0, 20); // Only left
    s_off[0] = 0;
    s_off[1] = 0;
    s_off[2] = 0;
    s_scale[0] = 0;
    s_scale[1] = 0;
    s_scale[2] = 0;
    switch (m_dungeon.m_difficulty) {
      case 0: s_ctotal = 32; s_countdown[0] = 22; s_countdown[1] = 11; s_countdown[2] = 1; break;
      case 1: s_ctotal = 16; s_countdown[0] = 1; s_countdown[1] = 1; s_countdown[2] = 1; break;
      default: s_ctotal = 1; s_countdown[0] = rand() % 16 + 1; s_countdown[1] = rand() % 16 + 1; s_countdown[2] = rand() % 16 + 1; break;
    }
    return false;
  }

  // Spears
  // for (int _i = 0; _i < 3; ++_i) {
  //   if (s_scale[_i] < 0.01f && (--s_countdown[_i] == 0)) {
  //     s_countdown[_i] = s_ctotal;
  //     s_off[_i] = 1.0f;
  //     s_scale[_i] = 1.2f;
  //   }

  //   s_off[_i] *= s_scale[_i];

  //   if (s_off[_i] > S_OFF) {
  //     s_scale[_i] = 0.5f;
  //     s_off[_i] = 32.0f;
  //   } else if (s_off[_i] < 1.0f && s_scale[_i] < 1.0f) {
  //     s_scale[_i] = 0.0f;
  //     s_off[_i] = 0.0f;
  //   }
  // }

  for (int _i = 0; _i < 3; ++_i) {
    if (s_off[_i] < SIZE*6) ++s_off[_i];
    else s_off[_i] = 0;
  }  


  if (s_state == 0) { // start initial move
    enterRoom(&s_state);
  } else if (s_state == 1) { // initial move is done
    resetPlayerChoice();
    setGameState(kLevelSpecific);
    s_state = 2; 
  } else if (s_state == 10) {
    setGameState(kFadeOut);
  } else {
    setGameState(kLevelSpecificWButtons);
    
    if (s_state % 2 == 0 && getPlayerChoice() == 1) {
      ++s_state;
      resetPlayerChoice();
    }

    // Check for death, up to 48 pixels in each spear
    #define DEATH_PIXELS 16
    for (int _i = 0; _i < 3; ++_i) {
      int _x = abs((SIZE*5) + (SIZE*4*_i) - m_player.m_position.x);
      if (_x < SIZE && s_off[_i] > DEATH_PIXELS) {
        m_dungeon.m_gameOver = 1;
        setGameState(kFadeOut);
       	vibes_long_pulse();
      }
    }

    if (s_state % 2 == 1) {
      if (getFrameCount() % 3 == 0 && ++m_player.m_playerFrame == MAX_FRAMES) m_player.m_playerFrame = 0;
      m_player.m_position.x += PLAYER_SPEED;
      switch (s_state) {
        case 3: if (m_player.m_position.x > SIZE*7  - SIZE/2) ++s_state; break;
        case 5: if (m_player.m_position.x > SIZE*11 - SIZE/2) ++s_state; break;
        case 7: if (m_player.m_position.x > SIZE*14 - SIZE/2) s_state += 2; break; // Don't want to wait for user-input here
        case 9: if (m_player.m_position.x > SIZE*17) ++s_state; break;
      }
    }

  }

  return true;
}

#endif