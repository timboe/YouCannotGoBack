#pragma once
#include <pebble.h>
#include "game.h"
#include "sprite.h"

void drawBitmap(GContext* _ctx, GBitmap* _bitmap, int _x, int _y);
void drawBitmapAbs(GContext* _ctx, GBitmap* _bitmap, GPoint _p);

void renderWalls(Layer* _thisLayer, GContext* _ctx, bool _l, bool _rA, bool _rB, bool _rC);
void renderFloor(Layer* _thisLayer, GContext* _ctx, int mode);
void renderPlayer(Layer* _thisLayer, GContext* _ctx);
void renderFade(Layer* _thisLayer, GContext* _ctx, bool _in);
void renderBorderText(GContext* _ctx, GRect _loc, GFont _f, const char* _buffer, uint8_t _offset, GTextAlignment _al);
void renderClutter(GContext* _ctx);
