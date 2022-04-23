#pragma once
#include "game.h"

#define SIZE 8
#define MAX_FLOOR 4
#define N_SMALL_CLUTTER 8
#define N_MED_CLUTTER 2
#define MAX_CLUTTER (N_SMALL_CLUTTER+N_MED_CLUTTER)

void initSprite();
void deinitSprite();

LCDBitmap* getOuterWall(int _d);
LCDBitmap* getFloor(bool _random);
LCDBitmap* getClutter(bool _broken);

extern LCDBitmap* m_UOuterWall[3];
extern LCDBitmap* m_DOuterWall[3];
extern LCDBitmap* m_LOuterWall[3];
extern LCDBitmap* m_ROuterWall[3];

extern LCDBitmap* m_halfUpperWall[2];
extern LCDBitmap* m_halfLowerWall[2];

extern LCDBitmap* m_outerCorner[4];
extern LCDBitmap* m_innerCorner[4];
extern LCDBitmap* m_innerWall[4];
extern LCDBitmap* m_torchWall[4];

extern LCDBitmap* m_stairs;
extern LCDBitmap* m_saw;
extern LCDBitmap* m_sawB;
extern LCDBitmap* m_sawA;


extern LCDBitmap* m_arrow;

extern LCDBitmap* m_playerSprite[MAX_FRAMES];

extern LCDBitmap* m_LOpenDoor;
extern LCDBitmap* m_ROpenDoor;
extern LCDBitmap* m_RShutDoor;
extern LCDBitmap* m_LDoorstep;
extern LCDBitmap* m_RDoorstep;

extern LCDBitmap* m_greek[MAX_GREEK];
extern LCDBitmap* m_tapestrySprite[3];
extern LCDBitmap* m_shieldSprite;

extern LCDBitmap* m_guardian;
extern LCDBitmap* m_blockWall[3];
extern LCDBitmap* m_block;
extern LCDBitmap* m_fire[2];

extern LCDBitmap* m_bridge[3];
extern LCDBitmap* m_grave;
extern LCDBitmap* m_flagstone[3];
extern LCDBitmap* m_chest;
extern LCDBitmap* m_table;
extern LCDBitmap* m_black;
extern LCDBitmap* m_treasureBanner;
extern LCDBitmap* m_treasure[3];
extern LCDBitmap* m_symbol[MAX_SYMBOL];
