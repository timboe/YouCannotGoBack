#pragma once
#include <pebble.h>
#include "../render.h"
#include "../game.h"
#ifdef YCGBv2

void updateProcBomb(GContext* _ctx);
bool tickBomb(bool _doInit);
void bombTimer(void);

#endif