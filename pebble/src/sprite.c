#include "sprite.h"

#ifdef YCGBv2
GBitmap* m_ycgb;

GBitmap* m_fuse[3];
GBitmap* m_floorArrow[kNOptionTypes];
GBitmap* m_parchment[2];
GBitmap* m_clack[2];
GBitmap* m_spear;
GBitmap* m_spearHole[2];
GBitmap* m_wheelFrame;
GBitmap* m_wheelIcon[kNWheelIcons];
GBitmap* m_warning;
GBitmap* m_crack;
GBitmap* m_octagon;
#endif

GBitmap* m_spriteMap;
GBitmap* m_saw;
GBitmap* m_sawA;
GBitmap* m_sawB;

GBitmap* m_UOuterWall[3];
GBitmap* m_DOuterWall[3];
GBitmap* m_LOuterWall[3];
GBitmap* m_ROuterWall[3];

GBitmap* m_outerCorner[4];
GBitmap* m_innerCorner[4];
GBitmap* m_innerWall[4];
GBitmap* m_torchWall[4];

GBitmap* m_floorA[MAX_FLOOR];
GBitmap* m_floorB[MAX_FLOOR];
GBitmap* m_floorC[MAX_FLOOR];

GBitmap* m_LOpenDoor;
GBitmap* m_ROpenDoor;
GBitmap* m_RShutDoor;
GBitmap* m_LDoorstep;
GBitmap* m_RDoorstep;

GBitmap* m_greek[MAX_GREEK];
GBitmap* m_playerSprite[MAX_FRAMES];
GBitmap* m_stairs;
GBitmap* m_arrow;
GBitmap* m_clutterSprite[MAX_CLUTTER];
GBitmap* m_tapestrySprite[3];
GBitmap* m_shieldSprite;

GBitmap* m_guardian;
GBitmap* m_blockWall[3];
GBitmap* m_block;
GBitmap* m_bridge[3];
GBitmap* m_grave;
GBitmap* m_fire[2];
GBitmap* m_flagstone[3];
GBitmap* m_chest[2];
GBitmap* m_table;
GBitmap* m_black;
GBitmap* m_treasureBanner;
GBitmap* m_treasure[3];
GBitmap* m_symbol[MAX_SYMBOL];

GBitmap* m_halfUpperWall[2];
GBitmap* m_halfLowerWall[2];

GBitmap* getSprite(int _x, int _y, int _w, int _h) {
  return gbitmap_create_as_sub_bitmap(m_spriteMap, GRect(SIZE * _x, SIZE * _y, SIZE * _w, SIZE * _h));
}

GBitmap* getClutter(bool _broken) {
  return m_clutterSprite[ _broken == true ? 2 : rand() % MAX_CLUTTER ];
}

GBitmap* getOuterWall(int _d) {
  int _r = rand() % 3;
  switch (_d) {
    case 0:  return m_UOuterWall[_r];
    case 1:  return m_ROuterWall[_r];
    case 2:  return m_DOuterWall[_r];
    default: return m_LOuterWall[_r];
  }
}

GBitmap* getFloor(bool _random, int8_t _level) {
  int _r = rand() % MAX_FLOOR;
  if (_random == false) _r = 0;
  switch (_level) {
    case 0:  return m_floorA[_r];
    case 1:  return m_floorB[_r];
    default: return m_floorC[_r];
  }
}

void initSprite() {
  #ifdef YCGBv2
  m_ycgb = gbitmap_create_with_resource(RESOURCE_ID_YCGB);
  m_wheelFrame = gbitmap_create_with_resource(RESOURCE_ID_WHEELFRAME);
  m_warning = gbitmap_create_with_resource(RESOURCE_ID_WARNING);
  #endif

  m_spriteMap = gbitmap_create_with_resource(RESOURCE_ID_SPRITESHEET);
  m_grave = gbitmap_create_with_resource(RESOURCE_ID_GRAVE);
  m_treasureBanner = gbitmap_create_with_resource(RESOURCE_ID_TREASURE);

  m_saw = gbitmap_create_with_resource(RESOURCE_ID_SAW);
  uint8_t _sawSize = 80;
  #ifdef HIGH_RES
    _sawSize = 112;
  #endif
  m_sawA = gbitmap_create_as_sub_bitmap(m_saw, GRect(0, 0, _sawSize, _sawSize/2));
  m_sawB = gbitmap_create_as_sub_bitmap(m_saw, GRect(0, _sawSize/2, _sawSize, _sawSize/2));

  // Items from spritesheet come below

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
    m_playerSprite[_p] = getSprite(16 + (_p*2), (_p  > 3 ? 4 : 14), 2, 2);
  }

  int _clutter = 0;
  for (int _c = 0; _c < N_SMALL_CLUTTER; ++_c) {
    m_clutterSprite[_clutter++] = getSprite(_c*2, 14, 2, 2);
  }
  m_clutterSprite[_clutter++] = getSprite(24, 10,  2, 3); // med 1
  m_clutterSprite[_clutter++] = getSprite(26, 10,  2, 3); // med 2

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

  m_chest[0] = getSprite(18, 12, 2, 2);
  m_chest[1] = getSprite(8, 10, 2, 2);
  m_table = getSprite(4, 10, 4, 2);
  m_black = getSprite(2, 2, 2, 2);

  m_treasure[0] = getSprite(16, 10, 2, 2);
  m_treasure[1] = getSprite(18, 10, 2, 2);
  m_treasure[2] = getSprite(23, 12, 1, 1);

  #ifdef YCGBv2
    m_fuse[0] = getSprite(28, 0, 2, 2);
    m_fuse[1] = getSprite(28, 2, 2, 2);
    m_fuse[2] = getSprite(28, 4, 2, 2);

    m_parchment[0] = getSprite(24, 13, 1, 3);
    m_parchment[1] = getSprite(25, 13, 1, 3);

    m_clack[0] = getSprite(28, 6, 2, 2);
    m_clack[1] = getSprite(28, 8, 2, 2);

    for (uint8_t _i = 0; _i < kNOptionTypes; ++_i) {
      m_floorArrow[_i] = getSprite(30, 2*_i, 2, 2);
    }

    for (uint8_t _i = 0; _i < kNWheelIcons; ++_i) {
      m_wheelIcon[_i] = getSprite(32, 2*_i, 2, 2);
    }

    m_spear = getSprite(28, 10, 2, 6);
    m_spearHole[0] = getSprite(26, 13, 2, 1);
    m_spearHole[1] = getSprite(26, 14, 2, 1);

    m_octagon = getSprite(32, 12, 2, 2);
    m_crack = getSprite(32, 14, 2, 2);
  #endif

}

