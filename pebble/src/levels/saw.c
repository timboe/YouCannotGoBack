#include "saw.h"

static uint16_t s_state = 0; // game state
static int8_t s_offset = 0; // moving backdrop
//static int8_t s_rotation = 0; // rotate status
static int8_t s_type[10 + MAX_LEVELS] = {0}; // which blades
static int16_t s_position = 0; //y axis
static int8_t s_count = 0; // how many blades dodged


void updateProcSaw(GContext* _ctx) {

  renderSawFloor(_ctx, s_offset);
  renderPlayer(_ctx);

  if (s_count < 4 + m_dungeon.m_difficulty) {
    if (s_type[s_count] == 1) {
      drawBitmapAbs(_ctx, m_sawB, GPoint(s_position, 5*SIZE));
    } else if (s_type[s_count] == 3) {
      drawBitmapAbs(_ctx, m_sawA, GPoint(s_position, 8*SIZE));
    } else if (s_type[s_count] == 2) {
      drawBitmapAbs(_ctx, m_sawB, GPoint(s_position, 3*SIZE));
      drawBitmapAbs(_ctx, m_sawA, GPoint(s_position, 10*SIZE));
    }
  }
  renderSawWalls(_ctx, s_offset);
  #ifdef YCGBv2
    renderWarning(_ctx);
  #endif
  renderArrows(_ctx, 0, 5, 2);
}

bool tickSaw(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position = GPoint(-2*SIZE, SIZE*8);
    s_offset = 0; // moving backdrop
    //s_rotation = 0; // rotate status
    for (int _i = 0; _i < (10 + MAX_LEVELS); ++_i) {
      s_type[_i] = (rand() % 3) + 1;
      // Non-guarenteed way of trying to vary blades
      if (_i && s_type[_i] == s_type[_i-1]) s_type[_i] = (rand() % 3) + 1;
    }
    s_position = PBL_DISPLAY_WIDTH; //y axis
    s_count = 0;
    return false;
  }

  if (s_state == 0) { // start initial move
     m_player.m_target = GPoint(SIZE*4, SIZE*8);
     setGameState(kMovePlayer);
     ++s_state;
  } else if (s_state == 1) {
    setGameState(kLevelSpecificWButtons);

    #ifdef SS_MODE
      static uint16_t _c = 0;
      if (++_c > 20 && _c < 120) {
        return true;
      }
    #endif

    //if (getFrameCount() % 2 == 0 && ++s_rotation == 2) s_rotation = 0;
    if (getFrameCount() % 3 == 0 && ++m_player.m_playerFrame == MAX_FRAMES) m_player.m_playerFrame = 0;
    if (++s_offset == (2*SIZE)) s_offset = 0;

    m_player.m_target = GPoint(4*SIZE, (6 + (2*getPlayerChoice())) * SIZE );
    for (uint8_t _s = 0; _s < PLAYER_SPEED; ++_s) {
      if      (m_player.m_target.y > m_player.m_position.y) ++m_player.m_position.y;
      else if (m_player.m_target.y < m_player.m_position.y) --m_player.m_position.y;
    }

    if (s_count < 4 + m_dungeon.m_difficulty) {
      uint8_t _diff = MIN(4, m_dungeon.m_difficulty);
      #ifdef HIGH_RES
        s_position -= ((3 + _diff) * 3) / 2;
      #else
        s_position -= 3 + _diff;
      #endif
      if (s_position < -80) {
        s_position = PBL_DISPLAY_WIDTH;
        ++s_count;
      }

      // top is at 6, middle is 8 bottom is at 10

      if (abs( s_position+40 - m_player.m_position.x ) < 30) {
        if (s_type[s_count] == 1 && m_player.m_position.y < 9*SIZE) s_state = 2;
        else if (s_type[s_count] == 2 && (m_player.m_position.y < 7*SIZE || m_player.m_position.y > 9*SIZE)) s_state = 2;
        else if (s_type[s_count] == 3 && m_player.m_position.y > 7*SIZE ) s_state = 2;
      }
    } else {
      setGameState(kLevelSpecific);
      m_player.m_position.x += (PLAYER_SPEED * 2);
      if (m_player.m_position.x > 20*SIZE) setGameState(kFadeOut);
    }
  } else if (s_state == 2) {
    #ifdef SS_MODE
      setGameState(kFadeOut);
      return true;
    #endif
    m_dungeon.m_gameOver = 1;
    vibes_long_pulse();
    setGameState(kFadeOut);
  }

  return true;
}
