#pragma once
#include <pebble.h>
#include "../render.h"
#include "../game.h"

void initStart();
void updateProcStart(Layer* _thisLayer, GContext* _ctx);
void tickStart();
void clickStart(ButtonId _button);
