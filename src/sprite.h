#pragma once
#include <pebble.h>
#include "game.h"

#define SIZE 8

void initSprite();
void deinitSprite();

GBitmap* getOuterWall(int _d);
GBitmap* getFloor();

extern GBitmap* m_outerCorner[4];
extern GBitmap* m_innerCorner[4];
extern GBitmap* m_innerWall[4];
extern GBitmap* m_torchWall[4];

extern GBitmap* m_stairs;

extern GBitmap* m_arrow;

extern GBitmap* m_playerSprite[MAX_FRAMES];

extern GBitmap* m_LOpenDoor;
extern GBitmap* m_ROpenDoor;
extern GBitmap* m_RShutDoor;
extern GBitmap* m_LDoorstep;
extern GBitmap* m_RDoorstep;
