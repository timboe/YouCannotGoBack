#include "game.h"
#include "generate.h"
#include "sound.h"

//Hints Format   kNoHint, kSpell, kSymbol, kShield, kNumber, kGreek, kNHintTypes
// kNoHint is currently unused, any other bit true will indicate that reqHint is true
// Set levels which are not to be chosen at random to minL = 9
RoomDescriptor_t m_roomDescriptor[kNRoomTypes] = {
 [kStart].m_minL    = 9, [kStart].m_giveHint    = 1, [kStart].m_reqHint    = {0, 0, 0, 0, 0, 0},
 [kStairs].m_minL   = 9, [kStairs].m_giveHint   = 1, [kStairs].m_reqHint   = {0, 0, 0, 0, 0, 0},
 [kChest].m_minL    = 0, [kChest].m_giveHint    = 1, [kChest].m_reqHint    = {0, 0, 0, 0, 0, 0},
 [kEmpty].m_minL    = 0, [kEmpty].m_giveHint    = 1, [kEmpty].m_reqHint    = {0, 0, 0, 0, 0, 0},
 [kGamble].m_minL   = 0, [kGamble].m_giveHint   = 1, [kGamble].m_reqHint   = {0, 0, 0, 0, 0, 0},
 [kShortcut].m_minL = 9, [kShortcut].m_giveHint = 0, [kShortcut].m_reqHint = {0, 0, 0, 0, 0, 0},
 [kPword].m_minL    = 0, [kPword].m_giveHint    = 1, [kPword].m_reqHint    = {0, 1, 0, 0, 1, 0},
 [kBridge].m_minL   = 1, [kBridge].m_giveHint   = 1, [kBridge].m_reqHint   = {0, 0, 1, 0, 0, 1},
 [kGreek].m_minL    = 0, [kGreek].m_giveHint    = 1, [kGreek].m_reqHint    = {0, 0, 0, 0, 0, 1},
 [kMaths].m_minL    = 0, [kMaths].m_giveHint    = 0, [kMaths].m_reqHint    = {0, 0, 0, 0, 0, 0},
 [kStones].m_minL   = 1, [kStones].m_giveHint   = 0, [kStones].m_reqHint   = {0, 0, 0, 1, 0, 0},
 [kDark].m_minL     = 0, [kDark].m_giveHint     = 0, [kDark].m_reqHint     = {0, 0, 0, 0, 0, 0},
 [kPattern].m_minL  = 0, [kPattern].m_giveHint  = 0, [kPattern].m_reqHint  = {0, 0, 0, 0, 0, 0},
 [kSaw].m_minL      = 0, [kSaw].m_giveHint      = 0, [kSaw].m_reqHint      = {0, 0, 0, 0, 0, 0},
 [kSpikes].m_minL   = 0, [kSpikes].m_giveHint   = 1, [kSpikes].m_reqHint   = {0, 0, 0, 0, 0, 0},
 [kSBall].m_minL    = 0, [kSBall].m_giveHint    = 0, [kSBall].m_reqHint    = {0, 0, 0, 0, 0, 0},
 [kShapes].m_minL   = 0, [kShapes].m_giveHint   = 1, [kShapes].m_reqHint   = {0, 0, 0, 0, 0, 0},
 [kBomb].m_minL     = 0, [kBomb].m_giveHint     = 1, [kSaw].m_reqHint      = {0, 0, 0, 0, 0, 0},
 [kBoxes].m_minL    = 0, [kBoxes].m_giveHint    = 0, [kBoxes].m_reqHint    = {0, 0, 0, 1, 0, 0},
 [kMaze].m_minL     = 0, [kMaze].m_giveHint     = 0, [kMaze].m_reqHint     = {0, 0, 0, 0, 0, 0},
 [kArrows].m_minL   = 0, [kArrows].m_giveHint   = 0, [kArrows].m_reqHint   = {0, 0, 0, 0, 0, 0},
 [kDeath].m_minL    = 9, [kDeath].m_giveHint    = 0, [kDeath].m_reqHint    = {0, 0, 0, 0, 0, 0},
 [kFinal].m_minL    = 9, [kFinal].m_giveHint    = 0, [kFinal].m_reqHint    = {0, 0, 0, 0, 0, 0},
 [kEnd].m_minL      = 9, [kEnd].m_giveHint      = 0, [kEnd].m_reqHint      = {0, 0, 0, 0, 0, 0}
};
 
