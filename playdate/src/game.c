#include <math.h>

#include "game.h"
#include "sprite.h"
#include "generate.h"
#include "render.h"
#include "sound.h"
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
#include "levels/bomb.h"
#include "levels/boxes.h"
#include "levels/spikes.h"
#include "levels/shapes.h"
#include "levels/greek.h"
#include "levels/spikeball.h"
#include "levels/pattern.h"
#include "levels/gamble.h"
#include "levels/shortcut.h"
#include "levels/arrows.h"

PlaydateAPI* pd = NULL;

static int s_frameCount = 0;
Dungeon_t m_dungeon = {0};
Player_t m_player = {0};
static GameState_t s_gameState = kIdle;
static int s_playerChoice = 0;
static const char* s_displayMsg = NULL;
Clutter_t m_clutter = {0};

bool m_rotated = false;
bool m_autoRotation = true;
LCDBitmap* m_rotatedBitmap = NULL;


int getFrameCount() { return s_frameCount; }
GameState_t getGameState() { return s_gameState; }
void setDisplayMsg(const char* _msg) { s_displayMsg = _msg; pd->system->resetElapsedTime(); }
void setGameState(GameState_t _state) { s_gameState = _state; }
int getPlayerChoice() { return s_playerChoice; }
void resetPlayerChoice() { s_playerChoice = -1; }

bool getFlash(bool _constant) {
  bool _value = (s_gameState != kFadeIn && s_gameState != kFadeOut
    && s_frameCount % (ANIM_FPS/2) < ANIM_FPS/4
    && (m_dungeon.m_ticksInLevel < ANIM_FPS*3 || _constant));
  if (_value && !_constant && s_frameCount % ANIM_FPS/2 == 0) beepSound();
  return _value;
}

void setPDPtr(PlaydateAPI* p) { pd = p; }

void gameClickConfigHandler(uint32_t buttonPressed) {
  clickSound();
  if (getGameState() == kDisplayingMsg) setGameState(kLevelSpecific); // break out of message display
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
    ++m_dungeon.m_difficulty;
    m_dungeon.m_room = 0;
    if (m_dungeon.m_level == MAX_LEVELS-1) {
      updateMusic(1); // Moving in to the final level
    }
  };
  ++m_dungeon.m_roomsVisited;
  m_dungeon.m_ticksTotal += m_dungeon.m_ticksInLevel;
  m_dungeon.m_ticksInLevel = 0;
  s_frameCount = 0;
  m_clutter.m_nClutter = 0;
  for (int _i = 0; _i < MAX_PLACE_CLUTTER; ++_i) {
    m_clutter.m_position_x[_i] = 0;
    m_clutter.m_position_y[_i] = 0;
  }
  ++m_dungeon.m_seed;
  setGameState(kDoInit);
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


int getHorizontalOffset() {
  int off = 0;
  switch (m_dungeon.m_rooms[ m_dungeon.m_level ][ m_dungeon.m_room ]) {
    case kStart: off = (m_player.m_position_x * -1) + 24; break;
    case kEnd: off = -12; break;
    default: off = m_player.m_position_x * -1;
  }
  if (off > 0) off = 0;
  return off < -24 ? -24 : off;
}

