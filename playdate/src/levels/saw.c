#include "saw.h"

static uint16_t s_state = 0; // game state
static int8_t s_offset = 0; // moving backdrop
//static int8_t s_rotation = 0; // rotate status
static int8_t s_type = 0; // which blades
static int16_t s_position = 0; //y axis
static int8_t s_count = 0; // how many blades dodged


void updateProcSaw(PlaydateAPI* _pd) {

  renderSawFloor(_pd, s_offset);
  renderPlayer(_pd);
  if (s_type == 1) {
     drawBitmapAbs(_pd, m_saw, s_position, 5*SIZE);
  } else if (s_type == 3) {
     drawBitmapAbs(_pd, m_saw, s_position, 8*SIZE);
  } else if (s_type == 2) {
     drawBitmapAbs(_pd, m_saw, s_position, 3*SIZE);
     drawBitmapAbs(_pd, m_saw, s_position, 10*SIZE);
  }
  renderSawWalls(_pd, s_offset);
  renderArrows(_pd, 0, 5, 2);
}

bool tickSaw(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position_x = -2*SIZE;
    m_player.m_position_y = SIZE*8;
    s_offset = 0; // moving backdrop
    //s_rotation = 0; // rotate status
    s_type = 0; // which blades
    s_position = 0; //y axis
    s_count = 0;
    return false;
  }

  if (s_state == 0) { // start initial move
     m_player.m_target_x = SIZE*4;
     m_player.m_target_y = SIZE*8;
     setGameState(kMovePlayer);
     ++s_state;
  } else if (s_state == 1) {
    setGameState(kLevelSpecificWButtons);
    //if (getFrameCount() % 2 == 0 && ++s_rotation == 2) s_rotation = 0;
    if (getFrameCount() % 3 == 0 && ++m_player.m_playerFrame == MAX_FRAMES) m_player.m_playerFrame = 0;
    if (++s_offset == 16) s_offset = 0;

    m_player.m_target_x = 4*SIZE;
    m_player.m_target_y = (6 + (2*getPlayerChoice())) * SIZE;
    if      (m_player.m_target_y > m_player.m_position_y) m_player.m_position_y += PLAYER_SPEED;
    else if (m_player.m_target_y < m_player.m_position_y) m_player.m_position_y -= PLAYER_SPEED;

    if (s_type == 0 && s_count < 5 + m_dungeon.m_level) {
      ++s_count;
      srand( m_dungeon.m_seed + s_count );
      s_type = 1 + (rand()%3);
      s_position = 144; //TODO round offset
    } else if (s_count < 5 + m_dungeon.m_level) {
      s_position -= 3 + m_dungeon.m_level;
      if (s_position < -80) s_type = 0;

      // top is at 6, middle is 8 bottom is at 10

      if (abs( s_position+40 - m_player.m_position_x ) < 30) {
        if (s_type == 1 && m_player.m_position_y < 9*SIZE) s_state = 2;
        else if (s_type == 2 && (m_player.m_position_y < 7*SIZE || m_player.m_position_y > 9*SIZE)) s_state = 2;
        else if (s_type == 3 && m_player.m_position_y > 7*SIZE ) s_state = 2;
      }
    } else {
      setGameState(kLevelSpecific);
      m_player.m_position_x += (PLAYER_SPEED * 2);
      if (m_player.m_position_x > 20*SIZE) setGameState(kFadeOut);
    }
  } else if (s_state == 2) {
    m_dungeon.m_gameOver = 1;
    //vibes_long_pulse();
    setGameState(kFadeOut);
  }

  return true;
}
