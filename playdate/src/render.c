#include "render.h"
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
  _pd->graphics->fillRect(128, 36+168, 144, 36, kColorBlack);

}

void renderClear(PlaydateAPI* _pd, bool transparentCentre) {
  if (transparentCentre) {
    _pd->graphics->clear(kColorChekerboard);
    _pd->graphics->fillRect(0, 0, SIZE*18, SIZE*2, kColorBlack);
    _pd->graphics->fillRect(0, 0, SIZE*2, SIZE*21, kColorBlack);
    _pd->graphics->fillRect(0, SIZE*20, SIZE*18, SIZE*2, kColorBlack);
    _pd->graphics->fillRect(SIZE*17, 0, SIZE*1, SIZE*21, kColorBlack);
  } else {
    _pd->graphics->clear(kColorChekerboard);
    _pd->graphics->fillRect(0, 0, SIZE*19, SIZE*24, kColorBlack);
  }
}

void renderArrows(PlaydateAPI* _pd, int8_t _x, int8_t _yStart, int8_t _yAdd) {
  if ((getGameState() == kAwaitInput || getGameState() == kLevelSpecificWButtons) && getFrameCount() < ANIM_FPS/2) {
    drawCBitmap(_pd, &m_arrow_u, _x, _yStart);
    drawCBitmap(_pd, &m_arrow_r, _x, _yStart + _yAdd);
    drawCBitmap(_pd, &m_arrow_d, _x, _yStart + _yAdd + _yAdd);
  }
}

void renderHintNumber(PlaydateAPI* _pd, PDRect _r, int _value, bool _invert) {
  static char _hintText[3];
  snprintf(_hintText, 3, "%i", _value);
  renderBorderText(_pd, _r, m_fontMain, _hintText, 1, /*GTextAlignmentCenter,*/ _invert);
}

