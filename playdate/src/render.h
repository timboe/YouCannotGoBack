#pragma once
#include "game.h"
#include "sprite.h"

void drawBitmap(PlaydateAPI* _pd, LCDBitmap* _bitmap, int _x, int _y);
void drawBitmapAbs(PlaydateAPI* _pd, LCDBitmap* _bitmap, int _x, int _y);
void drawBitmapAbsRot(PlaydateAPI* _pd, LCDBitmap* _bitmap, int _x, int _y, float _rot);

void drawCBitmap(PlaydateAPI* _pd, struct CBitmap* _cbitmap, int _x, int _y);
void drawCBitmapAbs(PlaydateAPI* _pd, struct CBitmap* _cbitmap, int _x, int _y);

void renderGameFrame(PlaydateAPI* _pd);
#ifdef SCOREBOARD
void renderScoresFrame(PlaydateAPI* _pd);
#endif
void renderClear(PlaydateAPI* _pd, bool transparentCentre);
void renderBlack(PlaydateAPI* _pd);
void renderHintNumber(PlaydateAPI* _pd, PDRect _r, int _value, bool _invert);
void renderWalls(PlaydateAPI* _pd, bool _l, bool _rA, bool _rB, bool _rC);
void renderWallClutter(PlaydateAPI* _pd);
void renderFloor(PlaydateAPI* _pd, int mode);
void renderPlayer(PlaydateAPI* _pd);
void renderFade(PlaydateAPI* _pd, bool _in, bool _isRotated);
void renderBorderText(PlaydateAPI* _pd, PDRect _loc, LCDFont* _f, const char* _buffer, uint8_t _offset, bool _invert);
void renderClutter(PlaydateAPI* _pd);
void renderTextInFrame(PlaydateAPI* _pd, const char* _msg, PDRect _b);
void renderText(PlaydateAPI* _pd, const char* _msg, PDRect _b, LCDBitmapDrawMode _dm);
void renderMessage(PlaydateAPI* _pd, const char* _msg, bool _isRotated);
void renderPit(PlaydateAPI* _pd);
void renderFinalPit(PlaydateAPI* _pd);
void renderFrame(PlaydateAPI* _pd, PDRect _b);
void renderStandingStoneFloor(PlaydateAPI* _pd);
void renderStandingStoneGrid(PlaydateAPI* _pd, 
	int8_t* _coloursA, 
	int8_t* _coloursB, 
	int8_t* _coloursC, 
	int8_t* _correct,
	int16_t _state,	
	bool _isPattern);
void renderLinePath(PlaydateAPI* _pd, int _x1, int _y1, int _x2, int _y2);
void renderStandingStone(PlaydateAPI* _pd, float _x, float _y, LCDColor _c, StoneTypes_t _st);
void renderPatternLine(PlaydateAPI* _pd, float _x, float _y, float _a, LCDColor _c1, LCDColor _c2, int _w1, int _w2);
void renderPatternUnder(PlaydateAPI* _pd, float _x, float _y, int _id1, int _id2);
void renderBoxGrid(PlaydateAPI* _pd, int8_t* _coloursA, int8_t* _coloursB, int8_t* _coloursC, int8_t* _offset);
void renderBoxGridBox(PlaydateAPI* _pd, int _x1, int _y1, LCDColor _c, int8_t _offset);
void renderProgressBar(PlaydateAPI* _pd, bool isRotated);
void renderSawFloor(PlaydateAPI* _pd, int8_t _offset);
void renderSawWalls(PlaydateAPI* _pd, int8_t _offset);
void renderShortcutFloor(PlaydateAPI* _pd);
void renderShortcutWalls(PlaydateAPI* _pd);
void renderArrows(PlaydateAPI* _pd, int8_t _x, int8_t _yStart, int8_t _yAdd);
void renderArrows2(PlaydateAPI* _pd, int8_t _x, int8_t _yStart, int8_t _yAdd, bool _0, bool _1, bool _2, bool _force);
void renderBomb(PlaydateAPI* _pd, int16_t _bomb, int8_t _location);
void renderSpikes(PlaydateAPI* _pd, float* _off, bool _top);
void renderGreekFrames(PlaydateAPI* _pd, const char* _a, const char* _b, const char* _c);

LCDColor getShieldColor(int8_t _value);

// Spikes
#define S_OFF 48