void deinitSprite() {

  for (uint8_t _i = 1; _i < 4; ++_i) {
    gbitmap_destroy(m_outerCorner[_i]);
    gbitmap_destroy(m_innerCorner[_i]);
    gbitmap_destroy(m_innerWall[_i]);
    gbitmap_destroy(m_torchWall[_i]);

    if (_i >= 3) continue;
    gbitmap_destroy(m_UOuterWall[_i]);
    gbitmap_destroy(m_DOuterWall[_i]);
    gbitmap_destroy(m_LOuterWall[_i]);
    gbitmap_destroy(m_ROuterWall[_i]);

    gbitmap_destroy(m_tapestrySprite[_i]);
    gbitmap_destroy(m_blockWall[_i]);
    gbitmap_destroy(m_bridge[_i]);
    gbitmap_destroy(m_flagstone[_i]);
    gbitmap_destroy(m_treasure[_i]);

    if (_i >= 2) continue;
    gbitmap_destroy(m_fire[_i]);

    gbitmap_destroy(m_halfUpperWall[_i]);
    gbitmap_destroy(m_halfLowerWall[_i]);
  }

  for (uint8_t _i = 1; _i < MAX_FLOOR; ++_i) {
    gbitmap_destroy(m_floorA[_i]);
    gbitmap_destroy(m_floorB[_i]);
    gbitmap_destroy(m_floorC[_i]);
  }

  for (uint8_t _i = 1; _i < MAX_GREEK; ++_i) gbitmap_destroy(m_greek[_i]);
  for (uint8_t _i = 1; _i < MAX_CLUTTER; ++_i) gbitmap_destroy(m_clutterSprite[_i]);
  for (uint8_t _i = 1; _i < MAX_FRAMES; ++_i) gbitmap_destroy(m_playerSprite[_i]);
  for (uint8_t _i = 1; _i < MAX_SYMBOL; ++_i) gbitmap_destroy(m_symbol[_i]);

  gbitmap_destroy(m_sawA);
  gbitmap_destroy(m_sawB);

  gbitmap_destroy(m_LOpenDoor);
  gbitmap_destroy(m_ROpenDoor);
  gbitmap_destroy(m_RShutDoor);
  gbitmap_destroy(m_LDoorstep);
  gbitmap_destroy(m_RDoorstep);
  gbitmap_destroy(m_stairs);
  gbitmap_destroy(m_arrow);
  gbitmap_destroy(m_shieldSprite);

  gbitmap_destroy(m_guardian);
  gbitmap_destroy(m_block);
  gbitmap_destroy(m_chest[0]);
  gbitmap_destroy(m_chest[1]);
  gbitmap_destroy(m_table);
  gbitmap_destroy(m_black);

  #ifdef YCGBv2
    for (uint8_t _i = 1; _i < 3; ++_i) gbitmap_destroy(m_fuse[_i]);
    for (uint8_t _i = 1; _i < kNOptionTypes; ++_i) gbitmap_destroy(m_floorArrow[_i]);
    for (uint8_t _i = 0; _i < kNWheelIcons; ++_i) gbitmap_destroy(m_wheelIcon[_i]);
    for (uint8_t _i = 1; _i < 2; ++_i) gbitmap_destroy(m_parchment[_i]);
    for (uint8_t _i = 1; _i < 2; ++_i) gbitmap_destroy(m_clack[_i]);
    for (uint8_t _i = 1; _i < 2; ++_i) gbitmap_destroy(m_spearHole[_i]);
    gbitmap_destroy(m_spear);
    gbitmap_destroy(m_crack);
    gbitmap_destroy(m_octagon);
  #endif

  // Actual bitmaps
  gbitmap_destroy(m_spriteMap);
  gbitmap_destroy(m_grave);
  gbitmap_destroy(m_treasureBanner);
  gbitmap_destroy(m_saw);

  #ifdef YCGBv2
    gbitmap_destroy(m_ycgb);
    gbitmap_destroy(m_wheelFrame);
    gbitmap_destroy(m_warning);
  #endif
}