void dungeonUpdateProc() {

  if (getGameState() == kIdle || getGameState() == kDoInit) {
    return;
  };
  srand(m_dungeon.m_seed);

  if (m_rotated) {
    pd->graphics->pushContext(m_rotatedBitmap);
    pd->graphics->setDrawOffset(0, 0);
  } else {
    pd->graphics->setDrawOffset(128, 36); 
    // 128 left border, 144 pebble display (18 8-pix tiles), 128 right border = 400
    // 36 top border, 168, 36 = 240
  }

  pd->display->setScale(1);
  renderClear(pd, true);

  switch (m_dungeon.m_rooms[ m_dungeon.m_level ][ m_dungeon.m_room ]) {
    case kStart: updateProcStart(pd); break;
    case kStairs: updateProcStairs(pd); break;
    case kChest: updateProcChest(pd); break;
    case kGamble: updateProcGamble(pd); break;
    case kShortcut: updateProcShortcut(pd); break;  
    case kEmpty: updateProcEmpty(pd); break;
    case kPword: updateProcPword(pd); break;
    case kBridge: updateProcBridge(pd); break;
    case kMaths: updateProcMaths(pd); break;
    case kStones: updateProcStones(pd); break;
    case kDark: updateProcDark(pd); break;
    case kPattern: updateProcPattern(pd); break;
    case kMaze: updateProcMaze(pd); break;
    case kArrows: updateProcArrows(pd); break;
    case kGreek: updateProcGreek(pd); break;
    case kSaw: updateProcSaw(pd); break;
    case kBomb: updateProcBomb(pd); break;
    case kBoxes: updateProcBoxes(pd); break;
    case kSpikes: updateProcSpikes(pd); break;
    case kSBall: updateProcSpikeball(pd); break;
    case kShapes: updateProcShapes(pd); break;
    case kDeath: updateProcDeath(pd); break;
    case kFinal: updateProcFinal(pd); break;
    case kEnd: updateProcEnd(pd, m_rotated); break;
    default: break;
  }

  //Do msg
  if (getGameState() == kDisplayMsg) {
    renderMessage(pd, s_displayMsg, m_rotated);
    setGameState(kDisplayingMsg);
  }

  if (m_dungeon.m_gameOver == 0 && !m_rotated) renderProgressBar(pd, m_rotated);

  if (m_rotated) {
    pd->graphics->popContext();

    pd->graphics->clear(kColorBlack);
    pd->display->setScale(2);
    pd->graphics->setDrawOffset(0, 0);
    // Offset to align in the center of the rotates screen
    pd->graphics->drawRotatedBitmap(m_rotatedBitmap, -56, getHorizontalOffset(), 90.0f, 0.0f, 0.0f, 1.0f, 1.0f);

  } else {
    pd->graphics->setDrawOffset(0, 0);
    renderGameFrame(pd);
  }

  if (m_dungeon.m_gameOver == 0 && m_rotated) renderProgressBar(pd, m_rotated);

  // Do fade
  if (s_gameState == kFadeIn) renderFade(pd, true, m_rotated);
  else if (s_gameState == kFadeOut) renderFade(pd, false, m_rotated);

  // Draw FPS indicator (dbg only)
  #ifdef DEBUG_MODE
  pd->system->drawFPS(0, 0);
  #endif
}

// Temporary until the playdate eventHandler is functional for inputs
void clickHandlerReplacement() {
  PDButtons current, pushed, released = 0;
  pd->system->getButtonState(&current, &pushed, &released);
  if (m_rotated) {
    //if (pushed & kButtonUp) gameClickConfigHandler(kButtonLeft); // Button not used
    if (pushed & kButtonRight) gameClickConfigHandler(kButtonUp);
    if (pushed & kButtonDown) gameClickConfigHandler(kButtonRight);
    if (pushed & kButtonLeft) gameClickConfigHandler(kButtonDown);
  } else {
    if (pushed & kButtonUp) gameClickConfigHandler(kButtonUp);
    if (pushed & kButtonRight) gameClickConfigHandler(kButtonRight);
    if (pushed & kButtonDown) gameClickConfigHandler(kButtonDown);
    //if (pushed & kButtonLeft) gameClickConfigHandler(kButtonLeft); // Button not used
  }
  //if (pushed & kButtonA) gameClickConfigHandler(kButtonA); // Button not used
  //if (pushed & kButtonB) gameClickConfigHandler(kButtonB); // Button not used

  static float fx, fy, fz;
  if (m_autoRotation) {
    pd->system->getAccelerometer(&fx, &fy, &fz);
    m_rotated = fabs(fx) > fabs(fy);
  }
}

