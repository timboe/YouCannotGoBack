#include "game.h"
#include "sprite.h"
#include "generate.h"
#include "render.h"
#include "levels/start.h"
#include "levels/stairs.h"
#include "levels/pword.h"
#include "levels/bridge.h"
#include "levels/maze.h"

static int s_frameCount = 0;
Dungeon_t m_dungeon = {0};
Player_t m_player = {0};
static GameState_t s_gameState = kIdle;
static int s_playerChoice = 0;
static const char* s_displayMsg = NULL;
Clutter_t m_clutter = {0};

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

void gameClickConfigHandler(ClickRecognizerRef _recognizer, void* _context) {
  if (getGameState() == kDisplayingMsg) setGameState(kLevelSpecific); // break out of message display
  if (getGameState() != kAwaitInput) return;
  ButtonId _button = click_recognizer_get_button_id(_recognizer);
  if (BUTTON_ID_UP == _button) s_playerChoice = 0;
  else if (BUTTON_ID_SELECT == _button) s_playerChoice = 1;
  else if (BUTTON_ID_DOWN == _button) s_playerChoice = 2;
  setGameState(kLevelSpecific);
}

bool newRoom() {
  if ( ++m_dungeon.m_room == m_dungeon.m_roomsPerLevel[ m_dungeon.m_level ] ) {
    ++m_dungeon.m_level;
    m_dungeon.m_room = 0;
  };
  m_clutter.m_nClutter = 0;
  ++m_dungeon.m_seed;
  s_gameState = kDoInit;
  return false;
}

void gameLoop(void* data) {
  if (++s_frameCount == ANIM_FPS) s_frameCount = 0;
  bool requestRedraw = false;
  if (s_frameCount == 0)  APP_LOG(APP_LOG_LEVEL_INFO,"f:%i GS:%i used:%i free:%i",s_frameCount, s_gameState, heap_bytes_used(), heap_bytes_free());

  #ifdef DEBUG_MODE
  ++s_FPS;
  #endif

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
    case kLevelSpecific:
    switch (m_dungeon.m_rooms[ m_dungeon.m_level ][ m_dungeon.m_room ]) {
      case kStart: requestRedraw = tickStart(_doInit); break;
      case kStairs: requestRedraw = tickStairs(_doInit); break;
      case kPword: requestRedraw = tickPword(_doInit); break;
      case kBridge: requestRedraw = tickBridge(_doInit); break;
      case kMaze: requestRedraw = tickMaze(_doInit); break;
      default: break;
    } break;
    default: APP_LOG(APP_LOG_LEVEL_INFO,"GS:%i ???",(int) s_gameState); break;
  }

  if (requestRedraw == true) layer_mark_dirty(s_dungeonLayer);

  s_gameLoopTimer = app_timer_register(ANIM_DELAY, gameLoop, NULL);
}


void dungeonUpdateProc(Layer* _thisLayer, GContext* _ctx) {
  if (getGameState() == kIdle) return;
  srand(m_dungeon.m_seed);
  graphics_context_set_compositing_mode(_ctx, GCompOpSet);

  switch (m_dungeon.m_rooms[ m_dungeon.m_level ][ m_dungeon.m_room ]) {
    case kStart: updateProcStart(_ctx); break;
    case kStairs: updateProcStairs(_ctx); break;
    case kPword: updateProcPword(_ctx); break;
    case kBridge: updateProcBridge(_ctx); break;
    case kMaze: updateProcMaze(_ctx); break;
    default: break;
  }

  //Do msg
  if (getGameState() == kDisplayMsg) {
    renderMessage(_ctx, s_displayMsg);
    setGameState(kDisplayingMsg);
  }

  // Do fade
  if (s_gameState == kFadeIn) renderFade(_thisLayer, _ctx, true);
  else if (s_gameState == kFadeOut) renderFade(_thisLayer, _ctx, false);

  // Draw FPS indicator (dbg only)
  #ifdef DEBUG_MODE
  static char FPSBuffer[5];
  snprintf(FPSBuffer, 10, "%i/%i %i", m_dungeon.m_room, m_dungeon.m_level, s_lastSecondFPS);
  GRect _fpsRect = GRect( 80, 155, 100, 15);
  graphics_context_set_text_color(_ctx, GColorWhite);
  graphics_draw_text(_ctx, FPSBuffer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), _fpsRect, GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
  #endif
}

bool movePlayer() {
  if (s_frameCount % 3 == 0 && ++m_player.m_playerFrame == MAX_FRAMES) m_player.m_playerFrame = 0;
  //APP_LOG(APP_LOG_LEVEL_INFO,"F:%i PF:%i",s_frameCount, m_player.m_playerFrame);

  if      (m_player.m_target.x > m_player.m_position.x) m_player.m_position.x += PLAYER_SPEED;
  if      (m_player.m_target.y > m_player.m_position.y) m_player.m_position.y += PLAYER_SPEED;
  else if (m_player.m_target.y < m_player.m_position.y) m_player.m_position.y -= PLAYER_SPEED;
  if (m_player.m_target.x == m_player.m_position.x && m_player.m_target.y == m_player.m_position.y) {
    m_player.m_playerFrame = 0;
    s_gameState = kLevelSpecific;
  }
  return true;
}


void gameWindowLoad(Window* _window) {
  GRect _b = layer_get_bounds( window_get_root_layer(_window) );

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
    case kShield: return rand() % kNShieldTypes;
    case kNumber: return rand() % MAX_NUMBER;
    case kSpell: return rand() % MAX_SPELLS;
    case kGreek: return rand() % MAX_GREEK;
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
