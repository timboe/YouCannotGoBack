#include "arrows.h"
#include "common.h"
#include "../sound.h"

static uint16_t s_state = 0;
static uint16_t s_tickA = 0;
static uint16_t s_tickB = 0;


static uint8_t s_start = 0;
static uint8_t s_correct = 0;

static uint8_t s_maze0[3][3] = {0};
static uint8_t s_maze1[5][7] = {0};

static uint8_t s_stack[5*7*2] = {0};
static uint16_t s_head = 0;

typedef enum {
  kN,
  kE,
  kS,
  kW,
  kNE,
  kSE,
  kSW,
  kNW
} Options_t;

static Options_t s_mazeDisp0[3][3] = {kN};
static Options_t s_mazeDisp1[5][7] = {kN};

LCDBitmap* bmp(PlaydateAPI* _pd, uint8_t _x) {
 return _pd->graphics->getTableBitmap(m_floorArrow, (_x + s_tickB) % 8);
}

void updateProcArrows(PlaydateAPI* _pd) {
  renderFloor(_pd, 0);
  renderStandingStone(_pd, 5, 6 + s_start*4, kColorBlack, kCircle);

  renderClutter(_pd);
  renderPlayer(_pd);

  // render floor arrows
  const bool _r = m_player.m_position_x > SIZE*8 && getFlash(true);
  const int8_t MAX_X = (m_dungeon.m_difficulty == 0 ? 3 : 5);
  const int8_t MAX_Y = (m_dungeon.m_difficulty == 0 ? 3 : 7);
  for (int _x = 0; _x < MAX_X; ++_x) {
    for (int _y = 0; _y < MAX_Y; ++_y) {
      if (m_dungeon.m_difficulty == 0) {
        _pd->graphics->setDrawMode(_r && s_maze0[_x][_y] == 2 ? kDrawModeInverted : kDrawModeCopy);
        drawBitmap(_pd, bmp(_pd, s_mazeDisp0[_x][_y]), 4 + (4*_x), 5 + (4*_y));
      } else {
        _pd->graphics->setDrawMode(_r && s_maze1[_x][_y] == 2 ? kDrawModeInverted : kDrawModeCopy);
        drawBitmap(_pd, bmp(_pd, s_mazeDisp1[_x][_y]), 4 + (2*_x), 3 + (2*_y));
      }
    }
  }
  _pd->graphics->setDrawMode(kDrawModeCopy);

  // Draw on top
  if (m_player.m_position_x <= SIZE*4 && getFlash(true)) renderPlayer(_pd);

  renderWalls(_pd, true, true, true, true);
  renderWallClutter(_pd);
  renderArrows(_pd, 15, 5, 4);
}

void pushStack(int8_t _x, int8_t _y) {
  s_stack[(s_head * 2) + 0] = _x;
  s_stack[(s_head * 2) + 1] = _y;
  if (m_dungeon.m_difficulty == 0) s_maze0[_x][_y] = 1;
  else                             s_maze1[_x][_y] = 1;
  ++s_head;
}

void popStack(int8_t* _xPtr, int8_t* _yPtr) {
  --s_head;
  *_xPtr = s_stack[(s_head * 2) + 0];
  *_yPtr = s_stack[(s_head * 2) + 1];
}

void getStack(uint16_t _loc, int8_t* _xPtr, int8_t* _yPtr) {
  *_xPtr = s_stack[(_loc * 2) + 0];
  *_yPtr = s_stack[(_loc * 2) + 1];
}

bool getOccupied(int8_t _x, int8_t _y) {
  const int8_t MAX_X = (m_dungeon.m_difficulty == 0 ? 3 : 5);
  const int8_t MAX_Y = (m_dungeon.m_difficulty == 0 ? 3 : 7);
  if (_x < 0 || _x >= MAX_X || _y < 0 || _y >= MAX_Y) return true;
  if (m_dungeon.m_difficulty == 0) return s_maze0[_x][_y];
  else                             return s_maze1[_x][_y];
}

