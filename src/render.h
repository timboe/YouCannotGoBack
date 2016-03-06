#pragma once
#include <pebble.h>
#include "game.h"
#include "sprite.h"

void drawBitmap(GContext* _ctx, GBitmap* _bitmap, int _x, int _y);
void drawBitmapAbs(GContext* _ctx, GBitmap* _bitmap, GPoint _p);

void renderHintNumber(GContext* _ctx, GRect _r, int _value);
void renderWalls(GContext* _ctx, bool _l, bool _rA, bool _rB, bool _rC);
void renderFloor(GContext* _ctx, int mode);
void renderPlayer(GContext* _ctx);
void renderFade(Layer* _thisLayer, GContext* _ctx, bool _in);
void renderBorderText(GContext* _ctx, GRect _loc, GFont _f, const char* _buffer, uint8_t _offset, GTextAlignment _al);
void renderClutter(GContext* _ctx);
void renderTextInFrame(GContext* _ctx, const char* _msg, GRect _b);
void renderMessage(GContext* _ctx, const char* _msg);
void renderPit(GContext* _ctx);
void renderFinalPit(GContext* _ctx);
void renderFrame(GContext* _ctx, GRect _b);
void renderStandingStoneGrid(GContext* _ctx, int8_t* _coloursA, int8_t* _coloursB, int8_t* _coloursC);
void renderLinePath(GContext* _ctx, int _x1, int _y1, int _x2, int _y2);
void renderStandingStone(GContext* _ctx, int _x1, int _y1, GColor _c);


GColor getShieldColor(int8_t _value);
