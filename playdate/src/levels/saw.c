#include <math.h>
#include "saw.h"
#include "../sound.h"

static uint16_t s_state = 0; // game state
static int8_t s_offset = 0; // moving backdrop
static float s_rotation = 0; // rotate status
static int8_t s_type = 0; // which blades
static int16_t s_position = 0; //y axis
static int8_t s_count = 0; // how many blades dodged

#define PD_OFFSET SIZE*4

void updateProcSaw(PlaydateAPI* _pd) {

  renderSawFloor(_pd, s_offset);
  renderPlayer(_pd);
  if (s_type == 1) {
     drawBitmapAbsRot(_pd, m_sawA, s_position + PD_OFFSET, 5*SIZE, s_rotation);
  } else if (s_type == 3) {
     drawBitmapAbsRot(_pd, m_sawB, s_position + PD_OFFSET, 13*SIZE, -s_rotation);
  } else if (s_type == 2) {
     drawBitmapAbsRot(_pd, m_sawA, s_position + PD_OFFSET, 3*SIZE, s_rotation);
     drawBitmapAbsRot(_pd, m_sawB, s_position + PD_OFFSET, 15*SIZE, -s_rotation);
  }
  renderSawWalls(_pd, s_offset);
  renderArrows(_pd, 3, 5, 2);
}

bool tickSaw(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position_x = -2*SIZE;
    m_player.m_position_y = SIZE*8;
    s_offset = 0; // moving backdrop
    s_rotation = 0; // rotate status
    s_type = 0; // which blades
    s_position = 0; //y axis
    s_count = 0;
    return false;
  }

  if (s_state == 0) { // start initial move
     m_player.m_target_x = SIZE*4;
     m_player.m_target_y = SIZE*8;
     setGameState(kMovePlayer);
     sawSound(true);
     sawVolume(0.0f);
     ++s_state;
  } else if (s_state == 1) {
    setGameState(kLevelSpecificWButtons);
    s_rotation += 5.0f;
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

      if (abs( s_position+40 - m_player.m_position_x ) < 30) {
        if (s_type == 1 && m_player.m_position_y < 9*SIZE) s_state = 2;
        else if (s_type == 2 && (m_player.m_position_y < 7*SIZE || m_player.m_position_y > 9*SIZE)) s_state = 2;
        else if (s_type == 3 && m_player.m_position_y > 7*SIZE ) s_state = 2;
      }
    } else {
      setGameState(kLevelSpecific);
      s_type = 0;
      m_player.m_position_x += (PLAYER_SPEED * 2);
      if (m_player.m_position_x > 20*SIZE) {
        setGameState(kFadeOut);
        sawSound(false);
      }
    }


    #define SAW_SOUND_DISTANCE 128.0f 
    sawVolume( 1.0f - (abs(s_position - m_player.m_position_x) / SAW_SOUND_DISTANCE) );
  } else if (s_state == 2) {
    m_dungeon.m_gameOver = 1;
    //vibes_long_pulse();
    setGameState(kFadeOut);
  }

  return true;
}
