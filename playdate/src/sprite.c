#include "sprite.h"


LCDBitmap* m_spriteMap;
LCDBitmap* m_saw;
LCDBitmap* m_sawA;
LCDBitmap* m_sawB;

LCDBitmap* m_UOuterWall[3];
LCDBitmap* m_DOuterWall[3];
LCDBitmap* m_LOuterWall[3];
LCDBitmap* m_ROuterWall[3];

LCDBitmap* m_outerCorner[4];
LCDBitmap* m_innerCorner[4];
LCDBitmap* m_innerWall[4];
LCDBitmap* m_torchWall[4];

LCDBitmap* m_floorA[MAX_FLOOR];
LCDBitmap* m_floorB[MAX_FLOOR];
LCDBitmap* m_floorC[MAX_FLOOR];

LCDBitmap* m_LOpenDoor;
LCDBitmap* m_ROpenDoor;
LCDBitmap* m_RShutDoor;
LCDBitmap* m_LDoorstep;
LCDBitmap* m_RDoorstep;

LCDBitmap* m_greek[MAX_GREEK];
LCDBitmap* m_playerSprite[MAX_FRAMES];
LCDBitmap* m_stairs;
LCDBitmap* m_arrow;
LCDBitmap* m_clutterSprite[MAX_CLUTTER];
LCDBitmap* m_tapestrySprite[3];
LCDBitmap* m_shieldSprite;

LCDBitmap* m_guardian;
LCDBitmap* m_blockWall[3];
LCDBitmap* m_block;
LCDBitmap* m_bridge[3];
LCDBitmap* m_grave;
LCDBitmap* m_fire[2];
LCDBitmap* m_flagstone[3];
LCDBitmap* m_chest;
LCDBitmap* m_table;
LCDBitmap* m_black;
LCDBitmap* m_treasureBanner;
LCDBitmap* m_treasure[3];
LCDBitmap* m_symbol[MAX_SYMBOL];

LCDBitmap* m_halfUpperWall[2];
LCDBitmap* m_halfLowerWall[2];

LCDBitmap* getSprite(int _x, int _y, int _w, int _h) {
  return NULL; // TODO
  //return gbitmap_create_as_sub_bitmap(m_spriteMap, GRect(SIZE * _x, SIZE * _y, SIZE * _w, SIZE * _h));
}

LCDBitmap* getClutter(bool _broken) {
  return m_clutterSprite[ rand() % (_broken == true ? 3 :MAX_CLUTTER) ];
}

LCDBitmap* getOuterWall(int _d) {
  int _r = rand() % 3;
  switch (_d) {
    case 0:  return m_UOuterWall[_r];
    case 1:  return m_ROuterWall[_r];
    case 2:  return m_DOuterWall[_r];
    default: return m_LOuterWall[_r];
  }
}

LCDBitmap* getFloor(bool _random) {
  int _r = rand() % MAX_FLOOR;
  if (_random == false) _r = 0;
  switch (m_dungeon.m_level) {
    case 0:  return m_floorA[_r];
    case 1:  return m_floorB[_r];
    default: return m_floorC[_r];
  }
}

