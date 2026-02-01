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
#ifdef YCGBv2
#include "levels/bomb.h"
#include "levels/boxes.h"
#endif

static int s_frameCount = 0;
Dungeon_t m_dungeon = {0};
Player_t m_player = {0};
static GameState_t s_gameState = kIdle;
static int s_playerChoice = 0;
static const char* s_displayMsg = NULL;
Clutter_t m_clutter = {0};
static bool s_renderQueued = false;

static AppTimer* s_gameLoopTimer = NULL;
static Layer* s_dungeonLayer;
bool movePlayer();

#ifdef DEBUG_MODE
static AppTimer* s_FPSTimer = NULL;
static int s_FPS = 0;
static int s_lastSecondFPS = 0;
void FPSTimer(void* data);
#endif

int getFrameCount() { return s_frameCount; }
GameState_t getGameState() { return s_gameState; }
void setDisplayMsg(const char* _msg) { s_displayMsg = _msg; }
void setGameState(GameState_t _state) { s_gameState = _state; }
int getPlayerChoice() { return s_playerChoice; }

bool getFlash(bool _constant) {
  bool _value = (s_gameState != kFadeIn && s_gameState != kFadeOut
    && s_frameCount % (ANIM_FPS/2) < ANIM_FPS/4
    && (m_dungeon.m_ticksInLevel < ANIM_FPS*2 || _constant));
  // if (_value && !_constant && s_frameCount % ANIM_FPS/2 == 0) vibes_short_pulse(); // TODO check this on device
  return _value;
}

void gameClickConfigHandler(ClickRecognizerRef _recognizer, void* _context) {
  if (getGameState() == kDisplayingMsg) setGameState(kLevelSpecific); // break out of message display
  if (getGameState() == kAwaitInput || getGameState() == kLevelSpecificWButtons) {
    ButtonId _button = click_recognizer_get_button_id(_recognizer);
    if (BUTTON_ID_UP == _button) s_playerChoice = 0;
    else if (BUTTON_ID_SELECT == _button) s_playerChoice = 1;
    else if (BUTTON_ID_DOWN == _button) s_playerChoice = 2;
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
  };
  ++m_dungeon.m_roomsVisited;
  m_dungeon.m_ticksTotal += m_dungeon.m_ticksInLevel;
  m_dungeon.m_ticksInLevel = 0;
  m_clutter.m_nClutter = 0;
  for (int _i = 0; _i < MAX_PLACE_CLUTTER; ++_i) m_clutter.m_position[_i] = GPoint(0,0);
  ++m_dungeon.m_seed;
  s_gameState = kDoInit;
  s_playerChoice = 1;
  #ifdef DEV
  APP_LOG(APP_LOG_LEVEL_WARNING,"ENTER %i [Give:%i val:%i] [Need:%i val:%i] ",
    m_dungeon.m_rooms[ m_dungeon.m_level ][ m_dungeon.m_room ],
    m_dungeon.m_roomGiveHint[ m_dungeon.m_level ][ m_dungeon.m_room ],
    m_dungeon.m_roomGiveHintValue[ m_dungeon.m_level ][ m_dungeon.m_room ],
    m_dungeon.m_roomNeedHint[ m_dungeon.m_level ][ m_dungeon.m_room ],
    m_dungeon.m_roomNeedHintValue[ m_dungeon.m_level ][ m_dungeon.m_room ]);
  #endif
  return false;
}

