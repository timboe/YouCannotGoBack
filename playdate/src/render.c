#include <math.h>
#include "render.h"
#include "sound.h"
#include "patterns.h"

#include "pd_api.h"

void drawBitmap(PlaydateAPI* _pd, LCDBitmap* _bitmap, int _x, int _y) {
  drawBitmapAbs(_pd, _bitmap, _x * SIZE, _y * SIZE);
}

void drawBitmapAbs(PlaydateAPI* _pd, LCDBitmap* _bitmap, int _x, int _y) {
  _pd->graphics->drawBitmap(_bitmap, _x, _y, 0);
}

void drawBitmapAbsRot(PlaydateAPI* _pd, LCDBitmap* _bitmap, int _x, int _y, float _rot) {
  _pd->graphics->drawRotatedBitmap(/*LCDBitmap**/ _bitmap,
    /*x*/ _x, /*y*/ _y, /*degrees*/ _rot,
    /*centerx*/ 0.5f, /*centery*/ 0.5f, /*xscale*/ 1.0f, /*yscale*/ 1.0f);
}

void drawCBitmap(PlaydateAPI* _pd, struct CBitmap* _cbitmap, int _x, int _y) {
  for (int _ww = 0; _ww < _cbitmap->w; ++_ww) {
    for (int _hh = 0; _hh < _cbitmap->h; ++_hh) {
      const int _loc = (_hh * CBITMAP_MAX) + _ww;
      LCDBitmap* _region = _cbitmap->bitmap[_loc];
      drawBitmap(_pd, _region, _x + _ww, _y + _hh);
    }
  }
}

void drawCBitmapAbs(PlaydateAPI* _pd, struct CBitmap* _cbitmap, int _x, int _y) {
  for (int _ww = 0; _ww < _cbitmap->w; ++_ww) {
    for (int _hh = 0; _hh < _cbitmap->h; ++_hh) {
      const int _loc = (_hh * CBITMAP_MAX) + _ww;
      LCDBitmap* _region = _cbitmap->bitmap[_loc];
      drawBitmapAbs(_pd, _region, _x + (_ww * SIZE), _y + (_hh * SIZE));
    }
  }
}

void renderGameFrame(PlaydateAPI* _pd) {
  drawBitmapAbs(_pd, m_borderL, 0, 0);
  drawBitmapAbs(_pd, m_borderR, 144+128, 0);
  _pd->graphics->fillRect(128, 0, 144, 36, kColorBlack);
  _pd->graphics->fillRect(128, 52+168, 144, 20, kColorBlack);
  static const char _portraitA[] = "Best";
  static const char _portraitB[] = "Played In";
  static const char _portraitC[] = "Portrait";
  static const char _portraitD[] = "Mode!";
  static const char _portraitVersion[] = "v1.1";
  if (m_dungeon.m_rooms[ m_dungeon.m_level ][ m_dungeon.m_room ] == kStart) {
    PDRect _vb = {.x = 32+16, .y = 0, .width = 32, .height = 16};
    renderText(_pd, _portraitVersion, _vb);
    PDRect _b = {.x = 16, .y = 32, .width = 96, .height = 128+8};
    renderTextInFrame(_pd, _portraitA, _b);
    _b.y += 16;
    renderText(_pd, _portraitB, _b);
    _b.y += 16;
    renderText(_pd, _portraitC, _b);
    _b.y += 16;
    renderText(_pd, _portraitD, _b);
    static float rot = 0;
    rot -= 4.0f;
    if (rot < -90.0f) rot = 90.0f;
    drawBitmapAbsRot(_pd, m_rotate, 64, 128+5, rot > 0.0f ? -90.0f : rot);
  }
}

void renderClear(PlaydateAPI* _pd, bool transparentCentre) {
  if (transparentCentre) {
    _pd->graphics->clear((uintptr_t)kColorChekerboard);
    _pd->graphics->fillRect(0, 0, SIZE*18, SIZE*2, kColorBlack);
    _pd->graphics->fillRect(0, 0, SIZE*2, SIZE*21, kColorBlack);
    _pd->graphics->fillRect(0, SIZE*20, SIZE*18, SIZE*(2+1), kColorBlack); // Extend the bottom
    _pd->graphics->fillRect(SIZE*17, 0, SIZE*1, SIZE*21, kColorBlack); 
  } else {
    _pd->graphics->clear((uintptr_t)kColorChekerboard);
    _pd->graphics->fillRect(0, 0, SIZE*19, SIZE*24, kColorBlack);
  }
}

