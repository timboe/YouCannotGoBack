#include "sprite.h"


LCDBitmapTable* m_spriteMap;
LCDBitmap* m_sawA;
LCDBitmap* m_sawB;
LCDBitmap* m_bomb[4];

LCDBitmap* m_borderL;
LCDBitmap* m_borderR;

LCDBitmap* m_treasureBanner;

LCDBitmap* m_grave;

LCDBitmap* m_spear;
LCDBitmapTable* m_smask;

LCDBitmap* m_fade;

LCDFont* m_fontMain;
LCDFont* m_fontMsg;
LCDFont* m_fontIntro;

struct CBitmap m_UOuterWall[3];
struct CBitmap m_DOuterWall[3];
struct CBitmap m_LOuterWall[3];
struct CBitmap m_ROuterWall[3];

struct CBitmap m_outerCorner[4];
struct CBitmap m_innerCorner[4];
struct CBitmap m_innerWall[4];
struct CBitmap m_torchWall[4];

struct CBitmap m_floorA[MAX_FLOOR];
struct CBitmap m_floorB[MAX_FLOOR];
struct CBitmap m_floorC[MAX_FLOOR];

struct CBitmap m_LOpenDoor;
struct CBitmap m_ROpenDoor;
struct CBitmap m_RShutDoor;
struct CBitmap m_LDoorstep;
struct CBitmap m_RDoorstep;

struct CBitmap m_greek[MAX_GREEK];
struct CBitmap m_playerSprite[MAX_FRAMES];
struct CBitmap m_stairs;
struct CBitmap m_arrow_u;
struct CBitmap m_arrow_r;
struct CBitmap m_arrow_d;
struct CBitmap m_clutterSprite[MAX_CLUTTER];
struct CBitmap m_tapestrySprite[3];
struct CBitmap m_shieldSprite;

struct CBitmap m_guardian;
struct CBitmap m_blockWall[3];
struct CBitmap m_block;
struct CBitmap m_bridge[3];
struct CBitmap m_fire[2];
struct CBitmap m_flagstone[3];
struct CBitmap m_chest[2];
struct CBitmap m_table;
struct CBitmap m_black;
struct CBitmap m_treasure[3];
struct CBitmap m_symbol[MAX_SYMBOL];
struct CBitmap m_halfUpperWall[2];
struct CBitmap m_halfLowerWall[2];

LCDBitmap* loadImageAtPath(PlaydateAPI* _pd, const char* _path) {
  const char* _outErr = NULL;
  LCDBitmap* _img = _pd->graphics->loadBitmap(_path, &_outErr);
  if (_outErr != NULL) {
    _pd->system->error("Error loading image at path '%s': %s", _path, _outErr);
  }
  return _img;
}

LCDBitmapTable* loadImageTableAtPath(PlaydateAPI* _pd, const char* _path) {
  const char* _outErr = NULL;
  LCDBitmapTable* _table = _pd->graphics->loadBitmapTable(_path, &_outErr);
  if (_outErr != NULL) {
    _pd->system->error("Error loading image table at path '%s': %s", _path, _outErr);
  }
  return _table;
}

LCDFont* loadFontAtPath(PlaydateAPI* _pd, const char* _path) {
  const char* _outErr = NULL;
  LCDFont* _f = _pd->graphics->loadFont(_path, &_outErr);
  if (_outErr != NULL) {
    _pd->system->error("Error loading font at path '%s': %s", _path, _outErr);
  }
  return _f;
}
 
struct CBitmap getSprite(PlaydateAPI* _pd, int _x, int _y, int _w, int _h) {
  struct CBitmap _cb;
  if (_w > CBITMAP_MAX || _h > CBITMAP_MAX) {
    _pd->system->error("Cannot request such a large sprite %i %i", _w, _h);
    return _cb;
  }
  _cb.w = _w;
  _cb.h = _h;
  for (int _ww = 0; _ww < _w; ++_ww) {
    for (int _hh = 0; _hh < _h; ++_hh) {
      const int _loc_internal = (_hh * CBITMAP_MAX) + _ww;
      const int _loc_global = ((_y + _hh) * SSHEET_X) + (_x + _ww);
      LCDBitmap* region = _pd->graphics->getTableBitmap(m_spriteMap, _loc_global);
      _cb.bitmap[_loc_internal] = region;
    }
  }
  return _cb;
}

struct CBitmap* getClutter(bool _broken) {
  if (_broken) return &m_clutterSprite[ 2 ];
  return &m_clutterSprite[ rand() % MAX_CLUTTER ];
}