bool tryMove(int8_t* _xPtr, int8_t* _yPtr, bool _backtrack) {
  // Get available options
  Options_t _options[8] = {kN};
  uint8_t _nOpt = 0;
  if (!getOccupied(*_xPtr + 0, *_yPtr - 1)) _options[_nOpt++] = kN;
  if (!getOccupied(*_xPtr + 1, *_yPtr + 0)) _options[_nOpt++] = kE;
  if (!getOccupied(*_xPtr + 0, *_yPtr + 1)) _options[_nOpt++] = kS;
  if (!getOccupied(*_xPtr - 1, *_yPtr + 0)) _options[_nOpt++] = kW;
  if (m_dungeon.m_difficulty >= 2) {
    if (!getOccupied(*_xPtr + 1, *_yPtr - 1)) _options[_nOpt++] = kNE;
    if (!getOccupied(*_xPtr + 1, *_yPtr + 1)) _options[_nOpt++] = kSE;
    if (!getOccupied(*_xPtr - 1, *_yPtr + 1)) _options[_nOpt++] = kSW;
    if (!getOccupied(*_xPtr - 1, *_yPtr - 1)) _options[_nOpt++] = kNW;
  }
  if (_nOpt == 0) {
    // Cannot move
    return false;
  }
  // Were we backtracking? If so we have stopped and need to re-push this popped coord
  if (_backtrack) pushStack(*_xPtr, *_yPtr);
  const Options_t _chosenOpt = _options[ rand() % _nOpt ];
  switch (_chosenOpt) {
    case kN: *_yPtr -= 1; break;
    case kE: *_xPtr += 1; break;
    case kS: *_yPtr += 1; break;
    case kW: *_xPtr -= 1; break; 
    case kNE: *_xPtr += 1; *_yPtr -= 1; break;
    case kSE: *_xPtr += 1; *_yPtr += 1; break;
    case kSW: *_xPtr -= 1; *_yPtr += 1; break;
    case kNW: *_xPtr -= 1; *_yPtr -= 1; break; 
    default: break;
  }
  // Do move
  pushStack(*_xPtr, *_yPtr);
  return true;
}

void gen(PlaydateAPI* _pd) {
  memset(&s_maze0, 0, sizeof(uint8_t)*3*3);
  memset(&s_maze1, 0, sizeof(uint8_t)*5*7);
  memset(&s_stack, 0, sizeof(uint8_t)*5*7*2);
  const int8_t MAX_X = (m_dungeon.m_difficulty == 0 ? 3 : 5);
  const int8_t MAX_Y = (m_dungeon.m_difficulty == 0 ? 3 : 7);
  uint8_t _begin, _end;
  // Modify entry and exit points for the larger maze
  if (m_dungeon.m_difficulty == 0) {
    _begin = s_start;
    _end = s_correct;
  }  else {
    _begin = (s_start * 2) + 1;
    _end = (s_correct * 2) + 1;
  }
  s_head = 0;
  int8_t _x = 0, _y = _begin;
  bool _backtrack = false;
  // Add start
  pushStack(_x, _y);
  #ifdef DEV
  _pd->system->logToConsole(" Arrow: START %i,%i", _x, _y);
  #endif
  do {
    // Try and move to a new square
    bool didMove = tryMove(&_x, &_y, _backtrack);
    // Else unwind one square
    if (!didMove) {
      popStack(&_x, &_y);
      _backtrack = true;
      #ifdef DEV
      _pd->system->logToConsole(" Arrow:   <- %i,%i", _x, _y);
      #endif
    } else {
      _backtrack = false;
      #ifdef DEV
      _pd->system->logToConsole(" Arrow: -> %i,%i", _x, _y);
      #endif
    }
  } while (!(_x == MAX_X-1 && _y == _end));
  #ifdef DEV
  _pd->system->logToConsole(" Arrow: STOP %i,%i (target %i,%i)", _x, _y, MAX_X-1, _end);
  #endif
}

Options_t getDir(int8_t _curX, int8_t _curY, int8_t _toX, int8_t _toY) {
  if (_toX > _curX && _toY < _curY) return kNE;
  if (_toX > _curX && _toY > _curY) return kSE;
  if (_toX < _curX && _toY > _curY) return kSW;
  if (_toX < _curX && _toY < _curY) return kNW;
  //
  if (_toX > _curX) return kE;
  if (_toX < _curX) return kW;
  if (_toY > _curY) return kS;
  if (_toY < _curY) return kN;
  return kN;
}

