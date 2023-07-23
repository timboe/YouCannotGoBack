#pragma once
#include "../render.h"
#include "../game.h"
#include "common.h"

typedef enum {
  kClover,
  kUnlockShortcut,
  kEvilWind,
  kInstantDeath,
  kNGambleTypes
} GambleOutcomes_t;

void updateProcGamble(PlaydateAPI* _pd);
bool tickGamble(bool _doInit);
