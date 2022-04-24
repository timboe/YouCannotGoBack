#include <time.h>

#include "game.h"
#include "sprite.h"
#include "generate.h"
#include "render.h"
#include "levels/start.h"
#include "levels/stairs.h"
#include "levels/pword.h"
#include "levels/bridge.h"
#include "levels/maze.h"
#include "levels/death.h"
#include "levels/end.h"
#include "levels/maths.h"
#include "levels/stones.h"
#include "levels/dark.h"
#include "levels/final.h"
#include "levels/chest.h"
#include "levels/empty.h"
#include "levels/saw.h"

static PlaydateAPI* pd = NULL;

static int s_frameCount = 0;
Dungeon_t m_dungeon = {0};
Player_t m_player = {0};
static GameState_t s_gameState = kIdle;
static int s_playerChoice = 0;
static const char* s_displayMsg = NULL;
Clutter_t m_clutter = {0};

bool m_rotated = true;
LCDBitmap* m_rotatedBitmap = NULL;

// static bool s_renderQueued = false; // no longer needed?

//static Layer* s_dungeonLayer;
bool movePlayer();

int getFrameCount() { return s_frameCount; }
GameState_t getGameState() { return s_gameState; }
void setDisplayMsg(const char* _msg) { s_displayMsg = _msg; pd->system->resetElapsedTime(); }
void setGameState(GameState_t _state) { s_gameState = _state; }
int getPlayerChoice() { return s_playerChoice; }

void setPDPtr(PlaydateAPI* p) { pd = p; }

void gameClickConfigHandler(uint32_t buttonPressed) {
  pd->system->logToConsole("PRESSED: %i", buttonPressed);
  if (getGameState() == kDisplayingMsg) setGameState(kLevelSpecific); // break out of message display
  if (kButtonA == buttonPressed) {
    m_rotated = !m_rotated;
  }
  if (getGameState() == kAwaitInput || getGameState() == kLevelSpecificWButtons) {
    if (kButtonUp == buttonPressed) s_playerChoice = 0;
    else if (kButtonRight == buttonPressed) s_playerChoice = 1;
    else if (kButtonDown == buttonPressed) s_playerChoice = 2;
    else return; // Note: Do NOT update game state if another button was pressed
    setGameState(kLevelSpecific);
  }
}

bool newRoom() {
  if (m_dungeon.m_gameOver > 0) { // PLAYER HAS WON OR LOST
    m_dungeon.m_level = 0;
    m_dungeon.m_room = 0;
    m_dungeon.m_rooms[0][0] = kEnd;
  } else  if ( ++m_dungeon.m_room == m_dungeon.m_roomsPerLevel[ m_dungeon.m_level ] ) { // New level
    ++m_dungeon.m_level;
    m_dungeon.m_room = 0;
  };
  ++m_dungeon.m_roomsVisited;
  m_clutter.m_nClutter = 0;
  for (int _i = 0; _i < MAX_PLACE_CLUTTER; ++_i) {
    m_clutter.m_position_x[_i] = 0;
    m_clutter.m_position_y[_i] = 0;
  }
  ++m_dungeon.m_seed;
  s_gameState = kDoInit;
  s_playerChoice = 1;
  #ifdef DEV
  pd->system->logToConsole("ENTER %i [Give:%i val:%i] [Need:%i val:%i] ",
    m_dungeon.m_rooms[ m_dungeon.m_level ][ m_dungeon.m_room ],
    m_dungeon.m_roomGiveHint[ m_dungeon.m_level ][ m_dungeon.m_room ],
    m_dungeon.m_roomGiveHintValue[ m_dungeon.m_level ][ m_dungeon.m_room ],
    m_dungeon.m_roomNeedHint[ m_dungeon.m_level ][ m_dungeon.m_room ],
    m_dungeon.m_roomNeedHintValue[ m_dungeon.m_level ][ m_dungeon.m_room ]);
  #endif
  return false;
}


