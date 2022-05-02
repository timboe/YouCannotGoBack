#pragma once
#include "game.h"
#include "sprite.h"

void drawBitmap(PlaydateAPI* _pd, LCDBitmap* _bitmap, int _x, int _y);
void drawBitmapAbs(PlaydateAPI* _pd, LCDBitmap* _bitmap, int _x, int _y);
void drawBitmapAbsRot(PlaydateAPI* _pd, LCDBitmap* _bitmap, int _x, int _y, float _rot);

void drawCBitmap(PlaydateAPI* _pd, struct CBitmap* _cbitmap, int _x, int _y);
void drawCBitmapAbs(PlaydateAPI* _pd, struct CBitmap* _cbitmap, int _x, int _y);

void renderGameFrame(PlaydateAPI* _pd);
void renderClear(PlaydateAPI* _pd, bool transparentCentre);
void renderHintNumber(PlaydateAPI* _pd, PDRect _r, int _value, bool _invert);
void renderWalls(PlaydateAPI* _pd, bool _l, bool _rA, bool _rB, bool _rC);
void renderWallClutter(PlaydateAPI* _pd);
void renderFloor(PlaydateAPI* _pd, int mode);
void renderPlayer(PlaydateAPI* _pd);
void renderFade(PlaydateAPI* _pd, bool _in);
void renderBorderText(PlaydateAPI* _pd, PDRect _loc, LCDFont* _f, const char* _buffer, uint8_t _offset, bool _invert);
void renderClutter(PlaydateAPI* _pd);
void renderTextInFrame(PlaydateAPI* _pd, const char* _msg, PDRect _b);
void renderMessage(PlaydateAPI* _pd, const char* _msg, bool _isRotated);
void renderPit(PlaydateAPI* _pd);
void renderFinalPit(PlaydateAPI* _pd);
void renderFrame(PlaydateAPI* _pd, PDRect _b);
void renderStandingStoneGrid(PlaydateAPI* _pd, int8_t* _coloursA, int8_t* _coloursB, int8_t* _coloursC);
void renderLinePath(PlaydateAPI* _pd, int _x1, int _y1, int _x2, int _y2);
void renderStandingStone(PlaydateAPI* _pd, int _x1, int _y1, LCDColor _c);
void renderProgressBar(PlaydateAPI* _pd, bool isRotated);
void renderSawFloor(PlaydateAPI* _pd, int8_t _offset);
void renderSawWalls(PlaydateAPI* _pd, int8_t _offset);
void renderArrows(PlaydateAPI* _pd, int8_t _x, int8_t _yStart, int8_t _yAdd);
void renderBomb(PlaydateAPI* _pd, int16_t _bomb, int8_t _locatioon);

LCDColor getShieldColor(int8_t _value);