void renderClutter(PlaydateAPI* _pd) {
  Hints_t _hint = m_dungeon.m_roomGiveHint[ m_dungeon.m_level ][ m_dungeon.m_room];
  int _hintValue = m_dungeon.m_roomGiveHintValue[ m_dungeon.m_level ][ m_dungeon.m_room];
  for (int _c = 0; _c < m_clutter.m_nClutter; ++_c) {
    if (_c == 0 && _hint == kGreek) {
      drawCBitmap(_pd, getClutter(true), m_clutter.m_position_x[_c], m_clutter.m_position_y[_c]);
      int _px = (m_clutter.m_position_x[_c] * SIZE) + 4;
      int _py = (m_clutter.m_position_y[_c] * SIZE) + 2;
      drawCBitmapAbs(_pd, &m_greek[ _hintValue ], _px, _py);
    } else if (_c == 0 && _hint == kNumber) {
      drawCBitmap(_pd, getClutter(true), m_clutter.m_position_x[_c], m_clutter.m_position_y[_c]);
      PDRect r = {.x = m_clutter.m_position_x[_c] * SIZE, .y = (m_clutter.m_position_y[_c] * SIZE)+3, .width = 16, .height = 16}; // TODO tune .y
      renderHintNumber(_pd, r, _hintValue, true);
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
    _pd->graphics->drawLine(_x1, _y1, _x1, _y2, /*width=*/ 8, kColorWhite);
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

void renderFrame(PlaydateAPI* _pd, PDRect _b) {
  _pd->graphics->fillRect(_b.x, _b.y, _b.width, _b.height, kColorBlack);
  _pd->graphics->drawRect(_b.x+2, _b.y+2, _b.width-4, _b.height-4, kColorWhite);
  //graphics_context_set_fill_color(_pd, GColorDarkGray);
  //graphics_context_set_stroke_color(_pd, GColorWhite);
  //graphics_context_set_stroke_width(_pd, 2);
  //graphics_fill_rect(_pd, _b, 0, 0);
  //graphics_draw_rect(_pd, GRect(_b.origin.x+2, _b.origin.y+2, _b.size.w-4, _b.size.h-4));
}

void renderTextInFrame(PlaydateAPI* _pd, const char* _msg, PDRect _b) {
  //yyy
  //_pd->graphics->setDrawMode(kDrawModeFillWhite);


  const static int _text_y_offset = SIZE*2;
  _pd->graphics->fillRect(_b.x, _b.y, _b.width, _b.height, kColorWhite);
  _pd->graphics->drawRect(_b.x+2, _b.y+2, _b.width-4, _b.height-4, kColorBlack);
  
  _pd->graphics->setDrawMode(kDrawModeFillBlack);
  _pd->graphics->setFont(m_fontMain);
  int _len = _pd->graphics->getTextWidth(m_fontMain, _msg, strlen(_msg), kASCIIEncoding, /*tracking*/ 0);
  _b.x += (_b.width - _len)/2;
  _pd->graphics->drawText(_msg, strlen(_msg), kASCIIEncoding, _b.x, _b.y + _text_y_offset);
  _pd->graphics->setDrawMode(kDrawModeCopy);

  //graphics_context_set_fill_color(_pd, GColorWhite);
  //graphics_fill_rect(_pd, _b, 13, 0);
  //graphics_context_set_stroke_color(_pd, GColorBlack);
  //graphics_context_set_stroke_width(_pd, 3);
  //graphics_draw_rect(_pd, GRect(_b.origin.x+2, _b.origin.y+2, _b.size.w-4, _b.size.h-4));
  //graphics_context_set_text_color(_pd, GColorBlack);
  //graphics_draw_text(_pd, _msg, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(_b.origin.x, _b.origin.y + 4, _b.size.w, _b.size.h), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
}

void renderMessage(PlaydateAPI* _pd, const char* _msg) {
  PDRect _b = {.x = 0*SIZE, .y = 8*SIZE, .width = 18*SIZE, .height = 5*SIZE};
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
      drawCBitmap(_pd, &m_shieldSprite, _r, 0);
      //graphics_context_set_fill_color(_pd, getShieldColor(getShieldA(_hintValue)));
      //graphics_fill_circle(_pd, _p, 3);
      _pd->graphics->fillEllipse(_px - _w/2, _py - _w/2, /*sizeH*/_w, /*sizeV*/_w, /*aStart*/0, /*aEnd*/0, getShieldColor(getShieldA(_hintValue)));
      _pd->graphics->drawEllipse(_px - _w/2, _py - _w/2, _w, _w, /*lineWidth*/1, 0, 0, kColorBlack);      
      //graphics_context_set_fill_color(_pd, getShieldColor(getShieldC(_hintValue)));
      _px += SIZE*2;
      //graphics_fill_circle(_pd, _p, 3);
      _pd->graphics->fillEllipse(_px - _w/2, _py - _w/2, /*sizeH*/_w, /*sizeV*/_w, /*aStart*/0, /*aEnd*/0, getShieldColor(getShieldC(_hintValue)));
      _pd->graphics->drawEllipse(_px - _w/2, _py - _w/2, _w, _w, /*lineWidth*/1, 0, 0, kColorBlack); 
      //graphics_context_set_fill_color(_pd, getShieldColor(getShieldB(_hintValue)));
      _px -= SIZE;
      //graphics_fill_circle(_pd, _p, 3);
      _pd->graphics->fillEllipse(_px - _w/2, _py - _w/2, /*sizeH*/_w, /*sizeV*/_w, /*aStart*/0, /*aEnd*/0, getShieldColor(getShieldB(_hintValue)));
      _pd->graphics->drawEllipse(_px - _w/2, _py - _w/2, _w, _w, /*lineWidth*/1, 0, 0, kColorBlack); 
    } else if (_hint == kSpell) { // Check spell
      drawCBitmap(_pd, &m_tapestrySprite[0], _r, 0);
      for (int _i=1; _i<5; ++_i) drawCBitmap(_pd, &m_tapestrySprite[1], _r+_i, 0);
      drawCBitmap(_pd, &m_tapestrySprite[2], _r+5, 0);
      PDRect rect =  {.x = _r * SIZE, .y = 4, .width = 48, .height = 16};
      renderBorderText(_pd, rect, m_fontMain, m_spellNames[_hintValue], 1, false);
    } else if (_hint == kSymbol) { // Check symbol
      drawCBitmap(_pd, &m_symbol[_hintValue], _r, 18);
    }
}

void renderSawFloor(PlaydateAPI* _pd, int8_t _offset) {
  for (int _x = 0; _x < 20; _x += 2) {
    for (int _y = 6; _y < 12; _y += 2) {
      drawCBitmapAbs(_pd, getFloor(false), (_x*SIZE) - _offset, _y*SIZE);
    }
    drawCBitmapAbs(_pd, &m_halfUpperWall[0], (_x*SIZE) - _offset, 5*SIZE);
    drawCBitmapAbs(_pd, &m_halfLowerWall[0], (_x*SIZE) - _offset, 12*SIZE);
  }
}

void renderSawWalls(PlaydateAPI* _pd, int8_t _offset) {
  _pd->graphics->fillRect(0, 0, SIZE*19, SIZE*2, kColorBlack);
  _pd->graphics->fillRect(0, SIZE*15, SIZE*19, SIZE*5, kColorBlack);
  for (int _x = 0; _x < 20; _x += 2) {
    drawCBitmapAbs(_pd, &m_halfUpperWall[1], (_x*SIZE) - _offset, 4*SIZE);
    drawCBitmapAbs(_pd, &m_halfLowerWall[1], (_x*SIZE) - _offset, 13*SIZE);
    drawCBitmapAbs(_pd, &m_black, (_x*SIZE) - _offset, 2*SIZE);
    drawCBitmapAbs(_pd, &m_black, (_x*SIZE) - _offset, 14*SIZE);

  }
}

void renderFloor(PlaydateAPI* _pd, int _mode) {
  for (int _x = 3; _x < 15; _x += 2) {
    if (_mode == 1 && !(_x == 3 || _x == 13)) continue; // Pit
    for (int _y = 2; _y < 18; _y += 2) {
      drawCBitmap(_pd, getFloor(true), _x, _y);
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
  drawCBitmapAbs(_pd, &m_playerSprite[ m_player.m_playerFrame ], _pos_x, _pos_y);
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

#define FADE_LEVELS 8
void renderFade(PlaydateAPI* _pd, bool _in) {
  if (_in == false && m_dungeon.m_fallingDeath == true) m_player.m_position_y++;
  static int s_progress = 1;
  //GRect _b = layer_get_bounds(_thisLayer);
  //LCDBitmap* _fBuffer = graphics_capture_frame_buffer(_ctx);
  //int _flag = (_in == true ? s_progress : FADE_LEVELS - s_progress );
  // // Have to do a funny iterating for round screens
  //for (int _y = 0; _y < _b.size.h; ++_y) {
  //  LCDBitmapDataRowInfo _rowInfo = LCDBitmap_get_data_row_info(_fBuffer, _y);
  //  for (int _x = _rowInfo.min_x; _x < _rowInfo.max_x; ++_x) {
  //    uint8_t* _pixelAddr = _rowInfo.data + _x;
  //    if (rand() % _flag == 0) (*_pixelAddr) = GColorBlack.argb;
  //   }
  //}
  //graphics_release_frame_buffer(_pd, _fBuffer);
  if (++s_progress == FADE_LEVELS) {
    s_progress = 1;
    if (_in == true) setGameState(kLevelSpecific); // Done fade in, let level choose what next
    else setGameState(kNewRoom);
  }
}

LCDColor getShieldColor(int8_t _value) {
  switch (_value) {
    case 0: return kColorBlack;
    case 1: return kColorWhite;
    case 2: return kColorChekerboard;
    default: return kColorBlack;
  }
}