void renderBlack(PlaydateAPI* _pd) {
  _pd->graphics->fillRect(0, 0, SIZE*19, SIZE*24, kColorBlack);
}


void renderArrows(PlaydateAPI* _pd, int8_t _x, int8_t _yStart, int8_t _yAdd) {
  renderArrows2(_pd, _x, _yStart, _yAdd, 1, 1, 1, false);
}

void renderArrows2(PlaydateAPI* _pd, int8_t _x, int8_t _yStart, int8_t _yAdd, bool _0, bool _1, bool _2, bool _force) {
  if ((getGameState() == kAwaitInput || getGameState() == kLevelSpecificWButtons) && (_force || getFrameCount() < ANIM_FPS/2)) {
    if (_0) drawCBitmap(_pd, &m_arrow_u, _x, _yStart);
    if (_1) drawCBitmap(_pd, &m_arrow_r, _x, _yStart + _yAdd);
    if (_2) drawCBitmap(_pd, &m_arrow_d, _x, _yStart + _yAdd + _yAdd);
    //if (getFrameCount() == 0) beepSound();
  }
}

void renderHintNumber(PlaydateAPI* _pd, PDRect _r, int _value, bool _invert) {
  static char _hintText[8];
  snprintf(_hintText, 8, "%i", _value);
  renderBorderText(_pd, _r, m_fontMain, _hintText, 1, /*GTextAlignmentCenter,*/ _invert);
}

void renderClutter(PlaydateAPI* _pd) {
  Hints_t _hint = m_dungeon.m_roomGiveHint[ m_dungeon.m_level ][ m_dungeon.m_room];
  int _hintValue = m_dungeon.m_roomGiveHintValue[ m_dungeon.m_level ][ m_dungeon.m_room];
  for (int _c = 0; _c < m_clutter.m_nClutter; ++_c) {
    if (_c == 0 && _hint == kGreekLetter) {
      drawCBitmap(_pd, getClutter(true), m_clutter.m_position_x[_c], m_clutter.m_position_y[_c]);
      int _px = (m_clutter.m_position_x[_c] * SIZE) + 4;
      int _py = (m_clutter.m_position_y[_c] * SIZE) + 2;
      if (getFlash(false)) _pd->graphics->setDrawMode(kDrawModeInverted);
      drawCBitmapAbs(_pd, &m_greek[ _hintValue ], _px, _py);
      _pd->graphics->setDrawMode(kDrawModeCopy);
    } else if (_c == 0 && _hint == kNumber) {
      drawCBitmap(_pd, getClutter(true), m_clutter.m_position_x[_c], m_clutter.m_position_y[_c]);
      PDRect r = {.x = m_clutter.m_position_x[_c] * SIZE, .y = (m_clutter.m_position_y[_c] * SIZE)+3, .width = 16, .height = 16}; // TODO tune .y
      renderHintNumber(_pd, r, _hintValue, /*invert*/getFlash(false));
    } else {
      drawCBitmap(_pd, getClutter(false), m_clutter.m_position_x[_c], m_clutter.m_position_y[_c]);
    }
  }
}

void renderProgressBar(PlaydateAPI* _pd, bool isRotated) {
  _pd->graphics->setLineCapStyle(kLineCapStyleRound);
  if (isRotated) {
    const static PDRect _b = {.x = 0, .y = 0, .width = 400/2, .height = 240/2}; // We are scaled in
    // Based on playdate screen (rendered late, ignores side-scroll)
    int _x1 = SIZE;
    int _y1 = 0;
    int _x2 = SIZE;
    int _y2 = ( _b.height * m_dungeon.m_roomsVisited ) / m_dungeon.m_totalRooms;
    _pd->graphics->drawLine(_x1, _y1, _x1, _y2, /*width=*/ 4, kColorWhite);
  } else {
    const static PDRect _b = {.x = 0, .y = 0, .width = 144, .height = 168}; 
    // Based on pebble screen
    int _x1 = 0;
    int _w = _b.width;
    int _x2 = ( _w * m_dungeon.m_roomsVisited ) / m_dungeon.m_totalRooms;
    int _h = _b.height;
    _pd->graphics->drawLine(_x1, _h, _x1 + _x2, _h, /*width=*/ 4, kColorWhite);
  }
}