void initSprite() {
  //m_spriteMap = gbitmap_create_with_resource(RESOURCE_ID_SPRITESHEET);
  //m_grave = gbitmap_create_with_resource(RESOURCE_ID_GRAVE);
  //m_treasureBanner = gbitmap_create_with_resource(RESOURCE_ID_TREASURE);

  //m_saw = gbitmap_create_with_resource(RESOURCE_ID_SAW);
  //m_sawA = gbitmap_create_as_sub_bitmap(m_saw, GRect(0, 0, 80, 40));
  //m_sawB = gbitmap_create_as_sub_bitmap(m_saw, GRect(0, 40, 80, 40));


  for (int _w = 0; _w < 3; ++_w) { // Walls
    m_UOuterWall[_w] = getSprite(_w + 0, 6, 2, 2);
    m_DOuterWall[_w] = getSprite(_w + 4, 8, 2, 2);
    m_ROuterWall[_w] = getSprite(0, _w + 9, 2, 2);
    m_LOuterWall[_w] = getSprite(2, _w + 8, 2, 2);

    m_flagstone[_w] = getSprite(12 + (_w * 2), 12, 2, 2);
  }

  m_halfUpperWall[0] = getSprite(2, 7, 2, 1);
  m_halfUpperWall[1] = getSprite(2, 6, 2, 1);
  m_halfLowerWall[0] = getSprite(4, 8, 2, 1);
  m_halfLowerWall[1] = getSprite(4, 9, 2, 1);

  m_outerCorner[0] = getSprite(6, 0, 2, 2);
  m_outerCorner[1] = getSprite(8, 0, 2, 2);
  m_outerCorner[2] = getSprite(6, 2, 2, 2);
  m_outerCorner[3] = getSprite(8, 2, 2, 2);

  m_innerCorner[0] = getSprite(0, 0, 2, 2);
  m_innerCorner[1] = getSprite(4, 0, 2, 2);
  m_innerCorner[2] = getSprite(4, 4, 2, 2);
  m_innerCorner[3] = getSprite(0, 4, 2, 2);

  m_innerWall[0] = getSprite(0, 2, 2, 2);
  m_innerWall[1] = getSprite(4, 2, 2, 2);
  m_innerWall[2] = getSprite(2, 0, 2, 2);
  m_innerWall[3] = getSprite(2, 4, 2, 2);

  m_torchWall[0] = getSprite(10, 2, 2, 2);
  m_torchWall[1] = getSprite(7,  4, 3, 2);
  m_torchWall[2] = getSprite(10, 4, 2, 3);
  m_torchWall[3] = getSprite(6,  6, 3, 2);

  m_arrow = getSprite(10, 0, 2, 2);

  m_stairs = getSprite(10, 8, 2, 2);

  for (int _f = 0; _f < MAX_FLOOR; ++_f) { // Floor
    m_floorA[_f] = getSprite(12 + (_f*2), 4, 2, 2);
    m_floorB[_f] = getSprite(12 + (_f*2), 2, 2, 2);
    m_floorC[_f] = getSprite(12 + (_f*2), 0, 2, 2);
  }

  m_LOpenDoor = getSprite(23, 6, 3, 4);
  m_ROpenDoor = getSprite(18, 6, 3, 4);
  m_RShutDoor = getSprite(14, 6, 3, 4);

  m_LDoorstep =  getSprite(27, 6, 1, 4);
  m_RDoorstep =  getSprite(26, 6, 1, 4);

  for (int _g = 0; _g < MAX_GREEK/2; ++_g) {
    m_greek[_g]               = getSprite(4 + _g, 12, 1, 1);
    m_greek[_g + MAX_GREEK/2] = getSprite(4 + _g, 13, 1, 1);
  }

  for (int _g = 0; _g < MAX_SYMBOL/2; ++_g) {
    m_symbol[_g]                = getSprite(24 + (_g*2), 0, 2, 2);
    m_symbol[_g + MAX_SYMBOL/2] = getSprite(24 + (_g*2), 2, 2, 2);
  }

  for (int _p = 0; _p < MAX_FRAMES; ++_p) {
    m_playerSprite[_p] = getSprite(16 + (_p*2), 14, 2, 2);
  }

  int _clutter = 0;
  for (int _c = 0; _c < N_SMALL_CLUTTER; ++_c) {
    m_clutterSprite[_clutter++] = getSprite(_c*2, 14, 2, 2);
  }
  m_clutterSprite[_clutter++] = getSprite(24, 10,  2, 4); // med 1
  m_clutterSprite[_clutter++] = getSprite(26, 10,  2, 4); // med 2

  m_tapestrySprite[0] = getSprite(20, 12,  1, 2);
  m_tapestrySprite[1] = getSprite(21, 12,  1, 2);
  m_tapestrySprite[2] = getSprite(22, 12,  1, 2);
  m_shieldSprite = getSprite(20, 10,  4, 2);

  m_guardian = getSprite(20, 0, 4, 6);
  m_blockWall[0] = getSprite(12, 6, 2, 2);
  m_blockWall[1] = getSprite(12, 8, 2, 2);
  m_blockWall[2] = getSprite(12, 10, 2, 2);
  m_block = getSprite(10, 10, 2, 2);
  m_fire[0] = getSprite(8, 12, 4, 2);
  m_fire[1] = getSprite(10, 7, 2, 1);


  m_bridge[0] = getSprite(6, 4, 1, 2);
  m_bridge[1] = getSprite(9, 6, 1, 2);
  m_bridge[2] = getSprite(14, 10, 1, 2);

  m_chest = getSprite(8, 10, 2, 2);
  m_table = getSprite(4, 10, 4, 2);
  m_black = getSprite(2, 2, 2, 2);

  m_treasure[0] = getSprite(16, 10, 2, 2);
  m_treasure[1] = getSprite(18, 10, 2, 2);
  m_treasure[2] = getSprite(23, 12, 1, 1);

}

