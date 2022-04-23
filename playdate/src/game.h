#pragma once
#include <pebble.h>

//#define DEBUG_MODE
//#define DEV

#define ANIM_FPS 20
#define ANIM_DELAY (1000/ANIM_FPS)

#define ROUND_OFFSET_X 18
#define ROUND_OFFSET_Y 10

#define PERSIST_KEY_VICTORY 0

typedef enum {
  kStart,
  kStairs,
  kChest,
  kEmpty,
  kPword,
  kBridge,
  kMaze,
  kMaths,
  kStones,
  kDark,
  kSaw,
  kDeath,
  kFinal,
  kEnd,
  kNRoomTypes
} Rooms_t;

#define HINT_TYPES 4
#define MAX_SPELLS 31
#define MAX_GREEK 8
#define MAX_SYMBOL 4
#define MAX_NUMBER 43
#define MAX_SHIELD_COLOUR 4
typedef enum {
  kNoHint,
  kSpell,
  kSymbol,
  kShield,
  kNumber,
  kGreek,
  kNHintTypes
} Hints_t;

typedef enum {
  kIdle = 0,
  kMovePlayer = 1,
  kAwaitInput = 2,
  kFadeOut = 3,
  kNewRoom = 4,
  kFadeIn = 5,
  kLevelSpecific = 6,
  kDisplayMsg = 7,
  kDisplayingMsg = 8,
  kDoInit = 9,
  kLevelSpecificWButtons = 10
} GameState_t;

typedef enum {
  kRWb,
  kRbB,
  kbBb,
  kbRW,
  kBbR,
  kBRR,
  kNShieldTypes
} ShieldTypes_t;
//RGBWb
//https://www.random.org/integer-sets/?sets=10&num=3&min=1&max=5&commas=on&order=random&format=html&rnd=new

#define MAX_LEVELS 3
#define MIN_ROOMS 5
#define MAX_ROOMS 10
#define MIN_ROOM_SEP 5
typedef struct {
  int8_t m_level;
  int8_t m_room;
  int8_t m_lives;
  int8_t m_totalRooms;
  int8_t m_roomsVisited;
  int8_t m_roomsPerLevel[MAX_LEVELS];
  int8_t m_rooms[MAX_LEVELS][MAX_ROOMS];
  int8_t m_roomGiveHint[MAX_LEVELS][MAX_ROOMS];
  int8_t m_roomGiveHintValue[MAX_LEVELS][MAX_ROOMS];
  int8_t m_roomNeedHint[MAX_LEVELS][MAX_ROOMS];
  int8_t m_roomNeedHintValue[MAX_LEVELS][MAX_ROOMS];
  int8_t m_finalPuzzle;
  int8_t m_gameOver;
  bool m_fallingDeath;
  uint32_t m_seed;
} Dungeon_t;

#define MAX_FRAMES 6
#define PLAYER_SPEED 2
typedef struct {
  uint16_t m_playerFrame;
  GPoint m_position;
  GPoint m_target;
} Player_t;

#define MAX_PLACE_CLUTTER 5
typedef struct {
  int16_t m_nClutter;
  GPoint m_position[MAX_PLACE_CLUTTER];
} Clutter_t;

extern Dungeon_t m_dungeon;
extern Player_t m_player;
extern Clutter_t m_clutter;

int getFrameCount();
int getPlayerChoice();
GameState_t getGameState();
void setGameState(GameState_t _state);
void setDisplayMsg(const char* _msg);

extern const char* const m_spellNames[MAX_SPELLS];
int getHintValueMax(Hints_t hint);

int getShieldA(int _value);
int getShieldB(int _value);
int getShieldC(int _value);

void gameWindowLoad(Window* _window);
void gameWindowUnload();
void gameClickConfigProvider(Window* _window);
