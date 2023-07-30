#include "greek.h"
#include "../sprite.h"

static uint16_t s_state = 0;
static uint16_t s_correct = 0;
static int8_t s_choices[3] = {0};

#define BUF_SIZE 64

static char s_strA[BUF_SIZE] = {0};
static char s_strB[BUF_SIZE] = {0};
static char s_strC[BUF_SIZE] = {0};

#define TOTAL_LETTERS 6

//Full: ΑΒΓΔΕΖΗΘΙΚΛΜΝΞΟΠΡΣΤΥΦΧΨΩαβγδεηθικλμνξοπρςστυφχψω
//Clue: πδζΛφηΩψ
//Excluded:ΦΨΠχ

// Note: First character, Α, never gets used 
static const char s_letters[] = "ΑΒΓΔΕΖΗΘΙΚΜΝΞΟΡΣΤΥΧαβγεθικλμνξορςστυω";
static const size_t s_size = ((sizeof(s_letters) / sizeof(s_letters[0])) - 1) / 2; // Sub \0, two char characters

// These need to be in the same order as in the sprite sheet
static const char s_clues[] = "πδζΛφηΩψ"; 

void updateProcGreek(PlaydateAPI* _pd) {
  renderFloor(_pd, 0);
  renderClutter(_pd);
  renderGreekFrames(_pd, s_strA, s_strB, s_strC);
  renderPlayer(_pd);
  renderWalls(_pd, true, true, true, true);
  renderWallClutter(_pd);
  renderArrows(_pd, 15, 5, 4);
}

void addChar(char* _target, const char* _source, const int _offset) {
  char buf[3] = {0};
  buf[0] = _source[_offset];
  buf[1] = _source[_offset+1];
  strcat(_target, buf); 
}

int randLetter(void) {
  return (rand() % s_size) * 2; // *2 as two char UNICODE
}

void populateScroll(PlaydateAPI* _pd, 
  char* _target, // String buffer to write into
  const int _lettersThisLevel, // How many characters to write in this level of the dungeon
  const int _cluePoint, // Which character of the string must be the clue symbol (right or wrong)
  const int8_t _clue, // The symbol to use at this point (again, right or wrong)
  const bool _correct) // Are we showing the correct clue symbol here?
{
  for (int _c = 0; _c < _lettersThisLevel; ++_c) {
    const int _r = randLetter();
    // If we were going to add an Α, or it is the place in the string we are guaranteed to have the clue symbol
    if (!_r || _cluePoint == _c) {
      addChar(_target, s_clues, _clue * 2); // *2 as two char UNICODE
      #ifdef DEV
      _pd->system->logToConsole("%i: %s %s (ID %i)", _c, (_correct ? "WIN" : "LOOSE"), _target, _clue);
      #endif
    } else {
      addChar(_target, s_letters, _r);
      #ifdef DEV
      _pd->system->logToConsole("%i: Pad %s", _c, _target);
      #endif
    } 
  }
}

bool tickGreek(PlaydateAPI* _pd, bool _doInit) {
  if (_doInit == true) {
    #ifdef DEV
    _pd->system->logToConsole("Total Greek letters: %i", s_size);
    #endif
    s_correct = randomiseChoices(_pd, s_choices, 0);

    memset(s_strA, 0, BUF_SIZE);
    memset(s_strB, 0, BUF_SIZE);
    memset(s_strC, 0, BUF_SIZE);

    const int _lettersThisLevel = MIN(6, TOTAL_LETTERS/3 * (m_dungeon.m_difficulty + 1));

    // Each clue symbol gets included at least once per string
    int _cluePoint[3] = {0};
    for (int _i = 0; _i < 3; ++_i) {
      _cluePoint[_i] = rand() % _lettersThisLevel;
    }
    #ifdef DEV
    _pd->system->logToConsole("Clue points %i %i %i", _cluePoint[0], _cluePoint[1], _cluePoint[2]);
    #endif
    
    #ifdef DEV
    _pd->system->logToConsole("-- A");
    #endif
    populateScroll(_pd, s_strA, _lettersThisLevel, _cluePoint[0], s_choices[0], s_correct == 0);
    
    #ifdef DEV
    _pd->system->logToConsole("-- B");
    #endif
    populateScroll(_pd, s_strB, _lettersThisLevel, _cluePoint[1], s_choices[1], s_correct == 1);
    
    #ifdef DEV
    _pd->system->logToConsole("-- C");
    #endif
    populateScroll(_pd, s_strC, _lettersThisLevel, _cluePoint[2], s_choices[2], s_correct == 2);

    s_state = 0;
    m_player.m_position_x = 0;
    m_player.m_position_y = SIZE*9;
    addCluter(6, 0, 20); // Only left
    return false;
  }

 if (s_state == 0) {
   enterRoom(&s_state);
 } else if (s_state == 1) {
   setGameState(kAwaitInput);
   ++s_state;
 } else if (s_state == 2) {
   if (getPlayerChoice() != s_correct) {
     if (m_dungeon.m_lives > 0) --m_dungeon.m_lives;
     else m_dungeon.m_rooms[ m_dungeon.m_level ][ m_dungeon.m_room + 1 ] = kDeath;
     #ifdef DEV
     _pd->system->logToConsole("WRONG");
     #endif
   }
   moveToExit(&s_state);
 } else if (s_state == 3) {
   setGameState(kFadeOut);
 }

  return false;
}
