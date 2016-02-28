#include "sprite.h"

#define MAX_FLOOR 8

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

GBitmap* getSprite(int _x, int _y, int _w, int _h) {
  return gbitmap_create_as_sub_bitmap(m_spriteMap, GRect(SIZE * _x, SIZE * _y, SIZE * _w, SIZE * _h));
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
  m_innerCorner[2] = getSprite(0, 4, 2, 2);
  m_innerCorner[3] = getSprite(4, 4, 2, 2);

  m_innerWall[0] = getSprite(0, 2, 2, 2);
  m_innerWall[1] = getSprite(4, 2, 2, 2);
  m_innerWall[2] = getSprite(2, 4, 2, 2);
  m_innerWall[3] = getSprite(0, 2, 2, 2);

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

  m_LOpenDoor = getSprite(31, 6, 3, 4);
  m_ROpenDoor = getSprite(26, 6, 3, 4);
  m_RShutDoor = getSprite(22, 6, 3, 4);

  m_LDoorstep =  getSprite(37, 0, 1, 4);
  m_RDoorstep =  getSprite(36, 0, 1, 4);


  for (int _g = 0; _g < MAX_GREEK/2; ++_g) {
    m_greek[_g]               = getSprite(4 + _g, 12, 1, 1);
    m_greek[_g + MAX_GREEK/2] = getSprite(4 + _g, 13, 1, 1);
  }

  for (int _p = 0; _p < MAX_FRAMES; ++_p) {
    m_playerSprite[_p] = getSprite(20 + (_p*2), 14, 2, 2);
  }

}

void deinitSprite() {

}