void renderStandingStoneGrid(PlaydateAPI* _pd, int8_t* _coloursA, int8_t* _coloursB, int8_t* _coloursC) {

  renderLinePath(_pd, 4, 10,   7, 6); // to upper
  renderLinePath(_pd, 4, 10,   15, 10); // straight accross
  renderLinePath(_pd, 4, 10,   7, 14); // to lower

  renderLinePath(_pd, 7, 6,    15, 6); //upper accross
  renderLinePath(_pd, 7, 14,   15, 14); // lower accross

  renderLinePath(_pd, 11, 6,    11, 14); // middle down

  renderLinePath(_pd, 7, 6,    15, 14); // cross \.
  renderLinePath(_pd, 7, 14,   15, 6); // cross /

  renderLinePath(_pd, 7, 10,   11, 6);  //diamond
  renderLinePath(_pd, 7, 10,   11, 14);
  renderLinePath(_pd, 11, 6,   15, 10);
  renderLinePath(_pd, 11, 14,  15, 10);

  renderStandingStone(_pd, 4, 10, kColorWhite);

  for (int _s = 0; _s < 3; ++_s) {
    renderStandingStone(_pd, 7, 6 + (4 * _s), getShieldColor(_coloursA[_s])); // Top row
    renderStandingStone(_pd, 11, 6 + (4 * _s), getShieldColor(_coloursB[_s])); // Middle row
    renderStandingStone(_pd, 15, 6 + (4 * _s), getShieldColor(_coloursC[_s])); // Bottom row
  }

}

void renderLinePath(PlaydateAPI* _pd, int _x1, int _y1, int _x2, int _y2) {
  // Draw GColorDarkGray width=7 then GColorLightGray width=3
  _pd->graphics->drawLine(_x1*SIZE, _y1*SIZE, _x2*SIZE, _y2*SIZE, /*width=*/ 7, kColorBlack); // TODO, use gray
  _pd->graphics->drawLine(_x1*SIZE, _y1*SIZE, _x2*SIZE, _y2*SIZE, /*width=*/ 3, kColorWhite);
}

void renderStandingStone(PlaydateAPI* _pd, int _x1, int _y1, LCDColor _c) {
  // Fill GColorLightGray, then SIZE-2 with GColorBlack then SIZE-4 with c
  _pd->graphics->fillEllipse(_x1*SIZE - SIZE+0, _y1*SIZE - SIZE+0, (SIZE*2)-0, (SIZE*2)-0, 0, 0, kColorWhite);
  _pd->graphics->fillEllipse(_x1*SIZE - SIZE+1, _y1*SIZE - SIZE+1, (SIZE*2)-2, (SIZE*2)-2, 0, 0, kColorBlack);
  _pd->graphics->fillEllipse(_x1*SIZE - SIZE+2, _y1*SIZE - SIZE+2, (SIZE*2)-4, (SIZE*2)-4, 0, 0, _c);
}

void renderBoxGrid(PlaydateAPI* _pd, int8_t* _coloursA, int8_t* _coloursB, int8_t* _coloursC, int8_t* _offset) {
  uint8_t _o = 0;
  for (int _s = 0; _s < 3; ++_s) {
    renderBoxGridBox(_pd, 5, 5 + (4 * _s), getShieldColor(_coloursA[_s]), _offset[_o++]); // Top row
    renderBoxGridBox(_pd, 8, 5 + (4 * _s), getShieldColor(_coloursB[_s]), _offset[_o++]); // Middle row
    renderBoxGridBox(_pd, 11, 5 + (4 * _s), getShieldColor(_coloursC[_s]), _offset[_o++]); // Bottom row
  }

}

void renderBoxGridBox(PlaydateAPI* _pd, int _x1, int _y1, LCDColor _c, int8_t _offset) {
  drawCBitmapAbs(_pd, &m_clutterSprite[2], _x1*SIZE + _offset, _y1*SIZE + _offset);
  _pd->graphics->fillRect(_x1*SIZE + 4 + _offset, _y1*SIZE + 2 + _offset, SIZE, SIZE, _c);
}