int m_hintsInPlay = 0;
uint8_t m_hintIsActive[kNHintTypes] = {0};
uint8_t m_hintValue[kNHintTypes] = {0};

Hints_t getHint(int _level, Rooms_t _roomType) {
  if (m_roomDescriptor[_roomType].m_giveHint == 0) return kNoHint; // Room does not allow hints
  if (m_hintsInPlay == HINT_TYPES) return kNoHint; // Have max hints in play
  if (rand() % 100 >= HINT_CHANCE) return kNoHint; // rnd

  // Else choose a hint
  Hints_t _hint = kNoHint;
  while (_hint == kNoHint) {
    Hints_t _random = rand() % kNHintTypes;
    if ( m_hintIsActive[_random] == false ) _hint = _random;
  }

  return _hint;
}

// #define TESTING 1

Rooms_t getRoom(int _level, int _room, Hints_t* _consumeHint, bool* _consumeItem) {

  int _spin = 0;
  while (true) {
    Rooms_t _newRoom;
    if (_level == 0 && _room == 0) { // First room

      _newRoom = kStart;
      //_newRoom = kPattern;
      //m_dungeon.m_difficulty = 2; // TESTING
      //m_dungeon.m_lives = 0; // TESTING

    // } else if (TESTING && _level == 0 && _room == 1) {
    //   _newRoom = kSpikes; // TESTING
    // } else if (TESTING && _level == 0 && _room == 2) {
    //   _newRoom = kArrows; // TESTING
    // } else if (TESTING && _level == 0 && _room == 3) {
    //   _newRoom = kPword; // TESTING
    //   (*_consumeHint) = kSpell; // TESTING
    // } else if (TESTING && _level == 0 && _room == 4) {
    //   _newRoom = kGamble; // TESTING

    } else if (_level == (MAX_LEVELS - 1) && _room == m_dungeon.m_roomsPerLevel[_level] - 1) { // End of game
      _newRoom = kFinal;
    } else if (_room == m_dungeon.m_roomsPerLevel[_level] - 1) { // End of floor
      _newRoom = kStairs;
    } else {
      _newRoom = rand() % kNRoomTypes;
      if (m_roomDescriptor[_newRoom].m_minL > _level) continue;  // Check level
    }

    bool _veto = false;
    for (int _c = 1; _c <= m_dungeon.m_roomsPerLevel[_level]; ++_c) {     // cycle rooms
      if (_room >= _c && m_dungeon.m_rooms[_level][_room - _c] == _newRoom) _veto = true;
    }
    if (_veto == true && ++_spin < 100) continue; // The "spin" var prevents us from having zero available rooms

    // Check if needs hint
    bool _needsHint = false;
    int _nCompatableHints = 0;
    int _hints[kNHintTypes] = {0};
    for (Hints_t _h = 1; _h < kNHintTypes; ++_h) { // Avoid kNoHint
      if ( m_roomDescriptor[_newRoom].m_reqHint[ _h ] == 1 ) _needsHint = true; // I require a hint
      if ( m_roomDescriptor[_newRoom].m_reqHint[ _h ] == 1 && m_hintIsActive[ _h ] == 1) { // This hint is available
        _hints[ _nCompatableHints++ ] = _h;
      }
    }
    if (_needsHint == true && _nCompatableHints == 0) continue; // Roll again
    else if (_needsHint == true) (*_consumeHint) = _hints[ rand() % _nCompatableHints ]; // Consume an available hint

    return _newRoom;
  }
}

