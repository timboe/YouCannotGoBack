#include "end.h"
#include "../generate.h"
#include "../sound.h"

static uint16_t s_state = 0;
static char s_timeDisplay[64];
static char s_scoreDisplay[64];
static char s_bestDisplay[64];
static char s_rankDisplay[64];
static int s_score = 0;
static int s_pb = 0;
static int s_rank = 0;
static PlaydateAPI* s_cachedPtr;

#define SCORE_MAX 10000
#define LOSS_PER_TICK 2

#define BOARD_NAME "High Scores"

void updateProcEnd(PlaydateAPI* _pd, bool _isRotated) {

  renderClear(_pd, false);
  int _x = (_isRotated ? SIZE*2 : 0);
  int _w = (_isRotated ? 144-(SIZE*4) : 144);
  if (m_dungeon.m_gameOver == 1) {
    static const char _end1[] = "OOOH, NASTY...";
    static const char _end1_short[] = "OOH, NASTY..";  
    PDRect _rect = {.x = _x, .y = 0, .width = _w, .height = 43};
    renderTextInFrame(_pd, _isRotated ? _end1_short : _end1, _rect);
    drawBitmapAbs(_pd, m_grave, 50, 50);
    static const char _end2[] = "RESTART";
    PDRect _rect2 = {.x = 28, .y = 125, .width = 88, .height = 43};
    renderTextInFrame(_pd, _end2, _rect2);
    if (getGameState() == kAwaitInput && getFrameCount() < ANIM_FPS/2) {
      drawCBitmap(_pd, &m_arrow_d, 3, 13);
      drawCBitmap(_pd, &m_arrow_d, 13, 13);
    }
  } else {
    static const char _end1[] = "NICELY DONE!\n DUNGEONEER";
    PDRect _rect = {.x = _x, .y = 0, .width = _w, .height = 60};
    renderTextInFrame(_pd, _end1, _rect);
    drawBitmapAbs(_pd, m_treasureBanner, 19, _rect.height + 5);
    PDRect _rect2 = {.x = 0, .y =  _rect.height + 84, .width = 144, .height = 20};
    renderBorderText(_pd, _rect2, m_fontMain, s_timeDisplay, 2, false);
    _rect2.y += 14;
    static int16_t _basicTimer = 0;
    ++_basicTimer;
    if (_basicTimer < 10) {
      renderBorderText(_pd, _rect2, m_fontMain, s_scoreDisplay, 2, false);
    } else  {
      renderBorderText(_pd, _rect2, m_fontMain, s_bestDisplay, 2, false);
      if (_basicTimer == 20) _basicTimer = 0;
    }
    _rect2.y += 14;
    renderBorderText(_pd, _rect2, m_fontMain, s_rankDisplay, 2, false);
    if (getGameState() == kAwaitInput && getFrameCount() < ANIM_FPS/2) {
      drawCBitmap(_pd, &m_arrow_d, 8, 12);
    }
  }
}

void addScoreCallback(PDScore* score, const char* errorMessage) {
  #ifdef DEV
  if (errorMessage) s_cachedPtr->system->logToConsole("addScoreCallback err: %s", errorMessage);
  #endif
  if (!score) return;
  s_rank = score->rank;
  s_score = score->value;
  snprintf(s_scoreDisplay, 64, "SCORE %i", s_score);
  snprintf(s_rankDisplay, 64, "RANK %i", s_rank);
  s_cachedPtr->scoreboards->freeScore(score);
}

void personalBestCallback(PDScore* score, const char* errorMessage) {
  #ifdef DEV
  if (errorMessage) s_cachedPtr->system->logToConsole("personalBestCallback err: %s", errorMessage);
  #endif
  if (!score) return;
  s_pb = score->value;
  snprintf(s_bestDisplay, 64, "BEST %i", s_pb);
  s_cachedPtr->scoreboards->freeScore(score);
}

bool tickEnd(PlaydateAPI* _pd, bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    if (m_dungeon.m_gameOver == 2) {
      s_cachedPtr = _pd;
      snprintf(s_timeDisplay, 64, "TIME %im%is", (m_dungeon.m_ticksTotal/ANIM_FPS) / 60, (m_dungeon.m_ticksTotal/ANIM_FPS) % 60);
      s_score = SCORE_MAX - (m_dungeon.m_ticksTotal * LOSS_PER_TICK);
      if (s_score < 0) s_score = 0;
      snprintf(s_scoreDisplay, 64, "SCORE %i", s_score);
      snprintf(s_bestDisplay, 64, "BEST ????");
      snprintf(s_rankDisplay, 64, "RANK ????");

      const int addScore = _pd->scoreboards->addScore(BOARD_NAME, s_score, addScoreCallback);
      #ifdef DEV
      if (addScore) _pd->system->logToConsole("addScore returned %i", addScore);
      #endif
      const int getPB = _pd->scoreboards->getPersonalBest(BOARD_NAME, personalBestCallback);
      #ifdef DEV
      if (getPB) _pd->system->logToConsole("getPersonalBest returned %i", getPB);
      #endif
    }
    return false;
  }

  if (s_state == 0) {
    setGameState(kAwaitInput);
    if (m_dungeon.m_gameOver == 1) {
      looseSound();
    } else {
      winSound();
    }
    ++s_state;
  } else if (s_state == 1) { // restart
    generate(_pd);
  }

  return false;
}
