#pragma once
#include <pebble.h>
#include "game.h"

typedef struct {
  bool m_reqHint[kNHintTypes];
  bool m_reqItem;
  bool m_giveItem;
  bool m_giveHint;
  int8_t m_minL;
} RoomDescriptor_t;

#define HINT_CHANCE 80

void generate();