// We don't have timer callbacks, replaces endRenderMsg
void callbackReplacement() {
  if (getGameState() != kDisplayingMsg) { return; }
  if (pd->system->getElapsedTime() >= 1.5f) {
    setGameState(kLevelSpecific);
  }
}

int gameLoop(void* data) {

  clickHandlerReplacement();
  callbackReplacement();

  if (++s_frameCount == ANIM_FPS) s_frameCount = 0;
  bool requestRedraw = false;
  ++m_dungeon.m_ticksInLevel;

  bool _doInit = false;
  switch (s_gameState) {
    case kIdle: break;
    case kNewRoom: requestRedraw = newRoom(); break;
    case kMovePlayer: requestRedraw = movePlayer(); break;
    case kAwaitInput: requestRedraw = (s_frameCount % (ANIM_FPS/4) == 0); break;
    case kFadeIn: case kFadeOut: requestRedraw = true; break;
    case kDisplayMsg: requestRedraw = true; break;
    case kDisplayingMsg: requestRedraw = false; break; // Wait for timer to expire or button click
    // For the init level case, we call the tick fn with a boolean flag, but we then need to wait until we fade in before we tick proper
    case kDoInit: _doInit = true; s_gameState = kFadeIn; // FALL THROUGH
    case kLevelSpecific: case kLevelSpecificWButtons:
    switch (m_dungeon.m_rooms[ m_dungeon.m_level ][ m_dungeon.m_room ]) {
      case kStart: requestRedraw = tickStart(_doInit); break;
      case kStairs: requestRedraw = tickStairs(_doInit); break;
      case kChest: requestRedraw = tickChest(_doInit); break;
      case kGamble: requestRedraw = tickGamble(_doInit); break;
      case kShortcut: requestRedraw = tickShortcut(_doInit); break;
      case kEmpty: requestRedraw = tickEmpty(_doInit); break;
      case kPword: requestRedraw = tickPword(pd, _doInit); break;
      case kBridge: requestRedraw = tickBridge(pd, _doInit); break;
      case kMaths: requestRedraw = tickMaths(pd,_doInit); break;
      case kStones: requestRedraw = tickStones(pd, _doInit); break;
      case kDark: requestRedraw = tickDark(_doInit); break;
      case kPattern: requestRedraw = tickPattern(_doInit); break;
      case kMaze: requestRedraw = tickMaze(pd, _doInit); break;
      case kArrows: requestRedraw = tickArrows(pd, _doInit); break;
      case kGreek: requestRedraw = tickGreek(pd, _doInit); break;
      case kSaw: requestRedraw = tickSaw(_doInit); break;
      case kBomb: requestRedraw = tickBomb(_doInit); break;
      case kBoxes: requestRedraw = tickBoxes(pd, _doInit); break;
      case kSpikes: requestRedraw = tickSpikes(_doInit); break;
      case kShapes: requestRedraw = tickShapes(pd,_doInit); break;
      case kSBall: requestRedraw = tickSpikeball(pd, _doInit); break;
      case kDeath: requestRedraw = tickDeath(_doInit); break;
      case kFinal: requestRedraw = tickFinal(_doInit); break;
      case kEnd: requestRedraw = tickEnd(pd, _doInit); break;
      default: break;
    } break;
    default: break;
  }

  // Special case (bomb needs to be able to explode at any time)
  if (m_dungeon.m_rooms[ m_dungeon.m_level ][ m_dungeon.m_room ] == kBomb) {
    bombTimer();
    requestRedraw = true;
  }

  // Rotation prompt
  if (!m_rotated && m_dungeon.m_rooms[m_dungeon.m_level][m_dungeon.m_room] == kStart) requestRedraw = true;


  if (requestRedraw) {
    dungeonUpdateProc();
  }

  // TODO: The docs say that returning 0 does not request a redraw,
  // but it is currently required
  requestRedraw = true;

  return (int)requestRedraw;
}

