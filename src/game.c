#include "game.h"
#include "sprite.h"
#include "generate.h"
#include "levels/start.h"
#include "levels/stairs.h"

Dungeon_t m_dungeon = {0};
Player_t m_player = {0};
GameState_t m_gameState = kIdle;
Clutter_t m_clutter = {0};
ItemStore_t m_items = {0};
int m_frame = 0;

static AppTimer* s_gameLoopTimer = NULL;
static Layer* s_dungeonLayer;
bool movePlayer();

#ifdef DEBUG_MODE
static AppTimer* s_FPSTimer = NULL;
static int s_FPS = 0;
static int s_lastSecondFPS = 0;
void FPSTimer(void* data);
#endif

void gameClickConfigHandler(ClickRecognizerRef _recognizer, void* _context) {
  if (m_gameState != kAwaitInput) return;
  ButtonId _button = click_recognizer_get_button_id(_recognizer);
  switch (m_dungeon.m_rooms[ m_dungeon.m_level ][ m_dungeon.m_room ]) {
    case kStart: clickStart(_button); break;
    case kStairs: clickStairs(_button); break;
    default: break;
  }
}

bool newRoom() {
  if ( ++m_dungeon.m_room == m_dungeon.m_roomsPerLevel[ m_dungeon.m_level ] ) {
    ++m_dungeon.m_level;
    m_dungeon.m_room = 0;
  };
  m_clutter.m_nClutter = 0;
  switch (m_dungeon.m_rooms[ m_dungeon.m_level ][ m_dungeon.m_room ]) {
    case kStart: initStart(); break;
    case kStairs: initStairs(); break;
    default: break;
  }
  m_gameState = kFadeIn;
  return false;
}

void gameLoop(void* data) {
  if (++m_frame == ANIM_FPS) m_frame = 0;
  bool requestRedraw = false;
  if (m_frame == 0) APP_LOG(APP_LOG_LEVEL_INFO,"used:%i free:%i",heap_bytes_used(), heap_bytes_free());

  #ifdef DEBUG_MODE
  ++s_FPS;
  #endif

  switch (m_gameState) {
    case kIdle: break;
    case kNewRoom: requestRedraw = newRoom(); break;
    case kMovePlayer: requestRedraw = movePlayer(); break;
    case kAwaitInput: requestRedraw = (m_frame == 0 || m_frame == ANIM_FPS/2 ? true : false);
    case kFadeIn: case kFadeOut: requestRedraw = true; break;
    case kLevelSpecific:
    switch (m_dungeon.m_rooms[ m_dungeon.m_level ][ m_dungeon.m_room ]) {
      case kStart: tickStart(); break;
      case kStairs: tickStairs(); break;
      default: break;
    }
    default: break;
  }

  if (requestRedraw == true) layer_mark_dirty(s_dungeonLayer);

  s_gameLoopTimer = app_timer_register(ANIM_DELAY, gameLoop, NULL);
}


void dungeonUpdateProc(Layer* _thisLayer, GContext* _ctx) {
  srand(m_dungeon.m_seed);
  graphics_context_set_compositing_mode(_ctx, GCompOpSet);

  switch (m_dungeon.m_rooms[ m_dungeon.m_level ][ m_dungeon.m_room ]) {
    case kStart: updateProcStart(_thisLayer, _ctx); break;
    case kStairs: updateProcStairs(_thisLayer, _ctx); break;
    default: break;
  }

  // Do fade
  if (m_gameState == kFadeIn) renderFade(_thisLayer, _ctx, true);
  else if (m_gameState == kFadeOut) renderFade(_thisLayer, _ctx, false);

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
  if (m_frame % 3 == 0 && ++m_player.m_frame == MAX_FRAMES) m_player.m_frame = 0;

  if      (m_player.m_target.x > m_player.m_position.x) m_player.m_position.x += PLAYER_SPEED;
  if      (m_player.m_target.y > m_player.m_position.y) m_player.m_position.y += PLAYER_SPEED;
  else if (m_player.m_target.y < m_player.m_position.y) m_player.m_position.y -= PLAYER_SPEED;
  if (m_player.m_target.x == m_player.m_position.x && m_player.m_target.y == m_player.m_position.y) {
    m_player.m_frame = 0;
    m_gameState = kLevelSpecific;
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
