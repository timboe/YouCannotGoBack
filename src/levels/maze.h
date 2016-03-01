#pragma once
#include <pebble.h>
#include "../render.h"
#include "../game.h"
#include "common.h"

void updateProcMaze(GContext* _ctx);
bool tickMaze(bool _doInit);