void generate(PlaydateAPI* _pd) {

  memset(&m_dungeon, 0, sizeof(Dungeon_t));
  memset(&m_player, 0, sizeof(Player_t));
  m_dungeon.m_seed = _pd->system->getSecondsSinceEpoch(NULL);
  srand(m_dungeon.m_seed);
  m_dungeon.m_finalPuzzle = rand() % 3;
  m_hintsInPlay = 0;
  for (int _i = 0; _i < kNHintTypes; ++_i) {
    m_hintIsActive[_i] = 0;
  }
  m_dungeon.m_room = -1; // Will be incremented on kNewLevel
  m_dungeon.m_lives = 0; // New for 2.0 - don't start with a life, now an extra way to get one
  updateMusic(0); // Reset
  stopSounds(); // Reset

  #ifdef DEV
  _pd->system->logToConsole("win:%i, seed:%i", m_dungeon.m_finalPuzzle, (int)m_dungeon.m_seed);
  #endif

  // Now we have high-scores, keep the total number of rooms consistent
  #define TOT_ROOMS 35
  #define ROOM_MIN 11
  #define ROOM_VAR 3
  // Level 0, 1: 10-12 rooms
  // Level 2: 11-15 (deliberate skew)

  m_dungeon.m_totalRooms = TOT_ROOMS;

  m_dungeon.m_roomsPerLevel[0] = ROOM_MIN - ((ROOM_VAR - 1)/2) + (rand() % ROOM_VAR);
  m_dungeon.m_roomsPerLevel[1] = ROOM_MIN - ((ROOM_VAR - 1)/2) + (rand() % ROOM_VAR);
  m_dungeon.m_roomsPerLevel[2] = TOT_ROOMS - m_dungeon.m_roomsPerLevel[1] - m_dungeon.m_roomsPerLevel[0];

  for (int _level = 0; _level < MAX_LEVELS; ++_level) {
    if (m_dungeon.m_roomsPerLevel[_level] >= MAX_ROOMS) {
      _pd->system->error("Rooms %i greater than max %i", m_dungeon.m_roomsPerLevel[_level], MAX_ROOMS);
      return;
    }

    #ifdef DEV
    _pd->system->logToConsole(" -- L%i R%i", _level, m_dungeon.m_roomsPerLevel[_level]);
    #endif
    for (int _room = 0; _room < m_dungeon.m_roomsPerLevel[_level]; ++_room) {

      Hints_t _consumeHint = kNoHint;
      bool _consumeItem = false;
      Rooms_t _roomType = getRoom(_level, _room, &_consumeHint, &_consumeItem);
      m_dungeon.m_rooms[_level][_room] = _roomType;

      #ifdef DEV
      _pd->system->logToConsole("[%i][%i] = t:%i", _level, _room, (int)_roomType);
      #endif

      // Can we add a hint to this room?
      Hints_t _addHint = getHint(_level, _roomType);

      // if (TESTING && _level == 0 && _room == 1) {
      //   _addHint = kSpell; // TESTING
      // } else if (TESTING && _level == 0 && _room == 4) {
      //   _addHint = kShield; // TESTING
      // }

      if (_addHint != kNoHint) {
        ++m_hintsInPlay;
        m_hintIsActive[_addHint] = 1;
        m_hintValue[_addHint] = rand() % getHintValueMax(_addHint);

        m_dungeon.m_roomGiveHint[_level][_room] = _addHint;
        m_dungeon.m_roomGiveHintValue[_level][_room] = m_hintValue[_addHint];
        #ifdef DEV
        _pd->system->logToConsole("      >> A t:%i v:%i", (int)_addHint, (int)m_hintValue[_addHint]);
        #endif
      }

      // Are we consuming a hint?
      if (_consumeHint != kNoHint) {
        m_dungeon.m_roomNeedHint[_level][_room] = _consumeHint;
        m_dungeon.m_roomNeedHintValue[_level][_room] = m_hintValue[_consumeHint];
        #ifdef DEV
        _pd->system->logToConsole("      << C t:%i v:%i", (int)_consumeHint, (int)m_hintValue[_consumeHint]);
        #endif

        --m_hintsInPlay;
        m_hintIsActive[_consumeHint] = 0;
        m_hintValue[_consumeHint] = 0;
      }

      // Special for CHEST/GAMBLE room, REMINDER
      // By increasing the number on the right, we make it less likely that the player
      // will get an extra life once they have been given a hint (chest only)
      bool canGiveHint = m_hintIsActive[kSpell] || m_hintIsActive[kNumber];
      canGiveHint &= (_roomType == kChest && rand() % 6) || _roomType == kGamble;
      if (canGiveHint) {
        if (m_hintIsActive[kSpell] == true) {
          m_dungeon.m_roomNeedHint[_level][_room] = kSpell;
          m_dungeon.m_roomNeedHintValue[_level][_room] = m_hintValue[kSpell];
        } else {
          m_dungeon.m_roomNeedHint[_level][_room] = kNumber;
          m_dungeon.m_roomNeedHintValue[_level][_room] = m_hintValue[kNumber];
        }
      }
    }
  }
  setGameState(kNewRoom);
}