struct CBitmap* getOuterWall(int _d) {
  int _r = rand() % 3;
  switch (_d) {
    case 0:  return &m_UOuterWall[_r];
    case 1:  return &m_ROuterWall[_r];
    case 2:  return &m_DOuterWall[_r];
    default: return &m_LOuterWall[_r];
  }
}

struct CBitmap* getFloor(bool _random, int8_t _level) {
  int _r = rand() % MAX_FLOOR;
  if (_random == false) _r = 0;
  switch (_level) {
    case 0:  return &m_floorA[_r];
    case 1:  return &m_floorB[_r];
    default: return &m_floorC[_r];
  }
}

void initSprite(PlaydateAPI* _pd) {
  m_grave = loadImageAtPath(_pd, "images/gameover");
  m_sawA = loadImageAtPath(_pd, "images/saw_a");
  m_sawB = loadImageAtPath(_pd, "images/saw_b");
  m_treasureBanner = loadImageAtPath(_pd, "images/treasure");
  m_borderL = loadImageAtPath(_pd, "images/border_left");
  m_borderR = loadImageAtPath(_pd, "images/border_right");

  m_bomb[0] = loadImageAtPath(_pd, "images/bomb0");
  m_bomb[1] = loadImageAtPath(_pd, "images/bomb1");
  m_bomb[2] = loadImageAtPath(_pd, "images/bomb2");
  m_bomb[3] = loadImageAtPath(_pd, "images/explosion");

  m_spear = loadImageAtPath(_pd, "images/spear");
  m_smask = loadImageTableAtPath(_pd, "images/smask/smask");

  m_fade = loadImageAtPath(_pd, "images/fadePatterns");

  m_spriteMap = loadImageTableAtPath(_pd, "images/spritesheet");

  m_fontMain = loadFontAtPath(_pd, "fonts/marblemadness_as");
  m_fontMsg = loadFontAtPath(_pd, "fonts/Roobert-9-Mono-Condensed");
  m_fontIntro = loadFontAtPath(_pd, "fonts/font-full-circle");

  _pd->graphics->setFont(m_fontMain);

  for (int _w = 0; _w < 3; ++_w) { // Walls
    m_UOuterWall[_w] = getSprite(_pd, _w + 0, 6, 2, 2);
    m_DOuterWall[_w] = getSprite(_pd, _w + 4, 8, 2, 2);
    m_ROuterWall[_w] = getSprite(_pd, 0, _w + 9, 2, 2);
    m_LOuterWall[_w] = getSprite(_pd, 2, _w + 8, 2, 2);

    m_flagstone[_w] = getSprite(_pd, 12 + (_w * 2), 12, 2, 2);
  }

  m_halfUpperWall[0] = getSprite(_pd, 2, 7, 2, 1);
  m_halfUpperWall[1] = getSprite(_pd, 2, 6, 2, 1);
  m_halfLowerWall[0] = getSprite(_pd, 4, 8, 2, 1);
  m_halfLowerWall[1] = getSprite(_pd, 4, 9, 2, 1);

  m_outerCorner[0] = getSprite(_pd, 6, 0, 2, 2);
  m_outerCorner[1] = getSprite(_pd, 8, 0, 2, 2);
  m_outerCorner[2] = getSprite(_pd, 6, 2, 2, 2);
  m_outerCorner[3] = getSprite(_pd, 8, 2, 2, 2);

  m_innerCorner[0] = getSprite(_pd, 0, 0, 2, 2);
  m_innerCorner[1] = getSprite(_pd, 4, 0, 2, 2);
  m_innerCorner[2] = getSprite(_pd, 4, 4, 2, 2);
  m_innerCorner[3] = getSprite(_pd, 0, 4, 2, 2);

  m_innerWall[0] = getSprite(_pd, 0, 2, 2, 2);
  m_innerWall[1] = getSprite(_pd, 4, 2, 2, 2);
  m_innerWall[2] = getSprite(_pd, 2, 0, 2, 2);
  m_innerWall[3] = getSprite(_pd, 2, 4, 2, 2);

  m_torchWall[0] = getSprite(_pd, 10, 2, 2, 2);
  m_torchWall[1] = getSprite(_pd, 7,  4, 3, 2);
  m_torchWall[2] = getSprite(_pd, 10, 4, 2, 3);
  m_torchWall[3] = getSprite(_pd, 6,  6, 3, 2);

  m_arrow_d = getSprite(_pd, 10, 0, 2, 2);
  m_arrow_u = getSprite(_pd, 0, 14, 2, 2);
  m_arrow_r = getSprite(_pd, 2, 14, 2, 2);

  m_stairs = getSprite(_pd, 10, 8, 2, 2);

  for (int _f = 0; _f < MAX_FLOOR; ++_f) { // Floor
    m_floorA[_f] = getSprite(_pd, 12 + (_f*2), 4, 2, 2);
    m_floorB[_f] = getSprite(_pd, 12 + (_f*2), 2, 2, 2);
    m_floorC[_f] = getSprite(_pd, 12 + (_f*2), 0, 2, 2);
  }

  m_LOpenDoor = getSprite(_pd, 23, 6, 3, 4);
  m_ROpenDoor = getSprite(_pd, 18, 6, 3, 4);
  m_RShutDoor = getSprite(_pd, 14, 6, 3, 4);

  m_LDoorstep =  getSprite(_pd, 27, 6, 1, 4);
  m_RDoorstep =  getSprite(_pd, 26, 6, 1, 4);

  for (int _g = 0; _g < MAX_GREEK/2; ++_g) {
    m_greek[_g]               = getSprite(_pd, 4 + _g, 12, 1, 1);
    m_greek[_g + MAX_GREEK/2] = getSprite(_pd, 4 + _g, 13, 1, 1);
  }

  for (int _g = 0; _g < MAX_SYMBOL/2; ++_g) {
    m_symbol[_g]                = getSprite(_pd, 24 + (_g*2), 0, 2, 2);
    m_symbol[_g + MAX_SYMBOL/2] = getSprite(_pd, 24 + (_g*2), 2, 2, 2);
  }

  int _player = 0;
  m_playerSprite[_player++] = getSprite(_pd, 28, 8, 2, 2);
  m_playerSprite[_player++] = getSprite(_pd, 30, 8, 2, 2);
  //
  m_playerSprite[_player++] = getSprite(_pd, 28, 10, 2, 2);
  m_playerSprite[_player++] = getSprite(_pd, 30, 10, 2, 2);
  //
  m_playerSprite[_player++] = getSprite(_pd, 28, 12, 2, 2);
  m_playerSprite[_player++] = getSprite(_pd, 30, 12, 2, 2);

  int _clutter = 0;
  m_clutterSprite[_clutter++] = getSprite(_pd, 28, 0, 2, 2); // small 1
  m_clutterSprite[_clutter++] = getSprite(_pd, 30, 0, 2, 2); // small 2

  m_clutterSprite[_clutter++] = getSprite(_pd, 28, 2, 2, 2); // small 3
  m_clutterSprite[_clutter++] = getSprite(_pd, 30, 2, 2, 2); // small 4

  m_clutterSprite[_clutter++] = getSprite(_pd, 28, 4, 2, 2); // small 5
  m_clutterSprite[_clutter++] = getSprite(_pd, 30, 4, 2, 2); // small 6

  m_clutterSprite[_clutter++] = getSprite(_pd, 28, 6, 2, 2); // small 7
  m_clutterSprite[_clutter++] = getSprite(_pd, 30, 6, 2, 2); // small 8

  m_clutterSprite[_clutter++] = getSprite(_pd, 24, 10,  2, 4); // med 1
  m_clutterSprite[_clutter++] = getSprite(_pd, 26, 10,  2, 4); // med 2

  m_tapestrySprite[0] = getSprite(_pd, 20, 12,  1, 2);
  m_tapestrySprite[1] = getSprite(_pd, 21, 12,  1, 2);
  m_tapestrySprite[2] = getSprite(_pd, 22, 12,  1, 2);
  m_shieldSprite = getSprite(_pd, 20, 10,  4, 2);

  m_guardian = getSprite(_pd, 20, 0, 4, 6);
  m_blockWall[0] = getSprite(_pd, 12, 6, 2, 2);
  m_blockWall[1] = getSprite(_pd, 12, 8, 2, 2);
  m_blockWall[2] = getSprite(_pd, 12, 10, 2, 2);
  m_block = getSprite(_pd, 10, 10, 2, 2);
  m_fire[0] = getSprite(_pd, 8, 12, 4, 2);
  m_fire[1] = getSprite(_pd, 10, 7, 2, 1);


  m_bridge[0] = getSprite(_pd, 6, 4, 1, 2);
  m_bridge[1] = getSprite(_pd, 9, 6, 1, 2);
  m_bridge[2] = getSprite(_pd, 14, 10, 1, 2);

  m_chest[0] = getSprite(_pd, 8, 10, 2, 2); // bad
  m_chest[1] = getSprite(_pd, 4, 14, 2, 2); // good
  m_table = getSprite(_pd, 4, 10, 4, 2);
  m_black = getSprite(_pd, 2, 2, 2, 2);

  m_treasure[0] = getSprite(_pd, 16, 10, 2, 2);
  m_treasure[1] = getSprite(_pd, 18, 10, 2, 2);
  m_treasure[2] = getSprite(_pd, 23, 12, 1, 1);

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
