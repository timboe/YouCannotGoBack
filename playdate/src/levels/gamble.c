#include "gamble.h"

static uint16_t s_state = 0;
static uint16_t s_spin = 0;
static float s_angle = 0; 
static float s_v = 0;
static float s_slowdown = 0;

void updateProcGamble(PlaydateAPI* _pd) {
  renderFloor(_pd, 0);
  renderClutter(_pd);
  renderPlayer(_pd);

  drawBitmapAbsRot(_pd, m_spin[s_spin], 79, 73, s_angle);
  drawBitmapAbs(_pd, m_wheel, 38, 29);

  if (s_state < 10) renderPlayer(_pd); // In front

  renderWalls(_pd, true, true, false, true);
  renderWallClutter(_pd);
  renderArrows2(_pd, 15, 5, 4, 1, 0, 0, false);
  renderArrows2(_pd, 9, 15, 0, 0, 0, 1, false);
}

bool tickGamble(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position_x = 0;
    m_player.m_position_y = SIZE*9;
    addCluter(4, 0, 20); // Only left
    s_v = 50.0f;
    //s_angle = rand() % 365;
    s_slowdown = 0.5f + (0.01f * (rand()%100)); 
    return false; 
  }

  if (s_state == 0) { // start initial move
   enterRoom(&s_state);
  } else if (s_state == 1) {
   setGameState(kAwaitInput);
   ++s_state;
  } else if (s_state == 2) {
    if (getPlayerChoice() == 0) { 
      s_state = 10; // leave  
    } else {
      m_player.m_target_x = SIZE*3;
      m_player.m_target_y = SIZE*15;
      setGameState(kMovePlayer);
      s_state = 3;
    }
  } else if (s_state == 3) {
    m_player.m_target_x = SIZE*9;
    m_player.m_target_y = SIZE*15;
    setGameState(kMovePlayer);
    s_state = 4;
  } else if (s_state == 4) {
    s_angle += s_v;
    if (s_angle > 365.0f) s_angle -= 365.0f;
    s_v *= 0.97f;
    s_v -= s_slowdown; //0.5=ff, 0.6 0.7=c 0.8 0.9=d 1.0 1.1=w 1.2=ff
    if (s_v <= 0) {
      s_state = 5;
    }

  } else if (s_state == 10) {
    moveToExit(&s_state);
  } else if (s_state == 11) {
   setGameState(kFadeOut);
  }


  return true;
}