void dungeonUpdateProc() {

  //s_renderQueued = false;
  if (getGameState() == kIdle || getGameState() == kDoInit) return;
  srand(m_dungeon.m_seed);
  //graphics_context_set_compositing_mode(_ctx, GCompOpSet);


  if (m_rotated) {
    pd->graphics->pushContext(m_rotatedBitmap);
  }

  pd->display->setScale(1);
  pd->graphics->clear(kColorBlack);

  switch (m_dungeon.m_rooms[ m_dungeon.m_level ][ m_dungeon.m_room ]) {
    case kStart: updateProcStart(pd); break;
    case kStairs: updateProcStairs(pd); break;
    case kChest: updateProcChest(pd); break;
    case kEmpty: updateProcEmpty(pd); break;
    case kPword: updateProcPword(pd); break;
    case kBridge: updateProcBridge(pd); break;
    case kMaths: updateProcMaths(pd); break;
    case kStones: updateProcStones(pd); break;
    case kDark: updateProcDark(pd); break;
    case kMaze: updateProcMaze(pd); break;
    case kSaw: updateProcSaw(pd); break;
    case kDeath: updateProcDeath(pd); break;
    case kFinal: updateProcFinal(pd); break;
    case kEnd: updateProcEnd(pd); break;
    default: break;
  }

  //Do msg
  if (getGameState() == kDisplayMsg) {
    renderMessage(pd, s_displayMsg);
    setGameState(kDisplayingMsg);
  }

  if (m_dungeon.m_gameOver == 0) renderProgressBar(/*_thisLayer,*/ pd);

  // Do fade
  if (s_gameState == kFadeIn) renderFade(/*_thisLayer,*/ pd, true);
  else if (s_gameState == kFadeOut) renderFade(/*_thisLayer,*/ pd, false);

  // Draw FPS indicator (dbg only)
  #ifdef DEBUG_MODE
  pd->system->drawFPS(0, 0);
  #endif

  if (m_rotated) {
    pd->graphics->popContext();

    pd->graphics->clear(kColorWhite);
    pd->display->setScale(2);
    // Offset to align in the centre of the rotates screen
    pd->graphics->drawRotatedBitmap(m_rotatedBitmap, -64+8, -8, 90.0f, 0.0f, 0.0f, 1.0f, 1.0f);
  }

}

// Temporary until the playdate eventHandler is functional for inputs
void clickHandlerReplacement() {
  uint32_t current, pushed, released = 0;
  pd->system->getButtonState(&current, &pushed, &released);
  if (pushed & kButtonUp) gameClickConfigHandler(kButtonUp);
  if (pushed & kButtonRight) gameClickConfigHandler(kButtonRight);
  if (pushed & kButtonDown) gameClickConfigHandler(kButtonDown);
  if (pushed & kButtonA) gameClickConfigHandler(kButtonA);
  if (pushed & kButtonB) gameClickConfigHandler(kButtonB);
}

// We don't have timer callbacks, replaces endRenderMsg
void callbackReplacement() {
  if (getGameState() != kDisplayingMsg) { return; }
  if (pd->system->getElapsedTime() >= 1.5f) {
    pd->system->logToConsole("mssage ellapsed");
    setGameState(kLevelSpecific);
  }
}

int gameLoop(void* data) {
  //if (s_renderQueued == true) {
  //  return ;
  //}

  clickHandlerReplacement();
  callbackReplacement();

  if (++s_frameCount == ANIM_FPS) s_frameCount = 0;
  bool requestRedraw = false;

  bool _doInit = false;
  switch (s_gameState) {
    case kIdle: break;
    case kNewRoom: requestRedraw = newRoom(); break;
    case kMovePlayer: requestRedraw = movePlayer(); break;
    case kAwaitInput: requestRedraw = (s_frameCount == 0 || s_frameCount == ANIM_FPS/2 ? true : false); break;
    case kFadeIn: case kFadeOut: requestRedraw = true; break;
    case kDisplayMsg: requestRedraw = true; break;
    case kDisplayingMsg: requestRedraw = false; break; // Wait for timer to expire or button click
    // For the init level case, we call the tick fn with a boolean flag, but we then need to wait until we fade in before we tick propper
    case kDoInit: _doInit = true; s_gameState = kFadeIn; // FALL THROUGH
    case kLevelSpecific: case kLevelSpecificWButtons:
    switch (m_dungeon.m_rooms[ m_dungeon.m_level ][ m_dungeon.m_room ]) {
      case kStart: requestRedraw = tickStart(_doInit); break;
      case kStairs: requestRedraw = tickStairs(_doInit); break;
      case kChest: requestRedraw = tickChest(_doInit); break;
      case kEmpty: requestRedraw = tickEmpty(_doInit); break;
      case kPword: requestRedraw = tickPword(pd, _doInit); break;
      case kBridge: requestRedraw = tickBridge(pd, _doInit); break;
      case kMaths: requestRedraw = tickMaths(pd,_doInit); break;
      case kStones: requestRedraw = tickStones(pd, _doInit); break;
      case kDark: requestRedraw = tickDark(_doInit); break;
      case kMaze: requestRedraw = tickMaze(pd, _doInit); break;
      case kSaw: requestRedraw = tickSaw(_doInit); break;
      case kDeath: requestRedraw = tickDeath(_doInit); break;
      case kFinal: requestRedraw = tickFinal(_doInit); break;
      case kEnd: requestRedraw = tickEnd(pd, _doInit); break;
      default: break;
    } break;
    default: break;
  }


  if (requestRedraw == true) {
    //s_renderQueued = true;
    //layer_mark_dirty(s_dungeonLayer);
    dungeonUpdateProc(); // TODO check the location of this
  }

  // TODO: The docs say that returning 0 does not request a redraw,
  // but it is currently 
  requestRedraw = true;


  return (int)requestRedraw;
}


