#pragma once
#include <pebble.h>
#include "../render.h"
#include "../game.h"
#include "common.h"

void initStart();
void updateProcStart(GContext* _ctx);
void tickStart();
void clickStart(ButtonId _button);
