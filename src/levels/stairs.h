#pragma once
#include <pebble.h>
#include "../render.h"
#include "../game.h"
#include "common.h"

void initStairs();
void updateProcStairs(Layer* _thisLayer, GContext* _ctx);
void tickStairs();
void clickStairs(ButtonId _button);