bool movePlayer() {
  if (s_frameCount % 3 == 0 && ++m_player.m_playerFrame == MAX_FRAMES) m_player.m_playerFrame = 0;

  if      (m_player.m_target_x > m_player.m_position_x) m_player.m_position_x += PLAYER_SPEED;
  if      (m_player.m_target_y > m_player.m_position_y) m_player.m_position_y += PLAYER_SPEED;
  else if (m_player.m_target_y < m_player.m_position_y) m_player.m_position_y -= PLAYER_SPEED;
  if (m_player.m_target_x == m_player.m_position_x && m_player.m_target_y == m_player.m_position_y) {
    m_player.m_playerFrame = 0;
    s_gameState = kLevelSpecific;
  }
  return true;
}


void gameWindowLoad() {
  setGameState(kIdle);

  m_rotatedBitmap = pd->graphics->newBitmap(400, 240, kColorWhite);

  //s_dungeonLayer = layer_create( _b );
  //layer_add_child(window_get_root_layer(_window), s_dungeonLayer);
  //layer_set_update_proc(s_dungeonLayer, dungeonUpdateProc);

  generate(pd);

}

void gameWindowUnload() {
  pd->graphics->freeBitmap(m_rotatedBitmap);
  m_rotatedBitmap = NULL;
}

int getHintValueMax(Hints_t _hint) {
  switch (_hint) {
    case kShield: return MAX_SHIELD_COLOUR;
    case kSymbol: return MAX_SYMBOL;
    case kSpell: return MAX_SPELLS;
    case kNumber: return MAX_NUMBER;
    case kGreek: return MAX_GREEK;
    default: return 0;
  }
}

// case 0: return GColorRed;
// case 1: return GColorBlack;
// case 2: return GColorWhite;
// case 3: return GColorBlue;
// default: return GColorLightGray;

int getShieldA(int _value) {
  switch (_value) {
    case kRWb: return 0;
    case kRbB: return 0;
    case kbBb: return 1;
    case kbRW: return 1;
    case kBbR: return 3;
    case kBRR: default: return 3;
  }
}

int getShieldB(int _value) {
  switch (_value) {
    case kRWb: return 2;
    case kRbB: return 1;
    case kbBb: return 3;
    case kbRW: return 0;
    case kBbR: return 1;
    case kBRR: default: return 0;
  }
}

int getShieldC(int _value) {
  switch (_value) {
    case kRWb: return 1;
    case kRbB: return 3;
    case kbBb: return 1;
    case kbRW: return 2;
    case kBbR: return 0;
    case kBRR: default: return 0;
  }
}

const char* const m_spellNames[MAX_SPELLS] = {
  "heian",
  "loper",
  "rivet",
  "drain",
  "nasby",
  "gouge",
  "smoke",
  "shrub",
  "geoid",
  "conic",
  "varna",
  "nymph",
  "herma",
  "forte",
  "abase",
  "notch",
  "hades",
  "xylyl",
  "endue",
  "razor",
  "gloom",
  "hazer",
  "gotra",
  "henge",
  "rigor",
  "mercy",
  "hinge",
  "warty",
  "verse",
  "groat",
  "buxom"
};