void gameLoop(void* data) {
  if (s_renderQueued == true) {
    s_gameLoopTimer = app_timer_register(ANIM_DELAY, gameLoop, NULL);
    return;
  }

  if (++s_frameCount == ANIM_FPS) s_frameCount = 0;
  bool requestRedraw = false;
  ++m_dungeon.m_ticksInLevel;
  #ifdef DEV
  if (s_frameCount == 0)  APP_LOG(APP_LOG_LEVEL_INFO,"f:%i GS:%i used:%i free:%i",s_frameCount, s_gameState, heap_bytes_used(), heap_bytes_free());
  #endif

  #ifdef DEBUG_MODE
  ++s_FPS;
  #endif

  //APP_LOG(APP_LOG_LEVEL_INFO,"GS: %i", (int) s_gameState);

  bool _doInit = false;
  switch (s_gameState) {
    case kIdle: break;
    case kNewRoom: requestRedraw = newRoom(); break;
    case kMovePlayer: requestRedraw = movePlayer(); break;
    case kAwaitInput: requestRedraw = true; //(s_frameCount == 0 || s_frameCount % ANIM_FPS/4 == 0 ? true : false); break; // TODO fix this with flashing intro objects
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
      case kPword: requestRedraw = tickPword(_doInit); break;
      case kBridge: requestRedraw = tickBridge(_doInit); break;
      case kMaths: requestRedraw = tickMaths(_doInit); break;
      case kStones: requestRedraw = tickStones(_doInit); break;
      case kDark: requestRedraw = tickDark(_doInit); break;
      case kMaze: requestRedraw = tickMaze(_doInit); break;
      case kSaw: requestRedraw = tickSaw(_doInit); break;
      #ifdef YCGBv2
        case kBomb: requestRedraw = tickBomb(_doInit); break;
        case kBoxes: requestRedraw = tickBoxes(_doInit); break;
      #endif
      case kDeath: requestRedraw = tickDeath(_doInit); break;
      case kFinal: requestRedraw = tickFinal(_doInit); break;
      case kEnd: requestRedraw = tickEnd(_doInit); break;
      default: break;
    } break;
    default: break;
  }

  #ifdef YCGBv2
    // Special case (bomb needs to be able to explode at any time)
    if (m_dungeon.m_rooms[ m_dungeon.m_level ][ m_dungeon.m_room ] == kBomb) {
      bombTimer();
      requestRedraw = true;
    }
  #endif

  if (requestRedraw == true) {
    layer_mark_dirty(s_dungeonLayer);
    s_renderQueued = true;
  }

  s_gameLoopTimer = app_timer_register(ANIM_DELAY, gameLoop, NULL);
}


