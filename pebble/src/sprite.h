#pragma once
#include <pebble.h>
#include "game.h"

#define SIZE 8
#define MAX_FLOOR 4
#define N_SMALL_CLUTTER 8
#define N_MED_CLUTTER 2
#define MAX_CLUTTER (N_SMALL_CLUTTER+N_MED_CLUTTER)

void initSprite();
void deinitSprite();

GBitmap* getOuterWall(int _d);
GBitmap* getFloor(bool _random);
GBitmap* getClutter(bool _broken);

extern GBitmap* m_UOuterWall[3];
extern GBitmap* m_DOuterWall[3];
extern GBitmap* m_LOuterWall[3];
extern GBitmap* m_ROuterWall[3];

extern GBitmap* m_halfUpperWall[2];
extern GBitmap* m_halfLowerWall[2];

extern GBitmap* m_outerCorner[4];
extern GBitmap* m_innerCorner[4];
extern GBitmap* m_innerWall[4];
extern GBitmap* m_torchWall[4];

extern GBitmap* m_stairs;
extern GBitmap* m_saw;
extern GBitmap* m_sawB;
extern GBitmap* m_sawA;


extern GBitmap* m_arrow;

extern GBitmap* m_playerSprite[MAX_FRAMES];

extern GBitmap* m_LOpenDoor;
extern GBitmap* m_ROpenDoor;
extern GBitmap* m_RShutDoor;
extern GBitmap* m_LDoorstep;
extern GBitmap* m_RDoorstep;

extern GBitmap* m_greek[MAX_GREEK];
extern GBitmap* m_tapestrySprite[3];
extern GBitmap* m_shieldSprite;

extern GBitmap* m_guardian;
extern GBitmap* m_blockWall[3];
extern GBitmap* m_block;
extern GBitmap* m_fire[2];

extern GBitmap* m_bridge[3];
extern GBitmap* m_grave;
extern GBitmap* m_flagstone[3];
extern GBitmap* m_chest;
extern GBitmap* m_table;
extern GBitmap* m_black;
extern GBitmap* m_treasureBanner;
extern GBitmap* m_treasure[3];
extern GBitmap* m_symbol[MAX_SYMBOL];