void renderGreekText(PlaydateAPI* _pd, const char* _msg, int _i) {
  PDRect _b = {.x = 8*SIZE, .y = 5*SIZE, .width = 6*SIZE, .height =2*SIZE};
  _b.y += 4*_i*SIZE;
  const static int _text_y_offset = 2;
  _pd->graphics->setDrawMode(kDrawModeFillBlack);
  _pd->graphics->setFont(m_fontGreek); /// xxx
  int _len = _pd->graphics->getTextWidth(m_fontGreek, _msg, strlen(_msg), kUTF8Encoding, /*tracking*/ 0);
  _b.x += (_b.width - _len)/2;
  _pd->graphics->drawText(_msg, strlen(_msg), kUTF8Encoding, _b.x, _b.y + _text_y_offset);
  _pd->graphics->setDrawMode(kDrawModeCopy);
}

void renderGreekFrames(PlaydateAPI* _pd, const char* _a, const char* _b, const char* _c) {
  PDRect _rect = {.x = 8*SIZE, .y = 5*SIZE, .width = 6*SIZE, .height =2*SIZE};
  for (int _i = 0; _i < 3; ++_i) {
    _pd->graphics->fillRect(_rect.x, _rect.y, _rect.width, _rect.height, kColorWhite);
    _pd->graphics->drawRect(_rect.x+2, _rect.y+2, _rect.width-4, _rect.height-4, kColorBlack);
    _rect.y += 4*SIZE;
  }
  renderGreekText(_pd, _a, 0);
  renderGreekText(_pd, _b, 1);
  renderGreekText(_pd, _c, 2);

}

void renderFrame(PlaydateAPI* _pd, PDRect _b) {
  _pd->graphics->fillRect(_b.x, _b.y, _b.width, _b.height, kColorBlack);
  _pd->graphics->drawRect(_b.x+2, _b.y+2, _b.width-4, _b.height-4, kColorWhite);
}

void renderTextInFrame(PlaydateAPI* _pd, const char* _msg, PDRect _b) {
  const static int _text_y_offset = SIZE*2;
  _pd->graphics->fillRect(_b.x, _b.y, _b.width, _b.height, kColorWhite);
  _pd->graphics->drawRect(_b.x+2, _b.y+2, _b.width-4, _b.height-4, kColorBlack);
  renderText(_pd, _msg, _b);
}

void renderText(PlaydateAPI* _pd, const char* _msg, PDRect _b) {
  const static int _text_y_offset = SIZE*2;
  _pd->graphics->setDrawMode(kDrawModeFillBlack);
  _pd->graphics->setFont(m_fontMsg);
  int _len = _pd->graphics->getTextWidth(m_fontMsg, _msg, strlen(_msg), kASCIIEncoding, /*tracking*/ 0);
  _b.x += (_b.width - _len)/2;
  _pd->graphics->drawText(_msg, strlen(_msg), kASCIIEncoding, _b.x, _b.y + _text_y_offset);
  _pd->graphics->setDrawMode(kDrawModeCopy);
}

void renderMessage(PlaydateAPI* _pd, const char* _msg, bool _isRotated) {
  int _x = _isRotated ? SIZE*3 : 0;
  int _w = _isRotated ? 15*SIZE : 18*SIZE;
  PDRect _b = {.x = _x, .y = 8*SIZE, .width = _w, .height = 5*SIZE};
  _pd->graphics->setFont(m_fontMain);
  renderTextInFrame(_pd, _msg, _b);
}