bool atDestination() {
  return (m_player.m_target_x == m_player.m_position_x && m_player.m_target_y == m_player.m_position_y);
}

bool movePlayer() {
  if (s_frameCount % 3 == 0) {
    if (++m_player.m_playerFrame == MAX_FRAMES) {
      m_player.m_playerFrame = 0;
    }
    if (m_player.m_playerFrame % 2 == 0) footSound();
  }

  if      (m_player.m_target_x > m_player.m_position_x) m_player.m_position_x += PLAYER_SPEED;
  if      (m_player.m_target_y > m_player.m_position_y) m_player.m_position_y += PLAYER_SPEED;
  else if (m_player.m_target_y < m_player.m_position_y) m_player.m_position_y -= PLAYER_SPEED;
  if (atDestination()) {
    m_player.m_playerFrame = 0;
    s_gameState = kLevelSpecific;
  }
  return true;
}

void menuOptionsCallback(void* userdata) {
  if (userdata == NULL) {
    // chose reset
    return generate(pd);
  }

  int _value = pd->system->getMenuItemValue((PDMenuItem*)userdata);
  if (_value == 2) {
    m_rotated = true;
    m_autoRotation = false;
    pd->system->setPeripheralsEnabled(kNone);
  } else if (_value == 1) {
    m_rotated = false; 
    m_autoRotation = false;
    pd->system->setPeripheralsEnabled(kNone);
  } else {
    m_autoRotation = true;
    pd->system->setPeripheralsEnabled(kAccelerometer);
  }
}

void menuOptionsCallbackAudio(void* userdata) {
  int _value = pd->system->getMenuItemValue((PDMenuItem*)userdata);
  if (_value == 0) {
    music(true);
    sfx(true);
  } else if (_value == 1) {
    music(true);
    sfx(false);
  } else if (_value == 2) {
    music(false);
    sfx(true);
  } else {
    music(false);
    sfx(false);
  }
}

void gameWindowLoad() {
  setGameState(kIdle);

  m_rotatedBitmap = pd->graphics->newBitmap(400, 240, kColorWhite);

  static const char* options2[] = {"Music+SFX", "Music", "SFX", "None"};
  PDMenuItem* _menu2 = pd->system->addOptionsMenuItem("audio", options2, 4, menuOptionsCallbackAudio, NULL);
  pd->system->setMenuItemUserdata(_menu2, (void*) _menu2); // User data is a pointer to the menu itself

  pd->system->addMenuItem("restart", menuOptionsCallback, NULL);

  static const char* options[] = {"Auto", "Landscape", "Portrait"};
  PDMenuItem* _menu = pd->system->addOptionsMenuItem("rotate", options, 3, menuOptionsCallback, NULL);
  pd->system->setMenuItemUserdata(_menu, (void*) _menu); // User data is a pointer to the menu itself

  pd->system->setPeripheralsEnabled(kAccelerometer);
  
  pd->graphics->clear(kColorBlack);
  renderGameFrame(pd);

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
    case kGreekLetter: return MAX_GREEK;
    default: return 0;
  }
}

// Mappings below are
//case 0: return kColorBlack;
//case 1: return kColorWhite;
//case 2: return kColorChekerboard;

int getShieldA(int _value) {
  switch (_value) {
    case kBWC: return 0;
    case kBCW: return 0;
    case kWCB: return 1;
    case kWBC: return 1;
    case kCBW: return 2;
    case kCWB: return 2;
    default: return 0;
  }
}

int getShieldB(int _value) {
  switch (_value) {
    case kBWC: return 1;
    case kBCW: return 2;
    case kWCB: return 2;
    case kWBC: return 0;
    case kCBW: return 0;
    case kCWB: return 1;
    default: return 0;
  }
}

int getShieldC(int _value) {
  switch (_value) {
    case kBWC: return 2;
    case kBCW: return 1;
    case kWCB: return 0;
    case kWBC: return 2;
    case kCBW: return 1;
    case kCWB: return 0;
    default: return 0;
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
