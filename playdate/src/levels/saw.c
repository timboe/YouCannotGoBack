#include <math.h>
#include "saw.h"
#include "../sound.h"

static uint16_t s_state = 0; // game state
static int8_t s_offset = 0; // moving backdrop
static float s_rotation = 0; // rotate status
static int8_t s_type[10 + MAX_LEVELS] = {0}; // which blades
static int16_t s_position = 0; //y axis
static int8_t s_count = 0; // how many blades dodged

#define PD_OFFSET SIZE*4

void updateProcSaw(PlaydateAPI* _pd) {

  renderSawFloor(_pd, s_offset);
  renderPlayer(_pd);
  if (s_count < 4 + m_dungeon.m_difficulty) {
    if (s_type[s_count] == 1) {
      drawBitmapAbsRot(_pd, m_sawA, s_position + PD_OFFSET, 5*SIZE, s_rotation);
    } else if (s_type[s_count] == 3) {
      drawBitmapAbsRot(_pd, m_sawB, s_position + PD_OFFSET, 13*SIZE, -s_rotation);
    } else if (s_type[s_count] == 2) {
      drawBitmapAbsRot(_pd, m_sawA, s_position + PD_OFFSET, 3*SIZE, s_rotation);
      drawBitmapAbsRot(_pd, m_sawB, s_position + PD_OFFSET, 15*SIZE, -s_rotation);
    }
  }
  renderSawWalls(_pd, s_offset);
  renderArrows(_pd, 6, 6, 2);
}

bool tickSaw(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position_x = -2*SIZE;
    m_player.m_position_y = SIZE*8;
    s_offset = 0; // moving backdrop
    s_rotation = 0; // rotate status
    for (int _i = 0; _i < (10 + MAX_LEVELS); ++_i) {
      s_type[_i] = (rand() % 3) + 1;
      // Non-guarenteed way of trying to vary blades
      if (_i && s_type[_i] == s_type[_i-1]) s_type[_i] = (rand() % 3) + 1;
    }
    s_position = 144;
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
    s_rotation += 5.0f + (m_dungeon.m_difficulty * 2.0f);
    if (getFrameCount() % 3 == 0) {
      if (++m_player.m_playerFrame == MAX_FRAMES) {
        m_player.m_playerFrame = 0;
      }
      if (m_player.m_playerFrame % 2 == 0) footSound();
    }
    if (++s_offset == 16) s_offset = 0;

    m_player.m_target_x = 4*SIZE;
    m_player.m_target_y = (6 + (2*getPlayerChoice())) * SIZE;
    if      (m_player.m_target_y > m_player.m_position_y) m_player.m_position_y += PLAYER_SPEED;
    else if (m_player.m_target_y < m_player.m_position_y) m_player.m_position_y -= PLAYER_SPEED;

    if (s_count < 4 + m_dungeon.m_difficulty) {
      s_position -= 3 + m_dungeon.m_difficulty;
      if (s_position < -80) {
        s_position = 144;
        ++s_count;
      }

      if (abs( s_position+30 - m_player.m_position_x ) < 30) {
        if (s_type[s_count] == 1 && m_player.m_position_y < 9*SIZE) s_state = 2;
        else if (s_type[s_count] == 2 && (m_player.m_position_y < 7*SIZE || m_player.m_position_y > 9*SIZE)) s_state = 2;
        else if (s_type[s_count] == 3 && m_player.m_position_y > 7*SIZE ) s_state = 2;
      }
    } else {
      setGameState(kLevelSpecific);
      sawSound(false);
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
    boomSound();
    setGameState(kFadeOut);
    sawSound(false);
  }

  return true;
}