void renderWalls(PlaydateAPI* _pd, bool _l, bool _rA, bool _rB, bool _rC) {
  drawCBitmap(_pd, &m_outerCorner[0], 1, 0);
  drawCBitmap(_pd, &m_outerCorner[1], 15, 0);
  drawCBitmap(_pd, &m_outerCorner[2], 1, 18);
  drawCBitmap(_pd, &m_outerCorner[3], 15, 18);
  bool _torches = (rand() % 2 == 0);
  for (int _x = 3; _x < 15; _x += 2) {  //Draw top and bottom wall
    if (_torches && (_x == 5 || _x == 11)) {
      drawCBitmap(_pd, &m_torchWall[0], _x, 0);
      drawCBitmap(_pd, &m_torchWall[2], _x, 18);
    } else {
      drawCBitmap(_pd, getOuterWall(0), _x, 0); // top
      drawCBitmap(_pd, getOuterWall(2), _x, 18);
    }
  }
  _torches = (rand() % 2 == 0);
  for (int _y = 2; _y < 18; _y += 2) { // Draw left wall
    if (_y == 8 && _l == true) {
      drawCBitmap(_pd, &m_LOpenDoor, 0,  _y);
      _y += 2;
    } else if (_torches && (_y == 6 || _y == 12)) {
      drawCBitmap(_pd, &m_torchWall[1], 0,  _y);
    } else {
      drawCBitmap(_pd, getOuterWall(1), 1,  _y);
    }
  }
  _torches = (rand() % 2 == 0);
  for (int _y = 2; _y < 18; _y += 2) { // draw right wall
    int _open = rand() % 2; // zero=wall, 1=closed door, 2=open door
    switch (_y) {
      case 4:  if (_rA == true) _open = 2; break;
      case 8:  if (_rB == true) _open = 2; break;
      case 12: if (_rC == true) _open = 2; break;
      default: _open = 0; break;
    }
    if (_open == 1) {
      drawCBitmap(_pd, &m_RShutDoor, 15,  _y);
      _y += 2;
    } else if (_open == 2) {
      drawCBitmap(_pd, &m_ROpenDoor, 15,  _y);
      _y += 2;
    } else if (_torches && (_y == 2 || _y == 16)) {
      drawCBitmap(_pd, &m_torchWall[3], 15,  _y);
    } else {
      drawCBitmap(_pd, getOuterWall(3), 15, _y);
    }
  }
}

void renderWallClutter(PlaydateAPI* _pd) {
    Hints_t _hint = m_dungeon.m_roomGiveHint[ m_dungeon.m_level ][ m_dungeon.m_room];
    int _hintValue = m_dungeon.m_roomGiveHintValue[ m_dungeon.m_level ][ m_dungeon.m_room];
    int _r = 4 + (rand()%6);
    if (_hint == kShield) {   // Check shield
      //GPoint _p = GPoint((_r + 1) * SIZE, SIZE);
      int _px = (_r + 1) * SIZE;
      int _py = SIZE;
      int _w = 8;
      if (getFlash(false)) _pd->graphics->setDrawMode(kDrawModeInverted);
      drawCBitmap(_pd, &m_shieldSprite, _r, 0);
      _pd->graphics->setDrawMode(kDrawModeCopy);
      _pd->graphics->fillEllipse(_px - _w/2, _py - _w/2, /*sizeH*/_w, /*sizeV*/_w, /*aStart*/0, /*aEnd*/0, getShieldColor(getShieldA(_hintValue)));
      _pd->graphics->drawEllipse(_px - _w/2, _py - _w/2, _w, _w, /*lineWidth*/1, 0, 0, kColorBlack);      
      _px += SIZE*2;
      _pd->graphics->fillEllipse(_px - _w/2, _py - _w/2, /*sizeH*/_w, /*sizeV*/_w, /*aStart*/0, /*aEnd*/0, getShieldColor(getShieldC(_hintValue)));
      _pd->graphics->drawEllipse(_px - _w/2, _py - _w/2, _w, _w, /*lineWidth*/1, 0, 0, kColorBlack); 
      _px -= SIZE;
      _pd->graphics->fillEllipse(_px - _w/2, _py - _w/2, /*sizeH*/_w, /*sizeV*/_w, /*aStart*/0, /*aEnd*/0, getShieldColor(getShieldB(_hintValue)));
      _pd->graphics->drawEllipse(_px - _w/2, _py - _w/2, _w, _w, /*lineWidth*/1, 0, 0, kColorBlack); 
    } else if (_hint == kSpell) { // Check spell
      drawCBitmap(_pd, &m_tapestrySprite[0], _r, 0);
      for (int _i=1; _i<5; ++_i) drawCBitmap(_pd, &m_tapestrySprite[1], _r+_i, 0);
      drawCBitmap(_pd, &m_tapestrySprite[2], _r+5, 0);
      PDRect rect =  {.x = _r * SIZE, .y = 4, .width = 48, .height = 16};
      renderBorderText(_pd, rect, m_fontMain, m_spellNames[_hintValue], 1, /*invert*/getFlash(false));
    } else if (_hint == kSymbol) { // Check symbol
      if (getFlash(false)) _pd->graphics->setDrawMode(kDrawModeInverted);
      drawCBitmap(_pd, &m_symbol[_hintValue], _r, 18);
      _pd->graphics->setDrawMode(kDrawModeCopy);
    }
}

