#include "bridge.h"

#define N_MAZES 2
#define N_MAZE_STEP 50

static uint16_t s_state = 0;
static uint16_t s_maze = 0;
static uint16_t s_path = 0;
//static uint16_t s_correct = 0;
static const uint16_t s_sollution[N_MAZES][3] = {{2,0,1},{1,0,2}};

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
  [1].m_path2 = {5,16, 8,16, 8,11, 9,11, 9,3, 11,3, 11,10, 13,10, 13,14, 15,14}
};

void drawLine(GContext* _ctx, int _x1, int _y1, int _x2, int _y2) {
  if (_x1 == 0 || _x2 == 0) return;
  graphics_draw_line(_ctx, GPoint(_x1*SIZE, _y1*SIZE), GPoint(_x2*SIZE,_y2*SIZE));
}

void updateProcMaze(GContext* _ctx) {

  renderFloor(_ctx, 0);

  graphics_context_set_stroke_width(_ctx, 2);
  switch (m_dungeon.m_level) {
    case 0: graphics_context_set_stroke_color(_ctx, GColorWhite); break;
    case 1: graphics_context_set_stroke_color(_ctx, GColorRed); break;
    default: graphics_context_set_stroke_color(_ctx, GColorBlack); break;
  }
  for (int _p = 0; _p < N_MAZE_STEP - 3; _p += 2) {
    drawLine(_ctx, s_mazes[s_maze].m_path0[_p], s_mazes[s_maze].m_path0[_p+1],  s_mazes[s_maze].m_path0[_p+2], s_mazes[s_maze].m_path0[_p+3]);
    drawLine(_ctx, s_mazes[s_maze].m_path1[_p], s_mazes[s_maze].m_path1[_p+1],  s_mazes[s_maze].m_path1[_p+2], s_mazes[s_maze].m_path1[_p+3]);
    drawLine(_ctx, s_mazes[s_maze].m_path2[_p], s_mazes[s_maze].m_path2[_p+1],  s_mazes[s_maze].m_path2[_p+2], s_mazes[s_maze].m_path2[_p+3]);
  }

  renderPlayer(_ctx);
  renderWalls(_ctx, true, true, true, true);
  renderClutter(_ctx);

  if (getGameState() == kAwaitInput && getFrameCount() < ANIM_FPS/2) {
    drawBitmap(_ctx, m_arrow, 15, 4);
    drawBitmap(_ctx, m_arrow, 15, 8);
    drawBitmap(_ctx, m_arrow, 15, 12);
  }

}

bool tickMaze(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    m_player.m_position = GPoint(0, SIZE*9);
    addCluter(3 + m_dungeon.m_level, 15, 20, 0);
    s_path = rand() % 3;
    s_maze = rand() % N_MAZES;
    return false;
  }

 if (s_state == 0) { // start initial move
   enterRoom(&s_state);
 } else if (s_state == 1) { // initial move is done
   switch (s_path) {
     case 0: m_player.m_target = GPoint(SIZE*3, SIZE*3); break;
     case 1: m_player.m_target = GPoint(SIZE*3, SIZE*9); break;
     case 2: m_player.m_target = GPoint(SIZE*3, SIZE*15); break;
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
   }
   switch (getPlayerChoice()) {
     case 0: m_player.m_target = GPoint(SIZE*17, SIZE*5); break;
     case 1: m_player.m_target = GPoint(SIZE*17, SIZE*9); break;
     case 2: m_player.m_target = GPoint(SIZE*17, SIZE*13); break;
   }
   setGameState(kMovePlayer);
   ++s_state;
 } else if (s_state == 4) {
   setGameState(kFadeOut);
 }

 //     ++s_state;
 //  } else if (s_state == 2) { // Move to chosen bridge
 //    switch (getPlayerChoice()) {
 //      case 0: m_player.m_target = GPoint(SIZE*3, SIZE*5); break;
 //      case 1: m_player.m_target = GPoint(SIZE*3, SIZE*9); break;
 //      case 2: m_player.m_target = GPoint(SIZE*3, SIZE*13); break;
 //    }
 //    setGameState(kMovePlayer);
 //    ++s_state;
 //  } else if (s_state == 3) { // move to center of bridge
 //    m_player.m_target.x = SIZE*8;
 //    if (getPlayerChoice() == s_correct) s_state = 5;
 //    else s_state = 4;
 //    setGameState(kMovePlayer);
 //  } else if (s_state == 4) { // LOOSE
 //   m_dungeon.m_gameOver = true;
 //   m_dungeon.m_fallingDeath = true;
 //   s_state = 6;
 // } else if (s_state == 5) {
 //   m_player.m_target.x = SIZE*17;
 //   setGameState(kMovePlayer);
 //   s_state = 6;
 // } else if (s_state == 6) {
 //   setGameState(kFadeOut);
 // }


  return false;
}
