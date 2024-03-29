#pragma once
#include "game.h"

typedef struct {
  bool m_reqHint[kNHintTypes];
  bool m_reqItem;
  bool m_giveItem;
  bool m_giveHint;
  int8_t m_minL;
} RoomDescriptor_t;

#define HINT_CHANCE 50

void generate(PlaydateAPI* pd);