void renderSawFloor(PlaydateAPI* _pd, int8_t _offset) {
  const int8_t _level = m_dungeon.m_level;
  for (int _x = 0; _x < 20; _x += 2) {
    for (int _y = 6; _y < 12; _y += 2) {
      drawCBitmapAbs(_pd, getFloor(false, _level), (_x*SIZE) - _offset, _y*SIZE);
    }
    drawCBitmapAbs(_pd, &m_halfUpperWall[0], (_x*SIZE) - _offset, 5*SIZE);
    drawCBitmapAbs(_pd, &m_halfLowerWall[0], (_x*SIZE) - _offset, 12*SIZE);
  }
}

void renderSawWalls(PlaydateAPI* _pd, int8_t _offset) {
  _pd->graphics->fillRect(0, 0, SIZE*19, SIZE*2, kColorBlack);
  _pd->graphics->fillRect(0, SIZE*15, SIZE*19, SIZE*6, kColorBlack);
  for (int _x = 0; _x < 20; _x += 2) {
    drawCBitmapAbs(_pd, &m_halfUpperWall[1], (_x*SIZE) - _offset, 4*SIZE);
    drawCBitmapAbs(_pd, &m_halfLowerWall[1], (_x*SIZE) - _offset, 13*SIZE);
    drawCBitmapAbs(_pd, &m_black, (_x*SIZE) - _offset, 2*SIZE);
    drawCBitmapAbs(_pd, &m_black, (_x*SIZE) - _offset, 14*SIZE);

  }
}

void renderFloor(PlaydateAPI* _pd, int _mode) {
  const int8_t _level = m_dungeon.m_level;
  for (int _x = 3; _x < 15; _x += 2) {
    if (_mode == 1 && !(_x == 3 || _x == 13)) continue; // Pit
    const int8_t _l = (_mode == 2 && (_x == 5 || _x == 9 || _x == 13)) ? (_level + 1) % 3 : _level; // spikes
    for (int _y = 2; _y < 18; _y += 2) {
      drawCBitmap(_pd, getFloor(true, _l), _x, _y);
    }
  }
  if (_mode == 1) { // Pit with one space on either side
    for (int _y = 4; _y < 16; _y += 2) {
      drawCBitmap(_pd, &m_innerWall[0], 5,  _y);
      drawCBitmap(_pd, &m_innerWall[1], 11, _y);
    }
    drawCBitmap(_pd, &m_innerCorner[0], 5,  2);
    drawCBitmap(_pd, &m_innerCorner[1], 11, 2);
    drawCBitmap(_pd, &m_innerCorner[2], 11, 16);
    drawCBitmap(_pd, &m_innerCorner[3], 5,  16);
    drawCBitmap(_pd, &m_innerWall[2], 7, 2);
    drawCBitmap(_pd, &m_innerWall[2], 9, 2);
    drawCBitmap(_pd, &m_innerWall[3], 7, 16);
    drawCBitmap(_pd, &m_innerWall[3], 9, 16);
  }
  // Extra bits where the doors can go
  drawCBitmap(_pd, &m_LDoorstep, 2, 8); // TODO this on its own call
  drawCBitmap(_pd, &m_RDoorstep, 15, 4);
  drawCBitmap(_pd, &m_RDoorstep, 15, 8);
  drawCBitmap(_pd, &m_RDoorstep, 15, 12);
}

void renderPit(PlaydateAPI* _pd) {
  for (int _y = 4; _y < 16; _y += 2) {
    drawCBitmap(_pd, &m_innerWall[0], 3,  _y);
    drawCBitmap(_pd, &m_innerWall[1], 13, _y);
  }
  for (int _x = 5; _x < 15; _x += 2) {
    drawCBitmap(_pd, &m_innerWall[2], _x, 2);
    drawCBitmap(_pd, &m_innerWall[3], _x, 16);
  }
  drawCBitmap(_pd, &m_innerCorner[0], 3,  2);
  drawCBitmap(_pd, &m_innerCorner[1], 13, 2);
  drawCBitmap(_pd, &m_innerCorner[2], 13, 16);
  drawCBitmap(_pd, &m_innerCorner[3], 3,  16);

  // Extra bits where the doors can go
  drawCBitmap(_pd, &m_LDoorstep, 2, 8); // TODO this on its own call
  drawCBitmap(_pd, &m_RDoorstep, 15, 4);
  drawCBitmap(_pd, &m_RDoorstep, 15, 8);
  drawCBitmap(_pd, &m_RDoorstep, 15, 12);
}

