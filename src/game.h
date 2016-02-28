#pragma once
#include <pebble.h>

#define DEBUG_MODE

#define ANIM_FPS 20
#define ANIM_DELAY (1000/ANIM_FPS)


typedef enum {
  kStart,
  kStairs,
  kWin,
  kLoose,
  kNRoomTypes
} Rooms_t;

typedef enum {
  kNoItem,
  kDagger,
  kAnkh,
  kPotion,
  kGold,
  kApple,
  kNItemTypes
} Items_t;

#define HINT_TYPES 4
#define MAX_SPELLS 31
#define MAX_GREEK 8
#define MAX_NUMBER 43
typedef enum {
  kNoHint,
  kShield,
  kNumber,
  kSpell,
  kGreek,
  kNHintTypes
} Hints_t;

typedef enum {
  kIdle,
  kMovePlayer,
  kAwaitInput,
  kFadeOut,
  kNewRoom,
  kFadeIn,
  kLevelSpecific
} GameState_t;

typedef enum {
  kRWb,
  kbRW,
  kBbR,
  kbBG,
  kbWG,
  kGRb,
  kBWG,
  kRbG,
  kGbB,
  kbRB,
  kNShieldTypes
} ShieldTypes_t;
//RGBWb
//https://www.random.org/integer-sets/?sets=10&num=3&min=1&max=5&commas=on&order=random&format=html&rnd=new

#define MAX_LEVELS 3
#define MIN_ROOMS 5
#define MAX_ROOMS 8
typedef struct {
  int8_t m_level;
  int8_t m_room;
  int8_t m_lives;
  int8_t m_roomsPerLevel[MAX_LEVELS];
  int8_t m_rooms[MAX_LEVELS][MAX_ROOMS];
  int8_t m_roomGiveItem[MAX_LEVELS][MAX_ROOMS];
  int8_t m_roomGiveHint[MAX_LEVELS][MAX_ROOMS];
  int8_t m_roomGiveHintValue[MAX_LEVELS][MAX_ROOMS];
  int8_t m_roomNeedHint[MAX_LEVELS][MAX_ROOMS];
  int8_t m_roomNeedHintValue[MAX_LEVELS][MAX_ROOMS];
  int8_t m_roomNeedItem[MAX_LEVELS][MAX_ROOMS];
  uint32_t m_seed;
} Dungeon_t;

#define MAX_FRAMES 6
#define PLAYER_SPEED 2
typedef struct {
  uint16_t m_frame;
  GPoint m_position;
  GPoint m_target;
} Player_t;

#define MAX_PLACE_CLUTTER 5
typedef struct {
  int16_t m_nClutter;
  GPoint m_position[MAX_PLACE_CLUTTER];
} Clutter_t;


// typedef struct {
//   int8_t m_nHints;
//   int8_t m_hintActive[HINT_TYPES]
//   int8_t m_hintValue[HINT_TYPES]
// } HintStore_t;

// Note changing MAX_INVENTORY has big effects in generate.c, check the TODOs
#define MAX_INVENTORY 2
typedef struct {
  int8_t m_nItems;
  int8_t m_itemValue[kNItemTypes];
} ItemStore_t;


extern Dungeon_t m_dungeon;
extern Player_t m_player;
extern GameState_t m_gameState;
extern Clutter_t m_clutter;
extern ItemStore_t m_items;
extern int m_frame;

extern const char* const m_spellNames[MAX_SPELLS];



void gameWindowLoad(Window* _window);
void gameWindowUnload();
void gameClickConfigProvider(Window* _window);
