#pragma once
#include <pebble.h>

#define DEBUG_MODE

#define ANIM_FPS 20
#define ANIM_DELAY (1000/ANIM_FPS)

typedef enum {
  kStart,
  kStairs,
  kWin,
  kLoose
} Levels_t;

typedef enum {
  kDagger,
  kAnkh,
  kPotion,
  kGold,
  kApple
} Items_t;

#define MAX_SPELLS 31
#define MAX_GREEK 8
typedef enum {
  kShield,
  kNumber,
  kSpell,
  kGreek
} Hints_t;

typedef enum {
  kIdle,
  kMovePlayer,
  kAwaitInput,
  kFadeOut,
  kNewRoom,
  kNewLevel,
  kFadeIn,
  kLevelSpecific
} GameState_t;

#define MAX_LEVELS 3
#define MIN_ROOMS 8
#define MAX_ROOMS 16
typedef struct {
  int16_t m_level;
  int16_t m_room;
  int8_t m_roomPerLevel[MAX_LEVELS];
  int8_t m_rooms[MAX_LEVELS][MAX_ROOMS];
  uint32_t m_seed;
} Dungeon_t;

#define MAX_FRAMES 6
#define PLAYER_SPEED 2
typedef struct {
  uint16_t m_frame;
  //bool m_walking;
  GPoint m_position;
  GPoint m_target;
} Player_t;

extern Dungeon_t m_dungeon;
extern Player_t m_player;
extern GameState_t m_gameState;
extern int m_frame;

extern const char* const m_spellNames[MAX_SPELLS];

void generate();

void gameWindowLoad(Window* _window);
void gameWindowUnload();
void gameClickConfigProvider(Window* _window);