void deinitSprite() {

  for (int _i = 1; _i < 4; ++_i) {
    //gbitmap_destroy(m_outerCorner[_i]);
    //gbitmap_destroy(m_innerCorner[_i]);
    //gbitmap_destroy(m_innerWall[_i]);
    //gbitmap_destroy(m_torchWall[_i]);

    if (_i >= 3) continue;
    //gbitmap_destroy(m_UOuterWall[_i]);
    //gbitmap_destroy(m_DOuterWall[_i]);
    //gbitmap_destroy(m_LOuterWall[_i]);
    //gbitmap_destroy(m_ROuterWall[_i]);

    //gbitmap_destroy(m_tapestrySprite[_i]);
    //gbitmap_destroy(m_blockWall[_i]);
    //gbitmap_destroy(m_bridge[_i]);
    //gbitmap_destroy(m_flagstone[_i]);
    //gbitmap_destroy(m_treasure[_i]);

    if (_i >= 2) continue;
    //gbitmap_destroy(m_fire[_i]);

    //gbitmap_destroy(m_halfUpperWall[_i]);
    //gbitmap_destroy(m_halfLowerWall[_i]);
  }

  for (int _i = 1; _i < MAX_FLOOR; ++_i) {
    //gbitmap_destroy(m_floorA[_i]);
    //gbitmap_destroy(m_floorB[_i]);
    //gbitmap_destroy(m_floorC[_i]);
  }

  //for (int _i = 1; _i < MAX_GREEK; ++_i) gbitmap_destroy(m_greek[_i]);
  //for (int _i = 1; _i < MAX_CLUTTER; ++_i) gbitmap_destroy(m_clutterSprite[_i]);
  //for (int _i = 1; _i < MAX_FRAMES; ++_i) gbitmap_destroy(m_playerSprite[_i]);
  //for (int _i = 1; _i < MAX_SYMBOL; ++_i) gbitmap_destroy(m_symbol[_i]);

  //gbitmap_destroy(m_sawA);
  //gbitmap_destroy(m_sawB);

  //gbitmap_destroy(m_LOpenDoor);
  //gbitmap_destroy(m_ROpenDoor);
  //gbitmap_destroy(m_RShutDoor);
  //gbitmap_destroy(m_LDoorstep);
  //gbitmap_destroy(m_RDoorstep);
  //gbitmap_destroy(m_stairs);
  //gbitmap_destroy(m_arrow);
  //gbitmap_destroy(m_shieldSprite);

  //gbitmap_destroy(m_guardian);
  //gbitmap_destroy(m_block);
  //gbitmap_destroy(m_chest);
  //gbitmap_destroy(m_table);
  //gbitmap_destroy(m_black);

  // Actual bitmaps
  ////gbitmap_destroy(m_spriteMap);
  ////gbitmap_destroy(m_grave);
  ////gbitmap_destroy(m_treasureBanner);
  ////gbitmap_destroy(m_saw);
}