void renderFinalPit(PlaydateAPI* _pd) {
  for (int _w = 0; _w < 4; ++_w) { //0=l, 1=r
    drawCBitmap(_pd, &m_innerWall[0], 7, 6 + (_w*2));
    drawCBitmap(_pd, &m_innerWall[1], 9, 6 + (_w*2));
    if (_w >= 2) continue;
    drawCBitmap(_pd, &m_innerWall[2], 9 + (_w*2), 2);
    drawCBitmap(_pd, &m_innerWall[3], 9 + (_w*2), 6);
    drawCBitmap(_pd, &m_innerWall[2], 9 + (_w*2), 12);
    drawCBitmap(_pd, &m_innerWall[3], 9 + (_w*2), 16);
  }

  drawCBitmap(_pd, &m_innerWall[0], 7, 4);
  drawCBitmap(_pd, &m_innerWall[1], 13, 4);

  drawCBitmap(_pd, &m_innerWall[0], 7, 14);
  drawCBitmap(_pd, &m_innerWall[1], 13, 14);

  drawCBitmap(_pd, &m_innerCorner[0], 7, 2);
  drawCBitmap(_pd, &m_innerCorner[1], 13, 2);
  drawCBitmap(_pd, &m_innerCorner[2], 13, 16);
  drawCBitmap(_pd, &m_innerCorner[3], 7, 16);

  drawCBitmap(_pd, &m_innerCorner[2], 13, 6);
  drawCBitmap(_pd, &m_innerCorner[1], 13, 12);

  drawCBitmap(_pd, &m_outerCorner[0], 9, 6);
  drawCBitmap(_pd, &m_outerCorner[2], 9, 12);

  drawCBitmap(_pd, &m_black, 9, 14);
  drawCBitmap(_pd, &m_black, 11, 14);
  drawCBitmap(_pd, &m_black, 9, 4);
  drawCBitmap(_pd, &m_black, 11, 4);
}

void renderPlayer(PlaydateAPI* _pd) {
  uint16_t _pos_x = m_player.m_position_x;
  uint16_t _pos_y = m_player.m_position_y;
  if (m_player.m_playerFrame == 1 || m_player.m_playerFrame == 4) --_pos_y;
  if (m_player.m_rotation) {
    // TODO
    drawCBitmapAbs(_pd, &m_playerSprite[ m_player.m_playerFrame ], _pos_x, _pos_y);
  } else {
    drawCBitmapAbs(_pd, &m_playerSprite[ m_player.m_playerFrame ], _pos_x, _pos_y);
  }
}

void renderBomb(PlaydateAPI* _pd, int16_t _bomb, int8_t _location) {
  const int16_t _off = _bomb == 3 ? 1 : 0;
  if (getFlash(true)) { 
    _pd->graphics->setDrawMode(kDrawModeInverted);
  }
  drawBitmap(_pd, m_bomb[_bomb > 3 ? 3 : _bomb], 10 - _off, 4 + (4 * _location) - _off);
  _pd->graphics->setDrawMode(kDrawModeCopy);

}

void renderBorderText(PlaydateAPI* _pd, PDRect _loc, LCDFont* _f, const char* _buffer, uint8_t _offset, /*GTextAlignment _al,*/ bool _invert) {
  _pd->graphics->setFont(_f);
  _pd->graphics->setDrawMode(kDrawModeFillBlack);
  if (_invert == true) _pd->graphics->setDrawMode(kDrawModeFillWhite);

  int _len = _pd->graphics->getTextWidth(_f, _buffer, strlen(_buffer), kASCIIEncoding, /*tracking*/ 0);
  _loc.x += (_loc.width - _len)/2;

  _loc.y += _offset; // CU
  _pd->graphics->drawText(_buffer, strlen(_buffer), kASCIIEncoding, _loc.x, _loc.y);
  _loc.x += _offset; // RU
  _loc.y -= _offset; // CR
  _pd->graphics->drawText(_buffer, strlen(_buffer), kASCIIEncoding, _loc.x, _loc.y);
  _loc.y -= _offset; // DR
  _loc.x -= _offset; // DC
  _pd->graphics->drawText(_buffer, strlen(_buffer), kASCIIEncoding, _loc.x, _loc.y);
  _loc.x -= _offset; // DR
  _loc.y += _offset; // CR
  _pd->graphics->drawText(_buffer, strlen(_buffer), kASCIIEncoding, _loc.x, _loc.y);

  // main
  _pd->graphics->setDrawMode(kDrawModeFillWhite);
  if (_invert == true) _pd->graphics->setDrawMode(kDrawModeFillBlack);
  _loc.x += _offset; // O
  _pd->graphics->drawText(_buffer, strlen(_buffer), kASCIIEncoding, _loc.x, _loc.y);

  // reset
  _pd->graphics->setDrawMode(kDrawModeCopy);
}

