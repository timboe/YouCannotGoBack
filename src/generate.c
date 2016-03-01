#include "game.h"
#include "generate.h"

//Hints Format [0]kNoHint [1]kShield [2]kNumber [3]kSpell [4]kGreek
// kNoHint is currently unused, any other bit true will indicate that reqHint is true
// Set levels which are not to be chosen at random to minL = 9
RoomDescriptor_t m_roomDescriptor[kNRoomTypes] = {
 [kStart].m_minL  = 0, [kStart].m_giveHint  = 1, [kStart].m_reqHint  = {0, 0, 0, 0, 0},
 [kStairs].m_minL = 0, [kStairs].m_giveHint = 1, [kStairs].m_reqHint = {0, 0, 0, 0, 0},
 [kPword].m_minL  = 0, [kPword].m_giveHint  = 1, [kPword].m_reqHint  = {0, 0, 1, 1, 0},
 [kBridge].m_minL = 0, [kBridge].m_giveHint = 1, [kBridge].m_reqHint = {0, 0, 1, 0, 1},
 [kMaze].m_minL   = 0, [kMaze].m_giveHint   = 1, [kMaze].m_reqHint   = {0, 0, 0, 0, 0},
 [kEnd].m_minL    = 9, [kEnd].m_giveHint    = 1, [kEnd].m_reqHint    = {0, 0, 0, 0, 0}
};

int m_hintsInPlay = 0;
uint8_t m_hintIsActive[kNHintTypes] = {0};
uint8_t m_hintValue[kNHintTypes] = {0};

Hints_t getHint(Rooms_t _room) {
  if (m_roomDescriptor[_room].m_giveHint == 0) return kNoHint; // Room does not allow hints
  if (m_hintsInPlay == HINT_TYPES) return kNoHint; // Have max hints in play
  if (rand() % 100 > HINT_CHANCE) return kNoHint; // rnd

  // Else choose a hint
  Hints_t _hint = kNoHint;
  while (_hint == kNoHint) {
    Hints_t _random = rand() % kNHintTypes;
    if ( m_hintIsActive[_random] == false ) _hint = _random;
  }

  return _hint;
}


Rooms_t getRoom(int _level, Hints_t* _consumeHint, bool* _consumeItem) {

  // TODO make rooms be spread out
  // TODO some hints only show up later

  while (true) {
    Rooms_t _room = kMaze;//rand() % kNRoomTypes;

    // Check level
    if (m_roomDescriptor[_room].m_minL > _level) continue;

    // Check if needs hint
    bool _needsHint = false;
    int _nCompatableHints = 0;
    int _hints[kNHintTypes] = {0};
    for (Hints_t _h = 1; _h < kNHintTypes; ++_h) { // Avoid kNoHint
      if ( m_roomDescriptor[_room].m_reqHint[ _h ] == 1 ) _needsHint = true; // I require a hint
      if ( m_roomDescriptor[_room].m_reqHint[ _h ] == 1 && m_hintIsActive[ _h ] == 1) { // This hint is available
        _hints[ _nCompatableHints++ ] = _h;
      }
    }
    if (_needsHint == true && _nCompatableHints == 0) continue; // Roll again
    else if (_needsHint == true) (*_consumeHint) = _hints[ rand() % _nCompatableHints ]; // Consume an available hint

    return _room;
  }
}

void generate() {

  m_dungeon.m_seed = time(NULL);
  srand(m_dungeon.m_seed);
  m_dungeon.m_fallingDeath = false;
  m_dungeon.m_gameOver = false;

  APP_LOG(APP_LOG_LEVEL_INFO,"sz info:%i dn:%i", sizeof(m_roomDescriptor) * kNRoomTypes, sizeof(m_dungeon) );

  m_dungeon.m_level = 0;
  m_dungeon.m_room = -1; // Will be incremented on kNewLevel
  m_dungeon.m_lives = 1;


  for (int _level = 0; _level < MAX_LEVELS; ++ _level) {
    m_dungeon.m_roomsPerLevel[_level] = MIN_ROOMS + (rand() % (MAX_ROOMS - MIN_ROOMS));
    APP_LOG(APP_LOG_LEVEL_INFO," -- L%i R%i", _level, m_dungeon.m_roomsPerLevel[_level]);
    for (int _room = 0; _room < m_dungeon.m_roomsPerLevel[_level]; ++_room) {

      Hints_t _consumeHint = kNoHint;
      bool _consumeItem = false;
      Rooms_t _roomType = getRoom(_level, &_consumeHint, &_consumeItem);
      m_dungeon.m_rooms[_level][_room] = _roomType;
      APP_LOG(APP_LOG_LEVEL_INFO,"GL:%i R:%i t:%i", _level, _room, (int)_roomType);

      // Can we add a hint to this room?
      Hints_t _addHint = getHint(_roomType);
      if (_addHint != kNoHint) {
        ++m_hintsInPlay;
        m_hintIsActive[_addHint] = 1;
        m_hintValue[_addHint] = rand() % getHintValueMax(_addHint);

        m_dungeon.m_roomGiveHint[_level][_room] = _addHint;
        m_dungeon.m_roomGiveHintValue[_level][_room] = m_hintValue[_addHint];
        APP_LOG(APP_LOG_LEVEL_INFO,"      A H t:%i v:%i", (int)_addHint, (int)m_hintValue[_addHint]);

      }

      // Are we consuming a hint?
      if (_consumeHint != kNoHint) {
        m_dungeon.m_roomNeedHint[_level][_room] = _consumeHint;
        m_dungeon.m_roomNeedHintValue[_level][_room] = m_hintValue[_consumeHint];
        APP_LOG(APP_LOG_LEVEL_INFO,"       C H t:%i v:%i", (int)_consumeHint, (int)m_hintValue[_consumeHint]);

        --m_hintsInPlay;
        m_hintIsActive[_consumeHint] = 0;
        m_hintValue[_consumeHint] = 0;
      }


    }
  }
  setGameState(kNewRoom);
}
