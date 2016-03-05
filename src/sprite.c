#include "sprite.h"


GBitmap* m_spriteMap;

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
GBitmap* m_fire;


GBitmap* getSprite(int _x, int _y, int _w, int _h) {
  return gbitmap_create_as_sub_bitmap(m_spriteMap, GRect(SIZE * _x, SIZE * _y, SIZE * _w, SIZE * _h));
}

GBitmap* getClutter(bool _broken) {
  return m_clutterSprite[ rand() % (_broken == true ? 3 :MAX_CLUTTER) ];
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

GBitmap* getFloor() {
  int _r = rand() % MAX_FLOOR;
  switch (m_dungeon.m_level) {
    case 0:  return m_floorA[_r];
    case 1:  return m_floorB[_r];
    default: return m_floorC[_r];
  }
}

void initSprite() {
  m_spriteMap = gbitmap_create_with_resource(RESOURCE_ID_SPRITESHEET);
  m_grave = gbitmap_create_with_resource(RESOURCE_ID_GRAVE);

  for (int _w = 0; _w < 3; ++_w) { // Walls
    m_UOuterWall[_w] = getSprite(_w + 0, 6, 2, 2);
    m_DOuterWall[_w] = getSprite(_w + 4, 8, 2, 2);
    m_ROuterWall[_w] = getSprite(0, _w + 9, 2, 2);
    m_LOuterWall[_w] = getSprite(2, _w + 8, 2, 2);
  }

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

  for (int _p = 0; _p < MAX_FRAMES; ++_p) {
    m_playerSprite[_p] = getSprite(16 + (_p*2), 14, 2, 2);
  }

  int _clutter = 0;
  for (int _c = 0; _c < N_SMALL_CLUTTER; ++_c) {
    m_clutterSprite[_clutter++] = getSprite(_c*2, 14, 2, 2);
  }
  m_clutterSprite[_clutter++] = getSprite(16, 10,  2, 4); // med 1
  m_clutterSprite[_clutter++] = getSprite(18, 10,  2, 4); // med 2

  m_tapestrySprite[0] = getSprite(20, 12,  1, 2);
  m_tapestrySprite[1] = getSprite(21, 12,  1, 2);
  m_tapestrySprite[2] = getSprite(22, 12,  1, 2);
  m_shieldSprite = getSprite(20, 10,  4, 2);

  m_guardian = getSprite(20, 0, 4, 6);
  m_blockWall[0] = getSprite(12, 6, 2, 2);
  m_blockWall[1] = getSprite(12, 8, 2, 2);
  m_blockWall[2] = getSprite(12, 10, 2, 2);
  m_block = getSprite(10, 10, 2, 2);
  m_fire = getSprite(8, 12, 2, 2);

  m_bridge[0] = getSprite(6, 4, 1, 2);
  m_bridge[1] = getSprite(9, 6, 1, 2);
  m_bridge[2] = getSprite(14, 10, 1, 2);

}

void deinitSprite() {
  gbitmap_destroy(m_spriteMap);
  gbitmap_destroy(m_grave);
}