#define SPIKES 4
#define SIZE_FINE SIZE/2
void renderSpikes(PlaydateAPI* _pd, float* _off, bool _top) {

  const static int8_t s_y1[SPIKES] = {0, SIZE_FINE*7, SIZE_FINE*14, SIZE_FINE*21};

  for (int _x = 0; _x < 3; ++_x) {
    for (int _i = (_top ? 0 : 2); _i < (_top ? 2 : SPIKES); ++_i) {
      int8_t _fx = (SIZE*5) + (SIZE*4*_x);
      int _ioff = round(_off[_x]);
      _pd->graphics->fillEllipse(0 + _fx, s_y1[_i] + S_OFF - SIZE_FINE + 0, SIZE_FINE*4 - 0, SIZE_FINE*2 - 0, 0.0f, 0.0f, kColorBlack);
      _pd->graphics->fillEllipse(1 + _fx, s_y1[_i] + S_OFF - SIZE_FINE + 1, SIZE_FINE*4 - 2, SIZE_FINE*2 - 2, 0.0f, 0.0f, kColorWhite);
      _pd->graphics->fillEllipse(2 + _fx, s_y1[_i] + S_OFF - SIZE_FINE + 2, SIZE_FINE*4 - 4, SIZE_FINE*2 - 4, 0.0f, 0.0f, kColorBlack);
      _pd->graphics->setBitmapMask(m_spear, _pd->graphics->getTableBitmap(m_smask, _ioff));
      drawBitmapAbs(_pd, m_spear, _fx, s_y1[_i] + S_OFF - _ioff);
    }
  }
}


void renderFade(PlaydateAPI* _pd, bool _in, bool _isRotated) {
  if (_in == false && m_dungeon.m_fallingDeath == true) m_player.m_position_y += 4;
  if (_in == false && (m_dungeon.m_fallingDeath == true 
    || m_dungeon.m_spinningDeath == true)) m_player.m_rotation += 12.0f;
  static int s_progress = 0;
  static int s_pattern = 0;
 

  static const PDRect s_r0 =  {.x = 128, .y = 0, .width = 144, .height = 240};
  static const PDRect s_r1 =  {.x = 0, .y = 0, .width = 400, .height = 240};

  const int _flag = (_in == false ? s_progress : FADE_LEVELS - s_progress - 1 );

  LCDPattern* _p = (LCDPattern*) kPattern0[_flag];
  switch (s_pattern) {
    case 1: _p = (LCDPattern*) kPattern1[_flag]; break;
    case 2: _p = (LCDPattern*) kPattern2[_flag]; break;
  }

  if (_isRotated) {
    _pd->graphics->fillRect(s_r1.x, s_r1.y, s_r1.width, s_r1.height, (uintptr_t) _p);
  } else {
    _pd->graphics->fillRect(s_r0.x, s_r0.y, s_r0.width, s_r0.height, (uintptr_t) _p);
  }

  if (++s_progress == FADE_LEVELS) {
    s_progress = 0;
    s_pattern = rand() % PATTERNS;
    if (_in == true) setGameState(kLevelSpecific); // Done fade in, let level choose what next
    else setGameState(kNewRoom);
  }
}

uintptr_t getShieldColor(int8_t _value) {
  switch (_value) {
    case 0: return kColorBlack;
    case 1: return kColorWhite;
    case 2: return (uintptr_t)kColorChekerboard;
    default: return kColorBlack;
  }
}
