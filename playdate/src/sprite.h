#pragma once
#include "game.h"
#include "patterns.h"

#define SIZE 8
#define MAX_FLOOR 4
#define N_SMALL_CLUTTER 8
#define N_MED_CLUTTER 2
#define MAX_CLUTTER (N_SMALL_CLUTTER+N_MED_CLUTTER)

// Number of sprites in x and y
#define SSHEET_X 32
#define SSHEET_Y 32

void initSprite(PlaydateAPI* _pd);
void deinitSprite(void);

#define CBITMAP_MAX 6
struct CBitmap {
  LCDBitmap* bitmap[CBITMAP_MAX*CBITMAP_MAX];
  uint16_t w;
  uint16_t h;
};

struct CBitmap* getOuterWall(int _d);
struct CBitmap* getFloor(bool _random, int8_t m_level);
struct CBitmap* getClutter(bool _broken);

extern LCDBitmap* m_borderL;
extern LCDBitmap* m_borderR;

extern LCDBitmap* m_grave;
extern LCDBitmap* m_treasureBanner;

extern LCDBitmap* m_sawB;
extern LCDBitmap* m_sawA;

extern LCDBitmap* m_rotate;

extern LCDBitmap* m_bomb[4];

extern LCDBitmap* m_spear;
extern LCDBitmapTable* m_smask;

extern struct CBitmap m_UOuterWall[3];
extern struct CBitmap m_DOuterWall[3];
extern struct CBitmap m_LOuterWall[3];
extern struct CBitmap m_ROuterWall[3];

extern struct CBitmap m_halfUpperWall[2];
extern struct CBitmap m_halfLowerWall[2];

extern struct CBitmap m_outerCorner[4];
extern struct CBitmap m_innerCorner[4];
extern struct CBitmap m_innerWall[4];
extern struct CBitmap m_torchWall[4];

extern struct CBitmap m_stairs;

extern struct CBitmap m_arrow_u;
extern struct CBitmap m_arrow_r;
extern struct CBitmap m_arrow_d;

extern struct CBitmap m_playerSprite[MAX_FRAMES];

extern struct CBitmap m_clutterSprite[MAX_CLUTTER];

extern struct CBitmap m_LOpenDoor;
extern struct CBitmap m_ROpenDoor;
extern struct CBitmap m_RShutDoor;
extern struct CBitmap m_LDoorstep;
extern struct CBitmap m_RDoorstep;

extern struct CBitmap m_greek[MAX_GREEK];
extern struct CBitmap m_tapestrySprite[3];
extern struct CBitmap m_shieldSprite;

extern struct CBitmap m_guardian;
extern struct CBitmap m_blockWall[3];
extern struct CBitmap m_block;
extern struct CBitmap m_fire[2];

extern struct CBitmap m_bridge[3];
extern struct CBitmap m_flagstone[3];
extern struct CBitmap m_chest[2];
extern struct CBitmap m_table;
extern struct CBitmap m_black;
extern struct CBitmap m_treasure[3];
extern struct CBitmap m_symbol[MAX_SYMBOL];

extern LCDFont* m_fontMain;
extern LCDFont* m_fontMsg;
extern LCDFont* m_fontIntro;


