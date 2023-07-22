#pragma once

#define DEBUG_MODE
#define DEV

#include "pd_api.h"

#include <stdio.h>
#include <stdbool.h> 

#define ANIM_FPS 20
#define ANIM_DELAY (1000/ANIM_FPS)

typedef enum {
  kStart,    // Special
  kStairs,   // Special
  kChest,    // Misc 0
  kEmpty,    // Misc 1
  kGamble,   // Misc 2
  //
  kPword,    // Recall 0
  kBridge,   // Recall 1
  kStones,   // Recall 2
  kBoxes,    // Recall 3
  kGreek,    // Recall 4
  //
  kMaze,     // Puzzle 0
  kMaths,    // Puzzle 1
  kShapes,   // Puzzle 2
  kPattern,  // Puzzle 3
  // Puzzle 4
  //
  kDark,     // Skill 0
  kSaw,      // Skill 1
  kBomb,     // Skill 2
  kSpikes,   // Skill 3
  kSBall,    // Skill 4
  //
  kShortcut, // Special
  kDeath,    // Special
  kFinal,    // Special
  kEnd,      // Special
  kNRoomTypes
} Rooms_t;

#define HINT_TYPES 4
#define MAX_SPELLS 31
#define MAX_GREEK 8
#define MAX_SYMBOL 4
#define MAX_NUMBER 43
#define MAX_SHIELD_COLOUR 3
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

typedef enum {
  kBWC,
  kBCW,
  kWCB,
  kWBC,
  kCBW,
  kCWB,
  kNShieldTypes
} ShieldTypes_t;

typedef enum {
  kCircle,
  kSquare,
  kTriangle,
  kNStoneTypes
} StoneTypes_t;

#define MAX_LEVELS 3
#define MIN_ROOMS 6
#define MAX_ROOMS 12
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
  bool m_spinningDeath;
  uint32_t m_seed;
  uint16_t m_ticksInLevel;
  uint16_t m_ticksTotal;
} Dungeon_t;

#define MAX_FRAMES 6
#define PLAYER_SPEED 2
typedef struct {
  uint16_t m_playerFrame;
  int16_t m_position_x;
  int16_t m_position_y;
  int16_t m_target_x;
  int16_t m_target_y;
  float m_rotation;
} Player_t;

#define MAX_PLACE_CLUTTER 5
typedef struct {
  int16_t m_nClutter;
  int16_t m_position_x[MAX_PLACE_CLUTTER];
  int16_t m_position_y[MAX_PLACE_CLUTTER];
} Clutter_t;

extern Dungeon_t m_dungeon;
extern Player_t m_player;
extern Clutter_t m_clutter;
void setPDPtr(PlaydateAPI* p);

int gameLoop(void* data);

int getFrameCount(void);
bool getFlash(bool _constant);
int getPlayerChoice(void);
void resetPlayerChoice(void);
GameState_t getGameState(void);
void setGameState(GameState_t _state);
void setDisplayMsg(const char* _msg);

extern const char* const m_spellNames[MAX_SPELLS];
int getHintValueMax(Hints_t hint);

int getShieldA(int _value);
int getShieldB(int _value);
int getShieldC(int _value);

void callbackReplacement(void);
void gameWindowLoad(void);
void gameWindowUnload(void);
void gameClickConfigHandler(uint32_t buttonPressed);
void clickHandlerReplacement(void);
bool atDestination(void);
bool movePlayer(void);
bool newRoom(void);
int getHorizontalOffset(void);
void dungeonUpdateProc(void);