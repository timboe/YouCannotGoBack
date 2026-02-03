#pragma once
#include <pebble.h>

// #define DEBUG_MODE
#define DEV

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

#if PBL_DISPLAY_WIDTH >= 200
  #define HIGH_RES 1
  #define FONT_KEY_SMALL FONT_KEY_GOTHIC_18_BOLD
  #define FONT_KEY_LARGE FONT_KEY_GOTHIC_28_BOLD
  #define PLAYER_SPEED 3
#else
  #define FONT_KEY_SMALL FONT_KEY_GOTHIC_14_BOLD
  #define FONT_KEY_LARGE FONT_KEY_GOTHIC_24_BOLD
  #define PLAYER_SPEED 2
#endif

// B&W graphics save enough memory for more levels! Or double the RAM, that helps too
#if defined(PBL_PLATFORM_FLINT) || defined(PBL_PLATFORM_EMERY) || defined(PBL_PLATFORM_DIORITE)
  #define YCGBv2 1
#endif

#define ANIM_FPS 20
#define ANIM_DELAY (1000/ANIM_FPS)

#define ROUND_OFFSET_X 18
#define ROUND_OFFSET_Y 10

#define EMERY_OFFSET_X 1
#define EMERY_OFFSET_Y 0

#define PERSIST_KEY_VICTORY 0

typedef enum {
  kStart,     // Special
  kStairs,    // Special
  kChest,     // Misc 0
  kEmpty,     // Misc 1
  #ifdef YCGBv2
    kGamble,  // Misc 2
  #endif
  //
  kPword,     // Recall 0
  kBridge,    // Recall 1
  kStones,    // Recall 2
  #ifdef YCGBv2
    kBoxes,   // Recall 3
    kGreek,   // Recall 4
  #endif
  //
  kMaze,       // Puzzle 0
  kMaths,      // Puzzle 1
  #ifdef YCGBv2
    // kShapes,   // Puzzle 2
    // kPattern,  // Puzzle 3
    kArrows,   // Puzzle 4
  #endif
  //
  kDark,     // Skill 0
  kSaw,      // Skill 1
  #ifdef YCGBv2
    kBomb,   // Skill 2
    // kSpikes, // Skill 3
    // kSBall,  // Skill 4
  #endif
  //
  #ifdef YCGBv2
    kShortcut, // Special
  #endif
  kDeath,      // Special
  kFinal,      // Special
  kEnd,        // Special
  kNRoomTypes
} Rooms_t;

#define HINT_TYPES 4
#define MAX_SPELLS 31
#define MAX_GREEK 8
#define MAX_SYMBOL 4
#define MAX_NUMBER 43
#ifdef PBL_BW
  #define MAX_SHIELD_COLOUR 3
#else
  #define MAX_SHIELD_COLOUR 4
#endif

#define TOTAL_LETTERS 5

typedef enum {
  kNoHint,
  kSpell,
  kSymbol,
  kShield,
  kNumber,
  kGreekLetter,
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

#ifdef PBL_BW
  typedef enum {
    kBWC,
    kBCW,
    kWCB,
    kWBC,
    kCBW,
    kCWB,
    kNShieldTypes
  } ShieldTypes_t;
#else
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
#endif

typedef enum {
  kN,
  kE,
  kS,
  kW,
  kNE,
  kSE,
  kSW,
  kNW,
  kNOptionTypes
} Options_t;

typedef enum {
  kClover,
  kUnlockShortcut,
  kGiveClue,
  kDifficultyUp,
  kEvilWind,
  kInstantDeath,
  kNGambleTypes
} GambleOutcomes_t;

#define MAX_LEVELS 3
#define MIN_ROOMS 5
#define MAX_ROOMS 10
#define MIN_ROOM_SEP 5
typedef struct {
  int8_t m_level;
  int8_t m_room;
  int8_t m_lives;
  int8_t m_difficulty;
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
  int8_t m_fallingDeath;
  uint32_t m_seed;
  uint16_t m_ticksInLevel;
  uint16_t m_ticksTotal;
} Dungeon_t;

#define MAX_FRAMES 6
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

bool getFlash(bool _constant);

extern const char* const m_spellNames[MAX_SPELLS];
int getHintValueMax(Hints_t hint);

int getShieldA(int _value);
int getShieldB(int _value);
int getShieldC(int _value);

void gameWindowLoad(Window* _window);
void gameWindowUnload();
void gameClickConfigProvider(Window* _window);