void set(PlaydateAPI* _pd) {
  // Walk the stack up to the last place (which must point right)
  for (int16_t _s = 0; _s < s_head - 1; ++_s) {
    int8_t _curX = 0, _curY = 0, _toX = 0, _toY = 0;
    getStack(_s + 0, &_curX, &_curY);
    getStack(_s + 1, &_toX, &_toY);
    #ifdef DEV
    _pd->system->logToConsole(" APath: Path %i,%i ->  %i,%i", _curX, _curY, _toX, _toY);
    #endif
    if (m_dungeon.m_difficulty == 0) {
      s_mazeDisp0[_curX][_curY] = getDir(_curX, _curY, _toX, _toY);
      s_maze0[_curX][_curY] = 2; // on the path
    } else {
      s_mazeDisp1[_curX][_curY] = getDir(_curX, _curY, _toX, _toY);
      s_maze1[_curX][_curY] = 2; // on the path
    }
  }
  // Final path piece (we already set this to kE)
  int8_t _x = 0, _y = 0;
  getStack(s_head - 1, &_x, &_y);
    #ifdef DEV
    _pd->system->logToConsole(" APath: FIN %i,%i", _x, _y);
    #endif
  if (m_dungeon.m_difficulty == 0) {
    s_maze0[_x][_y] = 2;
  } else {
    s_maze1[_x][_y] = 2;
  }
}

bool tickArrows(PlaydateAPI* _pd, bool _doInit) {
  #define TICK 6
  if (_doInit == true) {
    const int8_t MAX_X = (m_dungeon.m_difficulty == 0 ? 3 : 5);
    const int8_t MAX_Y = (m_dungeon.m_difficulty == 0 ? 3 : 7);
    s_state = 0;
    s_tickA = TICK/3;
    s_tickB = TICK;
    m_player.m_position_x = 0;
    m_player.m_position_y = SIZE*9;
    s_start = rand() % 3; // entry point
    s_correct = rand() % 3; // exit point
    // Randomize
    const uint8_t maxR = m_dungeon.m_difficulty >= 2 ? 8 : 4;
    for (int _x = 0; _x < MAX_X; ++_x) {
      for (int _y = 0; _y < MAX_Y; ++_y) {
        if (m_dungeon.m_difficulty == 0) s_mazeDisp0[_x][_y] = (Options_t) rand() % maxR;
        else                             s_mazeDisp1[_x][_y] = (Options_t) rand() % maxR;
      }
    }
    // But the three exits always point right
    if (m_dungeon.m_difficulty == 0) {
      s_mazeDisp0[2][0] = kE;
      s_mazeDisp0[2][1] = kE;
      s_mazeDisp0[2][2] = kE;
    } else {
      s_mazeDisp1[4][1] = kE;
      s_mazeDisp1[4][3] = kE;
      s_mazeDisp1[4][5] = kE;
    }
    // Make path
    gen(_pd);
    // Set path
    set(_pd);
    return false;
  }

  if (s_state == 0) { // start initial move
    enterRoom(&s_state);
  } else if (s_state == 1) { // initial move is done
    m_player.m_target_x = SIZE*4;
    switch (s_start) {
      case 0: m_player.m_target_y = SIZE*5; break;
      case 1: m_player.m_target_y = SIZE*9; break;
      case 2: m_player.m_target_y = SIZE*13; break;
    }
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 2) { // Anim
    setGameState(kLevelSpecific);
    if (--s_tickA == 0) {
      s_tickA = TICK/3;
      clackSound();
      if (--s_tickB == 0) ++s_state; 
    }
  } else if (s_state == 3) {
    setGameState(kAwaitInput);
    ++s_state;
  } else if (s_state == 4) {
   if (getPlayerChoice() != s_correct) { //Wrong solution!
     if (m_dungeon.m_lives > 0) --m_dungeon.m_lives;
     else m_dungeon.m_rooms[ m_dungeon.m_level ][ m_dungeon.m_room + 1 ] = kDeath;
     #ifdef DEV
     _pd->system->logToConsole("WRONG");
     #endif
   }
   moveToExit(&s_state);
  } else if (s_state == 5) {
    setGameState(kFadeOut);
  }
  return true;

}