void dungeonUpdateProc(Layer* _thisLayer, GContext* _ctx) {

  s_renderQueued = false;
  if (getGameState() == kIdle || getGameState() == kDoInit) return;
  srand(m_dungeon.m_seed);
  graphics_context_set_compositing_mode(_ctx, GCompOpSet);

  switch (m_dungeon.m_rooms[ m_dungeon.m_level ][ m_dungeon.m_room ]) {
    case kStart: updateProcStart(_ctx); break;
    case kStairs: updateProcStairs(_ctx); break;
    case kChest: updateProcChest(_ctx); break;
    case kEmpty: updateProcEmpty(_ctx); break;
    case kPword: updateProcPword(_ctx); break;
    case kBridge: updateProcBridge(_ctx); break;
    case kMaths: updateProcMaths(_ctx); break;
    case kStones: updateProcStones(_ctx); break;
    case kDark: updateProcDark(_ctx); break;
    case kMaze: updateProcMaze(_ctx); break;
    case kSaw: updateProcSaw(_ctx); break;
    #ifdef YCGBv2
      case kBomb: updateProcBomb(_ctx); break;
      case kBoxes: updateProcBoxes(_ctx); break;
    #endif
    case kDeath: updateProcDeath(_ctx); break;
    case kFinal: updateProcFinal(_ctx); break;
    case kEnd: updateProcEnd(_ctx); break;
    default: break;
  }

  //Do msg
  if (getGameState() == kDisplayMsg) {
    renderMessage(_ctx, s_displayMsg);
    setGameState(kDisplayingMsg);
  }

  if (m_dungeon.m_gameOver == 0) renderProgressBar(_thisLayer, _ctx);

  // Do fade
  if (s_gameState == kFadeIn) renderFade(_thisLayer, _ctx, true);
  else if (s_gameState == kFadeOut) renderFade(_thisLayer, _ctx, false);

  // On round and high res we need some masking borders
  #ifdef PBL_ROUND
  graphics_context_set_fill_color(_ctx, GColorBlack);
  GRect _b = layer_get_bounds(_thisLayer);
  graphics_fill_rect(_ctx, GRect(0,                        0, ROUND_OFFSET_X, _b.size.h), 0, GCornerNone);
  graphics_fill_rect(_ctx, GRect(_b.size.w-ROUND_OFFSET_X, 0, ROUND_OFFSET_X, _b.size.h), 0, GCornerNone);
  #elif defined HIGH_RES
  graphics_context_set_fill_color(_ctx, GColorBlack);
  GRect _b = layer_get_bounds(_thisLayer);
  graphics_fill_rect(_ctx, GRect(_b.size.w - 1, 0, 1, _b.size.h), 0, GCornerNone);
  #endif

  // Draw FPS indicator (dbg only)
  #ifdef DEBUG_MODE
  static char FPSBuffer[16];
  snprintf(FPSBuffer, 16, "%i/%i %i L:%i", m_dungeon.m_room, m_dungeon.m_level, s_lastSecondFPS, m_dungeon.m_lives);
#ifdef HIGH_RES
  GRect _fpsRect = GRect( 75, 210, 100, 15);
#else
  GRect _fpsRect = GRect( 50, 155, 100, 15);
#endif
  graphics_context_set_text_color(_ctx, GColorWhite);
  graphics_draw_text(_ctx, FPSBuffer, fonts_get_system_font(FONT_KEY_SMALL), _fpsRect, GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
  #endif
}

bool movePlayer() {
  APP_LOG(APP_LOG_LEVEL_INFO,"movePlayer target:%i,%i current:%i,%i",m_player.m_target.x, m_player.m_target.y, m_player.m_position.x, m_player.m_position.y); 
  if (s_frameCount % 3 == 0 && ++m_player.m_playerFrame == MAX_FRAMES) m_player.m_playerFrame = 0;
  for (uint8_t _step = 0; _step < PLAYER_SPEED; ++_step) {
    if      (m_player.m_target.x > m_player.m_position.x) ++m_player.m_position.x;
    if      (m_player.m_target.y > m_player.m_position.y) ++m_player.m_position.y;
    else if (m_player.m_target.y < m_player.m_position.y) --m_player.m_position.y;
    if (m_player.m_target.x == m_player.m_position.x && m_player.m_target.y == m_player.m_position.y) {
      m_player.m_playerFrame = 0;
      s_gameState = kLevelSpecific;
      return true;
    }
  }
  return true;
}


void gameWindowLoad(Window* _window) {
  GRect _b = layer_get_bounds( window_get_root_layer(_window) );
  setGameState(kIdle);

  s_dungeonLayer = layer_create( _b );
  layer_add_child(window_get_root_layer(_window), s_dungeonLayer);
  layer_set_update_proc(s_dungeonLayer, dungeonUpdateProc);

  generate();

  gameLoop(NULL);
  #ifdef DEBUG_MODE
  FPSTimer(NULL);
  #endif
}

void gameWindowUnload() {
  layer_destroy(s_dungeonLayer);
}

void gameClickConfigProvider(Window* _window) {
  window_single_click_subscribe(BUTTON_ID_UP, gameClickConfigHandler);
  window_single_click_subscribe(BUTTON_ID_DOWN, gameClickConfigHandler);
  window_single_click_subscribe(BUTTON_ID_SELECT, gameClickConfigHandler);
}

#ifdef DEBUG_MODE
void FPSTimer(void* data) {
  s_lastSecondFPS = s_FPS;
  s_FPS = 0;
  s_FPSTimer = app_timer_register(1000, FPSTimer, NULL);
}
#endif

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

#ifdef PBL_BW
  //case 0: return GColorBlack;
  //case 1: return GColorWhite;
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
#else
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
#endif

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
