#pragma once
#include <pebble.h>
#include "game.h"
#include "sprite.h"

void initRender(void);

void drawBitmap(GContext* _ctx, GBitmap* _bitmap, int _x, int _y);
void drawBitmapAbs(GContext* _ctx, GBitmap* _bitmap, GPoint _p);
void drawBitmapAbsNoCorrection(GContext* _ctx, GBitmap* _bitmap, GPoint _p);

void renderHintNumber(GContext* _ctx, GRect _r, int _value, bool _invert);
void renderBottomWall(GContext* _ctx);
void renderWalls(GContext* _ctx, bool _l, bool _rA, bool _rB, bool _rC);
void renderWallClutter(GContext* _ctx);
void renderFloor(GContext* _ctx, int mode, int8_t _from, int8_t _to);
void renderPlayer(GContext* _ctx);
void renderFade(Layer* _thisLayer, GContext* _ctx, bool _in);
void renderBorderText(GContext* _ctx, GRect _loc, GFont _f, const char* _buffer, uint8_t _offset, GTextAlignment _al, bool _invert);
void renderBorderTextNoCorrection(GContext* _ctx, GRect _loc, GFont _f, const char* _buffer, uint8_t _offset, GTextAlignment _al, bool _invert);
void renderClutter(GContext* _ctx);
void renderTextInFrame(GContext* _ctx, const char* _msg, GRect _b);
void renderTextInFrameNoCorrection(GContext* _ctx, const char* _msg, GRect _b);
void renderMessage(GContext* _ctx, const char* _msg);
void renderPit(GContext* _ctx);
void renderFinalPit(GContext* _ctx);
void renderFrame(GContext* _ctx, GRect _b);
void renderLinePath(GContext* _ctx, int _x1, int _y1, int _x2, int _y2);
void renderStandingStoneFloor(GContext* _ctx);
void renderStandingStoneGrid(GContext* _ctx, int8_t* _coloursA, int8_t* _coloursB, int8_t* _coloursC, int8_t* _correct, int16_t _state, bool _isPattern);
void renderStandingStone(GContext* _ctx, GPoint _p, GColor _c, StoneTypes_t _st);
void renderProgressBar(Layer* _thisLayer, GContext* _ctx);
void renderSawFloor(GContext* _ctx, int8_t _offset);
void renderSawWalls(GContext* _ctx, int8_t _offset);
void renderArrows(GContext* _ctx, int8_t _x, int8_t _yStart, int8_t _yAdd);
void renderArrowsDetailed(GContext* _ctx, int8_t _x, int8_t _yStart, int8_t _yAdd, bool _0, bool _1, bool _2, bool _force);

#ifdef YCGBv2
  void renderBomb(GContext* _ctx, uint8_t _bombStage, int8_t _location);
  void renderBoxGrid(GContext* _ctx, int8_t* _coloursA, int8_t* _coloursB, int8_t* _coloursC, int8_t* _offset);
  void renderFloorArrows(GContext* _ctx, Options_t maze0[3][3], uint8_t mwin0[3][3], Options_t maze1[5][7], uint8_t mwin1[5][7], uint16_t _tickB);
  void renderShortcutFloor(GContext* _ctx);
  void renderShortcutWalls(GContext* _ctx);
  void renderGreekFrames(GContext* _ctx, uint8_t _a[TOTAL_LETTERS], uint8_t _b[TOTAL_LETTERS], uint8_t _c[TOTAL_LETTERS], uint8_t _lettersThisLevel);
  void renderGamble(GContext* _ctx, uint8_t _wheel, uint16_t _angle, uint8_t _clack);
  void renderPatternUnder(GContext* _ctx, GPoint _p, uint8_t _id1, uint8_t _id2);
  void renderPatternLine(GContext* _ctx, GPoint _p, uint16_t _a, GColor _c1, GColor _c2, uint8_t _w1, uint8_t _w2);
  void renderFloorPuzzleShape(GContext* _ctx, GPoint _p, uint8_t _inner[4], uint8_t _outer[4], uint8_t _rot, uint8_t _flip);
  void renderSpikes(GContext* _ctx, int16_t _off[3], uint8_t _layer);
  void renderSpikeHoleBottom(GContext* _ctx, uint8_t _layer);
#endif

GColor getShieldColor(int8_t _value);
