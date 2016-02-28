#pragma once
#include <pebble.h>
#include "../render.h"
#include "../game.h"
#include "common.h"

void initPword();
void updateProcPword(GContext* _ctx);
void tickPword();
void clickPword(ButtonId _button);
