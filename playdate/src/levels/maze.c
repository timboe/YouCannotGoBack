#include "maze.h"
#include "../sprite.h"

#define N_MAZES 5
#define N_MAZE_STEP 30

static uint16_t s_state = 0;
static uint16_t s_maze = 0;
static uint16_t s_path = 0;
//static uint16_t s_correct = 0;
static const uint16_t s_sollution[N_MAZES][3] = { {2,0,1}, {1,0,2}, {0,2,1}, {2,0,1}, {2,1,0} };

typedef struct {
  uint8_t m_path2[N_MAZE_STEP];
  uint8_t m_path1[N_MAZE_STEP];
  uint8_t m_path0[N_MAZE_STEP];
} Maze_t;

static const Maze_t s_mazes[N_MAZES] = {
  [0].m_path0 = {5,4, 13,4, 13,12, 7,12, 7,14, 15,14},
  [0].m_path1 = {5,10, 6,10, 6,3, 10,3, 10,6, 15,6},
  [0].m_path2 = {5,16, 10,16, 10,8, 7,8, 7,10, 15,10},
  [1].m_path0 = {5,4, 6,4, 6,9, 8,9, 8,10, 10,10, 10,14, 6,14, 6,17, 11,17, 11,11, 14,11, 14,10, 15,10},
  [1].m_path1 = {5,10, 6,10, 6,13, 7,13, 7, 3, 8,3, 8,7, 13,7, 13,6, 15,6},
  [1].m_path2 = {5,16, 8,16, 8,11, 9,11, 9,3, 11,3, 11,10, 13,10, 13,14, 15,14},
  [2].m_path0 = {5,4, 6,4, 6,7, 11,7, 11,16, 13,16, 13,6, 15,6},
  [2].m_path1 = {5,10, 9,10, 9,8, 10,8, 10,9, 6,9, 6,13, 9,13, 9,15, 7,15, 7,17, 14,17, 14,14, 15,14},
  [2].m_path2 = {5,16, 6,16, 6,14, 8,14, 8,6, 12,6, 12,5, 8,5, 8,3, 14,3, 14,10, 15,10},
  [3].m_path0 = {5,4, 6,4, 10,8, 6,12, 12,18, 14,16, 14,14, 15,14},
  [3].m_path1 = {5,10, 6,10, 9,13, 10,12, 13,15, 10,18, 6,14, 14,6, 15,6},
  [3].m_path2 = {5,16, 6,16, 7,17, 12,12, 6,6, 10,2, 13,5, 11,7, 14,10, 15,10},
  [4].m_path0 = {5,4, 6,4, 12,10, 14,8, 12,6, 6,12, 9,15, 8,16, 6,14, 9,11, 12,14, 15,14},
  [4].m_path1 = {5,10, 6,10, 11,15, 10,16, 11,17, 12,16, 13,17, 14,16, 13,15, 15,13, 13,11, 14,10, 15,10},
  [4].m_path2 = {5,16, 6,16, 11,11, 6,6, 9,3, 11,5, 10,6, 12,8, 14,6, 15,6}
};

void drawLine(PlaydateAPI* _pd, int _x1, int _y1, int _x2, int _y2, int _w, LCDColor _c) {
  if (_x1 == 0 || _x2 == 0) return;
  _pd->graphics->drawLine(_x1*SIZE, _y1*SIZE, _x2*SIZE, _y2*SIZE, _w, _c);
}

void lines(PlaydateAPI* _pd, LCDColor _c, int _w) {
  const bool _l = m_player.m_position_x < SIZE*8 || getFlash(true); 
  const bool _p0 = _l || s_path == 0;
  const bool _p1 = _l || s_path == 1;
  const bool _p2 = _l || s_path == 2;
  for (int _p = 0; _p < N_MAZE_STEP - 3; _p += 2) {
    if (_p0) drawLine(_pd, s_mazes[s_maze].m_path0[_p], s_mazes[s_maze].m_path0[_p+1],  s_mazes[s_maze].m_path0[_p+2], s_mazes[s_maze].m_path0[_p+3], _w, _c);
    if (_p1) drawLine(_pd, s_mazes[s_maze].m_path1[_p], s_mazes[s_maze].m_path1[_p+1],  s_mazes[s_maze].m_path1[_p+2], s_mazes[s_maze].m_path1[_p+3], _w, _c);
    if (_p2) drawLine(_pd, s_mazes[s_maze].m_path2[_p], s_mazes[s_maze].m_path2[_p+1],  s_mazes[s_maze].m_path2[_p+2], s_mazes[s_maze].m_path2[_p+3], _w, _c);
  }
}

void updateProcMaze(PlaydateAPI* _pd) {
  renderFloor(_pd, 0);

  renderStandingStone(_pd, 4, 4 + s_path*6, kColorBlack, kCircle);

  LCDColor _c1 = kColorWhite, _c2 = kColorWhite;
  int _w = 4;

  switch (m_dungeon.m_level) {
    case 0: _c1 = kColorWhite; _c2 = kColorBlack; _w = 4; break;
    case 1: _c1 = kColorBlack; _c2 = kColorWhite; _w = 4; break;
    default: _c1 = kColorWhite; _w = 1; break;
  }
  lines(_pd, _c1, _w);

  if (m_dungeon.m_level < 3) {
    lines(_pd, _c2, _w/2);
  } 

  if (m_dungeon.m_level == 2) renderClutter(_pd); // Obscure
  renderPlayer(_pd);
  renderWalls(_pd, true, true, true, true);
  renderWallClutter(_pd);
  renderArrows(_pd, 15, 5, 4);
}

bool tickMaze(PlaydateAPI* _pd, bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position_x = 0;
    m_player.m_position_y = SIZE*9;
    addCluter(15, 20, 0);
    s_path = rand() % 3;
    s_maze = rand() % N_MAZES;
    return false;
  }

 if (s_state == 0) { // start initial move
   enterRoom(&s_state);
 } else if (s_state == 1) { // initial move is done
   m_player.m_target_x = SIZE*3;
   switch (s_path) {
     case 0: m_player.m_target_y = SIZE*3; break;
     case 1: m_player.m_target_y = SIZE*9; break;
     case 2: m_player.m_target_y = SIZE*15; break;
   }
   setGameState(kMovePlayer);
   ++s_state;
 } else if (s_state == 2) {
   setGameState(kAwaitInput);
   ++s_state;
 } else if (s_state == 3) {
   if (getPlayerChoice() != s_sollution[s_maze][s_path]) { //Wrong solution!
     if (m_dungeon.m_lives > 0) --m_dungeon.m_lives;
     else m_dungeon.m_rooms[ m_dungeon.m_level ][ m_dungeon.m_room + 1 ] = kDeath;
     #ifdef DEV
     _pd->system->logToConsole("WRONG");
     #endif
   }
   moveToExit(&s_state);
 } else if (s_state == 4) {
   setGameState(kFadeOut);
 }



  return false;
}
