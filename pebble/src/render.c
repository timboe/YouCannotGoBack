#include "render.h"

static const GPathInfo TRIANGLE_PATH_INFO_T1 = {
  .num_points = 3,
  .points = (GPoint []) {{SIZE, 0}, {SIZE*2, SIZE*2}, {0, SIZE*2}}
};
static const GPathInfo TRIANGLE_PATH_INFO_T2 = {
  .num_points = 3,
  .points = (GPoint []) {{SIZE, 4}, {SIZE*2 - 2, SIZE*2 - 2}, {2, SIZE*2 - 2}}
};
static const GPathInfo TRIANGLE_PATH_INFO_T3 = {
  .num_points = 3,
  .points = (GPoint []) {{SIZE, 6}, {SIZE*2 - 4, SIZE*2 - 4}, {4, SIZE*2 - 4}}
};

static GPath* s_trigPath[3];

void initRender(void) {
  s_trigPath[0] = gpath_create(&TRIANGLE_PATH_INFO_T1);
  s_trigPath[1] = gpath_create(&TRIANGLE_PATH_INFO_T2);
  s_trigPath[2] = gpath_create(&TRIANGLE_PATH_INFO_T3);
}

void drawBitmap(GContext* _ctx, GBitmap* _bitmap, int _x, int _y) {
  GRect _r = gbitmap_get_bounds(_bitmap);
  _r.origin.x = _x * SIZE;
  _r.origin.y = _y * SIZE;
  _r.origin.x += GLOBAL_OFFSET_X;
  _r.origin.y += GLOBAL_OFFSET_Y;
  graphics_draw_bitmap_in_rect(_ctx, _bitmap, _r);
}

void renderArrows(GContext* _ctx, int8_t _x, int8_t _yStart, int8_t _yAdd) {
  renderArrowsDetailed(_ctx, _x, _yStart, _yAdd, 1, 1, 1, false);
}

void renderArrowsDetailed(GContext* _ctx, int8_t _x, int8_t _yStart, int8_t _yAdd, bool _0, bool _1, bool _2, bool _force) {
  if ((getGameState() == kAwaitInput || getGameState() == kLevelSpecificWButtons) && (_force || getFrameCount() < ANIM_FPS/2)) {
    if (_0) drawBitmap(_ctx, m_arrow, _x, _yStart);
    if (_1) drawBitmap(_ctx, m_arrow, _x, _yStart + _yAdd);
    if (_2) drawBitmap(_ctx, m_arrow, _x, _yStart + _yAdd + _yAdd);
  }
}

void renderHintNumber(GContext* _ctx, GRect _r, int _value, bool _invert) {
  static char _hintText[3];
  snprintf(_hintText, 3, "%i", _value);
  renderBorderText(_ctx, _r,  fonts_get_system_font(FONT_KEY_SMALL), _hintText, 2, GTextAlignmentCenter, _invert);
}

void renderClutter(GContext* _ctx) {
  Hints_t _hint = m_dungeon.m_roomGiveHint[ m_dungeon.m_level ][ m_dungeon.m_room];
  int _hintValue = m_dungeon.m_roomGiveHintValue[ m_dungeon.m_level ][ m_dungeon.m_room];
  for (int _c = 0; _c < m_clutter.m_nClutter; ++_c) {
    if (_c == 0 && _hint == kGreekLetter) {
      drawBitmap(_ctx, getClutter(true), m_clutter.m_position[_c].x, m_clutter.m_position[_c].y);
      #ifdef HIGH_RES
        GPoint _p = GPoint((m_clutter.m_position[_c].x * SIZE) + 5, (m_clutter.m_position[_c].y * SIZE) + 3);
      #else
        GPoint _p = GPoint((m_clutter.m_position[_c].x * SIZE) + 4, (m_clutter.m_position[_c].y * SIZE) + 2);
      #endif
      if (!getFlash(false)) drawBitmapAbs(_ctx, m_greek[ _hintValue ], _p);
    } else if (_c == 0 && _hint == kNumber) {
      drawBitmap(_ctx, getClutter(true), m_clutter.m_position[_c].x, m_clutter.m_position[_c].y);
      if (!getFlash(false)) renderHintNumber(_ctx, GRect(m_clutter.m_position[_c].x * SIZE, (m_clutter.m_position[_c].y * SIZE)-3, 16, 16), _hintValue, true);
    } else {
      drawBitmap(_ctx, getClutter(false), m_clutter.m_position[_c].x, m_clutter.m_position[_c].y);
    }
  }
}

void renderProgressBar(Layer* _thisLayer, GContext* _ctx) {
  GRect _b = layer_get_bounds(_thisLayer);
  int _x1 = 0;
  int _w = _b.size.w;
#ifdef PBL_ROUND
  _x1 += _b.size.w / 3;
  _w -= (2 * _b.size.w) / 3;
#endif
  int _x2 = ( _w * m_dungeon.m_roomsVisited ) / m_dungeon.m_totalRooms;
  int _h = _b.size.h - (SIZE/2);
#ifdef PBL_PLATFORM_GABBRO
  _h -= SIZE;
#endif
  GPoint _s = GPoint(_x1, _h);
  GPoint _e = GPoint(_x1 + _x2, _h);
  graphics_context_set_stroke_width(_ctx, 3);
#ifdef PBL_BW
  graphics_context_set_stroke_color(_ctx, GColorWhite);
#else
  graphics_context_set_stroke_color(_ctx, GColorRed);
#endif
  graphics_draw_line(_ctx, _s, _e);
}

void drawBitmapAbsInternal(GContext* _ctx, GBitmap* _bitmap, GPoint _p, bool _correction) {
  GRect _r = gbitmap_get_bounds(_bitmap);
  _r.origin = _p;
  if (_correction) {
    _r.origin.x += GLOBAL_OFFSET_X;
    _r.origin.y += GLOBAL_OFFSET_Y;
  }
  graphics_draw_bitmap_in_rect(_ctx, _bitmap, _r);
}

void drawBitmapAbs(GContext* _ctx, GBitmap* _bitmap, GPoint _p) {
  drawBitmapAbsInternal(_ctx, _bitmap, _p, true);
}

void drawBitmapAbsNoCorrection(GContext* _ctx, GBitmap* _bitmap, GPoint _p) {
  drawBitmapAbsInternal(_ctx, _bitmap, _p, false);
}

static void endRenderMsg(void* _data) {
  // Stop displaying message timeout
  if (getGameState() == kDisplayingMsg) setGameState(kLevelSpecific);
}

void renderStandingStoneFloor(GContext* _ctx) {

  renderLinePath(_ctx, 4, 10,   7, 6); // to upper
  renderLinePath(_ctx, 4, 10,   15, 10); // straight accross
  renderLinePath(_ctx, 4, 10,   7, 14); // to lower

  renderLinePath(_ctx, 7, 6,    15, 6); //upper accross
  renderLinePath(_ctx, 7, 14,   15, 14); // lower accross

  renderLinePath(_ctx, 11, 6,    11, 14); // middle down

  renderLinePath(_ctx, 7, 6,    15, 14); // cross \.
  renderLinePath(_ctx, 7, 14,   15, 6); // cross /

  renderLinePath(_ctx, 7, 10,   11, 6);  //diamond
  renderLinePath(_ctx, 7, 10,   11, 14);
  renderLinePath(_ctx, 11, 6,   15, 10);
  renderLinePath(_ctx, 11, 14,  15, 10);
}

void renderStandingStoneGrid(GContext* _ctx, int8_t* _coloursA, int8_t* _coloursB, int8_t* _coloursC, int8_t* _correct, int16_t _state, bool _isPattern) {

  StoneTypes_t _st = kCircle;

  // TODO - update triangle stone to be from spritesheet

  renderStandingStone(_ctx, GPoint(SIZE*4, SIZE*10), GColorLightGray, _st);

  if (_isPattern) {
#ifdef YCGBv2
    //x:7,11,15
    //y:6,10,14
    _st = _coloursA[ _correct[0] ];

    renderStandingStone(_ctx, GPoint(SIZE*7, SIZE*6), GColorLightGray, _coloursA[0]); // Left col
    renderStandingStone(_ctx, GPoint(SIZE*7, SIZE*10), GColorLightGray, _coloursA[1]); // Middle col
    renderStandingStone(_ctx, GPoint(SIZE*7, SIZE*14), GColorLightGray, _coloursA[2]); // Right col

    if (_state > 6) {
      renderStandingStone(_ctx, GPoint(SIZE*11, SIZE*6), GColorLightGray, _st);
      renderStandingStone(_ctx, GPoint(SIZE*11, SIZE*10), GColorLightGray, _st);
      renderStandingStone(_ctx, GPoint(SIZE*11, SIZE*14), GColorLightGray, _st);
    }
    if (_state > 8) {
      renderStandingStone(_ctx, GPoint(SIZE*15, SIZE*6), GColorLightGray, _st);
      renderStandingStone(_ctx, GPoint(SIZE*15, SIZE*10), GColorLightGray, _st);
      renderStandingStone(_ctx, GPoint(SIZE*15, SIZE*14), GColorLightGray, _st);
    }
#endif
  } else {

    for (int _s = 0; _s < 3; ++_s) {
      renderStandingStone(_ctx, GPoint(SIZE*7, SIZE*(6 + (4 * _s))), getShieldColor(_coloursA[_s]), _st); // Top row
      renderStandingStone(_ctx, GPoint(SIZE*11, SIZE*(6 + (4 * _s))), getShieldColor(_coloursB[_s]), _st); // Middle row
      renderStandingStone(_ctx, GPoint(SIZE*15, SIZE*(6 + (4 * _s))), getShieldColor(_coloursC[_s]), _st); // Bottom row
    }

  }

}

void renderLinePath(GContext* _ctx, int _x1, int _y1, int _x2, int _y2) {
  GPoint _p1 = GPoint(_x1*SIZE + GLOBAL_OFFSET_X, _y1*SIZE + GLOBAL_OFFSET_Y);
  GPoint _p2 = GPoint(_x2*SIZE + GLOBAL_OFFSET_X, _y2*SIZE + GLOBAL_OFFSET_Y);
  #ifdef HIGH_RES
    const uint8_t _w1 = 5;
    const uint8_t _w2 = 9;
  #else
    const uint8_t _w1 = 3;
    const uint8_t _w2 = 7;
  #endif
  graphics_context_set_stroke_width(_ctx, _w2);
  graphics_context_set_stroke_color(_ctx, GColorDarkGray);
  graphics_draw_line(_ctx, _p1, _p2);
  graphics_context_set_stroke_width(_ctx, _w1);
  graphics_context_set_stroke_color(_ctx, GColorLightGray);
  graphics_draw_line(_ctx, _p1, _p2);
}

void renderStandingStone(GContext* _ctx, GPoint _p, GColor _c, StoneTypes_t _st) {
  _p.x += GLOBAL_OFFSET_X;
  _p.y += GLOBAL_OFFSET_Y;

  if (_st == kCircle) {
    graphics_context_set_fill_color(_ctx, GColorLightGray);
    graphics_fill_circle(_ctx, _p, SIZE);
    graphics_context_set_fill_color(_ctx, GColorBlack);
    graphics_fill_circle(_ctx, _p, SIZE - 2);
    graphics_context_set_fill_color(_ctx, _c);
    graphics_fill_circle(_ctx, _p, SIZE - 4);
#ifdef YCGBv2
  } else if (_st == kSquare) {
    graphics_context_set_fill_color(_ctx, GColorLightGray);
    graphics_fill_rect(_ctx, GRect(_p.x - SIZE+0, _p.y - SIZE+0, (SIZE*2)-0, (SIZE*2)-0), 0, 0);
    graphics_context_set_fill_color(_ctx, GColorBlack);
    graphics_fill_rect(_ctx, GRect(_p.x - SIZE+1, _p.y - SIZE+1, (SIZE*2)-2, (SIZE*2)-2), 0, 0);
    graphics_context_set_fill_color(_ctx, _c);
    graphics_fill_rect(_ctx, GRect(_p.x - SIZE+2, _p.y - SIZE+2, (SIZE*2)-4, (SIZE*2)-4), 0, 0);
  } else if (_st == kTriangle) {
    _p.x -= SIZE;
    _p.y -= SIZE;
    gpath_move_to(s_trigPath[0], _p);
    gpath_move_to(s_trigPath[1], _p);
    gpath_move_to(s_trigPath[2], _p);
    graphics_context_set_fill_color(_ctx, GColorWhite);
    gpath_draw_filled(_ctx, s_trigPath[0]);
    graphics_context_set_fill_color(_ctx, GColorBlack);
    gpath_draw_filled(_ctx, s_trigPath[1]);
    graphics_context_set_fill_color(_ctx, _c);
    gpath_draw_filled(_ctx, s_trigPath[2]);
#endif
  }
}

void renderFrame(GContext* _ctx, GRect _b) {
  graphics_context_set_fill_color(_ctx, GColorBlack);
  graphics_context_set_stroke_color(_ctx, GColorDarkGray);
  graphics_context_set_stroke_width(_ctx, 2);
  _b.origin.x += GLOBAL_OFFSET_X;
  _b.origin.y += GLOBAL_OFFSET_Y;
  graphics_fill_rect(_ctx, _b, 0, 0);
  graphics_draw_rect(_ctx, GRect(_b.origin.x+2, _b.origin.y+2, _b.size.w-4, _b.size.h-4));
}


void renderTextInFrameInternal(GContext* _ctx, const char* _msg, GRect _b, bool _correction) {
  if (_correction) {
    _b.origin.x += GLOBAL_OFFSET_X;
    _b.origin.y += GLOBAL_OFFSET_Y;
  }
  uint8_t _offset = 4;
  #ifdef HIGH_RES
    _offset *= 2;
  #endif
  graphics_context_set_fill_color(_ctx, GColorWhite);
  graphics_fill_rect(_ctx, _b, 13, 0);
  graphics_context_set_stroke_color(_ctx, GColorBlack);
  graphics_context_set_stroke_width(_ctx, 3);
  graphics_draw_rect(_ctx, GRect(_b.origin.x+2, _b.origin.y+2, _b.size.w-4, _b.size.h-4));
  graphics_context_set_text_color(_ctx, GColorBlack);
  graphics_draw_text(_ctx, _msg, fonts_get_system_font(FONT_KEY_LARGE), GRect(_b.origin.x, _b.origin.y + _offset, _b.size.w, _b.size.h), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
}


void renderTextInFrame(GContext* _ctx, const char* _msg, GRect _b) {
  renderTextInFrameInternal(_ctx, _msg, _b, true);
}

void renderTextInFrameNoCorrection(GContext* _ctx, const char* _msg, GRect _b) {
  renderTextInFrameInternal(_ctx, _msg, _b, false);
}

void renderMessage(GContext* _ctx, const char* _msg) {

  GRect _b = GRect(0*SIZE, 8*SIZE, 18*SIZE, 5*SIZE);
  renderTextInFrame(_ctx, _msg, _b);
  app_timer_register(1500, endRenderMsg, NULL);
}

void renderBottomWall(GContext* _ctx) {
  graphics_context_set_fill_color(_ctx, GColorBlack);
  graphics_fill_rect(_ctx, GRect(0, PBL_DISPLAY_HEIGHT - SIZE, PBL_DISPLAY_WIDTH, SIZE), 0, 0);
  bool _torches = (m_dungeon.m_level % 2 == 0);
  for (int _x = 3; _x < 15; _x += 2) {  //Draw top and bottom wall
    if (_torches && (_x == 5 || _x == 11)) {
      drawBitmap(_ctx, m_torchWall[2], _x, 18);
    } else {
      drawBitmap(_ctx, getOuterWall(2), _x, 18);
    }
  }
}

void renderWalls(GContext* _ctx, bool _l, bool _rA, bool _rB, bool _rC) {
  drawBitmap(_ctx, m_outerCorner[0], 1, 0);
  drawBitmap(_ctx, m_outerCorner[1], 15, 0);
  drawBitmap(_ctx, m_outerCorner[2], 1, 18);
  drawBitmap(_ctx, m_outerCorner[3], 15, 18);
  srand(m_dungeon.m_seed);
  bool _torches = _torches = (rand() % 2 == 0);
  for (int _x = 3; _x < 15; _x += 2) {  //Draw top and bottom wall
    if (_torches && (_x == 5 || _x == 11)) {
      drawBitmap(_ctx, m_torchWall[0], _x, 0);
      drawBitmap(_ctx, m_torchWall[2], _x, 18);
    } else {
      drawBitmap(_ctx, getOuterWall(0), _x, 0); // top
      drawBitmap(_ctx, getOuterWall(2), _x, 18);
    }
  }
  _torches = (rand() % 2 == 0);
  for (int _y = 2; _y < 18; _y += 2) { // Draw left wall
    if (_y == 8 && _l == true) {
      drawBitmap(_ctx, m_LOpenDoor, 0,  _y);
      _y += 2;
    } else if (_torches && (_y == 6 || _y == 12)) {
      drawBitmap(_ctx, m_torchWall[1], 0,  _y);
    } else {
      drawBitmap(_ctx, getOuterWall(1), 1,  _y);
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
      drawBitmap(_ctx, m_RShutDoor, 15,  _y);
      _y += 2;
    } else if (_open == 2) {
      drawBitmap(_ctx, m_ROpenDoor, 15,  _y);
      _y += 2;
    } else if (_torches && (_y == 2 || _y == 16)) {
      drawBitmap(_ctx, m_torchWall[3], 15,  _y);
    } else {
      drawBitmap(_ctx, getOuterWall(3), 15, _y);
    }
  }
}

void renderWallClutter(GContext* _ctx) {
    Hints_t _hint = m_dungeon.m_roomGiveHint[ m_dungeon.m_level ][ m_dungeon.m_room];
    int _hintValue = m_dungeon.m_roomGiveHintValue[ m_dungeon.m_level ][ m_dungeon.m_room];
    int _r = 4 + (rand()%6);
    if (_hint == kShield && !getFlash(false)) {   // Check shield
      GPoint _p = GPoint((_r + 1) * SIZE + GLOBAL_OFFSET_X, SIZE + GLOBAL_OFFSET_Y);
      #ifdef HIGH_RES
        const uint8_t _radius = 5;
        const uint8_t _border = 2;
      #else
        const uint8_t _radius = 3;
        const uint8_t _border = 1;
      #endif
      drawBitmap(_ctx, m_shieldSprite, _r, 0);
      graphics_context_set_fill_color(_ctx, GColorBlack);
      graphics_fill_circle(_ctx, _p, _radius + _border);
      graphics_context_set_fill_color(_ctx, getShieldColor(getShieldA(_hintValue)));
      graphics_fill_circle(_ctx, _p, _radius);

      _p.x += SIZE*2;
      graphics_context_set_fill_color(_ctx, GColorBlack);
      graphics_fill_circle(_ctx, _p, _radius + _border);
      graphics_context_set_fill_color(_ctx, getShieldColor(getShieldC(_hintValue)));
      graphics_fill_circle(_ctx, _p, _radius);

      _p.x -= SIZE;
      graphics_context_set_fill_color(_ctx, GColorBlack);
      graphics_fill_circle(_ctx, _p, _radius + _border);
      graphics_context_set_fill_color(_ctx, getShieldColor(getShieldB(_hintValue)));
      graphics_fill_circle(_ctx, _p, _radius);
    } else if (_hint == kSpell) { // Check spell
      drawBitmap(_ctx, m_tapestrySprite[0], _r, 0);
      for (int _i=1; _i<5; ++_i) drawBitmap(_ctx, m_tapestrySprite[1], _r+_i, 0);
      drawBitmap(_ctx, m_tapestrySprite[2], _r+5, 0);
      int8_t _xoffset = 0;
      #ifdef HIGH_RES
      _xoffset = 1; // TODO reduce?
      #endif
      if (!getFlash(false)) renderBorderText(_ctx, GRect((_r+_xoffset) * SIZE, -2, 48, 16), fonts_get_system_font(FONT_KEY_SMALL), m_spellNames[_hintValue], 1, GTextAlignmentCenter, false);
    } else if (_hint == kSymbol && !getFlash(false)) { // Check symbol
      drawBitmap(_ctx, m_symbol[_hintValue], _r, 18);
    }
}

void renderSawFloor(GContext* _ctx, int8_t _offset) {
  for (int _x = 0; _x < 20; _x += 2) {
    for (int _y = 6; _y < 12; _y += 2) {
      drawBitmapAbs(_ctx, getFloor(false, m_dungeon.m_level), GPoint((_x*SIZE) - _offset, _y*SIZE));
    }
    drawBitmapAbs(_ctx, m_halfUpperWall[0], GPoint((_x*SIZE) - _offset, 5*SIZE));
    drawBitmapAbs(_ctx, m_halfLowerWall[0], GPoint((_x*SIZE) - _offset, 12*SIZE));
  }
}

void renderSawWalls(GContext* _ctx, int8_t _offset) {
  for (int _x = 0; _x < 20; _x += 2) {
    drawBitmapAbs(_ctx, m_halfUpperWall[1], GPoint((_x*SIZE) - _offset, 4*SIZE));
    drawBitmapAbs(_ctx, m_halfLowerWall[1], GPoint((_x*SIZE) - _offset, 13*SIZE));
    drawBitmapAbs(_ctx, m_black, GPoint((_x*SIZE) - _offset, 2*SIZE));
    drawBitmapAbs(_ctx, m_black, GPoint((_x*SIZE) - _offset, 14*SIZE));

  }
}

void renderFloor(GContext* _ctx, Rooms_t _room, int8_t _from, int8_t _to, int8_t _offsetX[6][8], int8_t _offsetY[6][8]) {

  if (_from == -1) _from = 2;
  if (_to == -1) _to = 18;
  const int8_t _level = m_dungeon.m_level;
  for (uint16_t _x = SIZE*3; _x < SIZE*15; _x += SIZE*2) {
    if (_room == kBridge && !(_x == SIZE*3 || _x == SIZE*13)) continue; // Pit
    int8_t _l = _level;
    #ifdef YCGBv2
      if (_room == kSpikes && (_x == SIZE*5 || _x == SIZE*9 || _x == SIZE*13)) _l = (_level + 1) % 3; // spikes
    #endif
    for (uint16_t _y = SIZE*_from; _y < SIZE*_to; _y += SIZE*2) {
      #ifdef YCGBv2
        if (_room == kUnstable) { // Unstable
          const GPoint _loc = GPoint(_x/(2*SIZE) - 1, _y/(2*SIZE) - 1);
          if (_offsetX[_loc.x][_loc.y] != 64) {
            drawBitmapAbs(_ctx, getFloor(true, _l), GPoint(_x + _offsetX[_loc.x][_loc.y], _y + _offsetY[_loc.x][_loc.y]));
          } else {
            getFloor(true, _l); // Keeps the floor rng in sycn
          }
        } else {
          drawBitmapAbs(_ctx, getFloor(true, _l), GPoint(_x,  _y));
        }
      #else
        drawBitmapAbs(_ctx, getFloor(true, _l), GPoint(_x,  _y));
      #endif
    }
  }
  if (_room == kBridge) { // Pit with one space on either side
    for (uint16_t _y = 4*SIZE; _y < 16*SIZE; _y += 2*SIZE) {
      drawBitmapAbs(_ctx, m_innerWall[0], GPoint(SIZE*5,  _y));
      drawBitmapAbs(_ctx, m_innerWall[1], GPoint(SIZE*11, _y));
    }
    drawBitmapAbs(_ctx, m_innerCorner[0], GPoint(SIZE*5,  SIZE*2));
    drawBitmapAbs(_ctx, m_innerCorner[1], GPoint(SIZE*11, SIZE*2));
    drawBitmapAbs(_ctx, m_innerCorner[2], GPoint(SIZE*11, SIZE*16));
    drawBitmapAbs(_ctx, m_innerCorner[3], GPoint(SIZE*5,  SIZE*16));
    drawBitmapAbs(_ctx, m_innerWall[2], GPoint(SIZE*7, SIZE*2));
    drawBitmapAbs(_ctx, m_innerWall[2], GPoint(SIZE*9, SIZE*2));
    drawBitmapAbs(_ctx, m_innerWall[3], GPoint(SIZE*7, SIZE*16));
    drawBitmapAbs(_ctx, m_innerWall[3], GPoint(SIZE*9, SIZE*16));
  }
  // Extra bits where the doors can go
  if (_to == 18) {
    drawBitmapAbs(_ctx, m_LDoorstep, GPoint(SIZE*2, SIZE*8)); // TODO this on its own call
    drawBitmapAbs(_ctx, m_RDoorstep, GPoint(SIZE*15, SIZE*4));
    drawBitmapAbs(_ctx, m_RDoorstep, GPoint(SIZE*15, SIZE*8));
    drawBitmapAbs(_ctx, m_RDoorstep, GPoint(SIZE*15, SIZE*12));
  }
}

void renderPit(GContext* _ctx) {
  for (int _y = 4; _y < 16; _y += 2) {
    drawBitmap(_ctx, m_innerWall[0], 3,  _y);
    drawBitmap(_ctx, m_innerWall[1], 13, _y);
  }
  for (int _x = 5; _x < 15; _x += 2) {
    drawBitmap(_ctx, m_innerWall[2], _x, 2);
    drawBitmap(_ctx, m_innerWall[3], _x, 16);
  }
  drawBitmap(_ctx, m_innerCorner[0], 3,  2);
  drawBitmap(_ctx, m_innerCorner[1], 13, 2);
  drawBitmap(_ctx, m_innerCorner[2], 13, 16);
  drawBitmap(_ctx, m_innerCorner[3], 3,  16);

  // Extra bits where the doors can go
  drawBitmap(_ctx, m_LDoorstep, 2, 8); // TODO this on its own call
  drawBitmap(_ctx, m_RDoorstep, 15, 4);
  drawBitmap(_ctx, m_RDoorstep, 15, 8);
  drawBitmap(_ctx, m_RDoorstep, 15, 12);
}

void renderFinalPit(GContext* _ctx) {
  for (int _w = 0; _w < 4; ++_w) { //0=l, 1=r
    drawBitmap(_ctx, m_innerWall[0], 7, 6 + (_w*2));
    drawBitmap(_ctx, m_innerWall[1], 9, 6 + (_w*2));
    if (_w >= 2) continue;
    drawBitmap(_ctx, m_innerWall[2], 9 + (_w*2), 2);
    drawBitmap(_ctx, m_innerWall[3], 9 + (_w*2), 6);
    drawBitmap(_ctx, m_innerWall[2], 9 + (_w*2), 12);
    drawBitmap(_ctx, m_innerWall[3], 9 + (_w*2), 16);
  }

  drawBitmap(_ctx, m_innerWall[0], 7, 4);
  drawBitmap(_ctx, m_innerWall[1], 13, 4);

  drawBitmap(_ctx, m_innerWall[0], 7, 14);
  drawBitmap(_ctx, m_innerWall[1], 13, 14);

  drawBitmap(_ctx, m_innerCorner[0], 7, 2);
  drawBitmap(_ctx, m_innerCorner[1], 13, 2);
  drawBitmap(_ctx, m_innerCorner[2], 13, 16);
  drawBitmap(_ctx, m_innerCorner[3], 7, 16);

  drawBitmap(_ctx, m_innerCorner[2], 13, 6);
  drawBitmap(_ctx, m_innerCorner[1], 13, 12);

  drawBitmap(_ctx, m_outerCorner[0], 9, 6);
  drawBitmap(_ctx, m_outerCorner[2], 9, 12);

  drawBitmap(_ctx, m_black, 9, 14);
  drawBitmap(_ctx, m_black, 11, 14);
  drawBitmap(_ctx, m_black, 9, 4);
  drawBitmap(_ctx, m_black, 11, 4);
}

void renderPlayer(GContext* _ctx) {
  GPoint _pos = m_player.m_position;
  if (m_player.m_playerFrame == 1 || m_player.m_playerFrame == 4) {
    --_pos.y;
    #ifdef HIGH_RES
    --_pos.y;
    #endif
  }
  drawBitmapAbs(_ctx, m_playerSprite[ m_player.m_playerFrame ], _pos);
}

void renderBorderTextInternal(GContext* _ctx, GRect _loc, GFont _f, const char* _buffer, uint8_t _offset, GTextAlignment _al, bool _invert, bool _correction) {

  graphics_context_set_text_color(_ctx, GColorBlack);
  if (_invert == true) graphics_context_set_text_color(_ctx, GColorWhite);

  if (_correction) {
    _loc.origin.x += GLOBAL_OFFSET_X;
    _loc.origin.y += GLOBAL_OFFSET_Y;
  }

  _loc.origin.y += _offset; // CU
  graphics_draw_text(_ctx, _buffer, _f, _loc, GTextOverflowModeWordWrap, _al, NULL);
  _loc.origin.x += _offset; // RU
  _loc.origin.y -= _offset; // CR
  graphics_draw_text(_ctx, _buffer, _f, _loc, GTextOverflowModeWordWrap, _al, NULL);
  _loc.origin.y -= _offset; // DR
  _loc.origin.x -= _offset; // DC
  graphics_draw_text(_ctx, _buffer, _f, _loc, GTextOverflowModeWordWrap, _al, NULL);
  _loc.origin.x -= _offset; // DR
  _loc.origin.y += _offset; // CR
  graphics_draw_text(_ctx, _buffer, _f, _loc, GTextOverflowModeWordWrap, _al, NULL);

  // main
  graphics_context_set_text_color(_ctx, GColorWhite);
  if (_invert == true) graphics_context_set_text_color(_ctx, GColorBlack);
  _loc.origin.x += _offset; // O
  graphics_draw_text(_ctx, _buffer, _f, _loc, GTextOverflowModeWordWrap, _al, NULL);
}

void renderBorderText(GContext* _ctx, GRect _loc, GFont _f, const char* _buffer, uint8_t _offset, GTextAlignment _al, bool _invert) {
  renderBorderTextInternal(_ctx, _loc, _f, _buffer, _offset, _al, _invert, true);
}

void renderBorderTextNoCorrection(GContext* _ctx, GRect _loc, GFont _f, const char* _buffer, uint8_t _offset, GTextAlignment _al, bool _invert) {
  renderBorderTextInternal(_ctx, _loc, _f, _buffer, _offset, _al, _invert, false);
}

#define FADE_LEVELS 8
void renderFade(Layer* _thisLayer, GContext* _ctx, bool _in) {
  if (_in == false && m_dungeon.m_fallingDeath == true) m_player.m_position.y += PLAYER_SPEED;
  static int s_progress = 1;
  GRect _b = layer_get_bounds(_thisLayer);
  GBitmap* _fBuffer = graphics_capture_frame_buffer(_ctx);
  int _flag = (_in == true ? s_progress : FADE_LEVELS - s_progress );
  // Have to do a funny iterating for round screens
  // TODO - fix this for B&W
  #ifdef PBL_COLOR
    for (int _y = 0; _y < _b.size.h; ++_y) {
      GBitmapDataRowInfo _rowInfo = gbitmap_get_data_row_info(_fBuffer, _y);
      for (int _x = _rowInfo.min_x; _x < _rowInfo.max_x; ++_x) {
        uint8_t* _pixelAddr = _rowInfo.data + _x;
        if (rand() % _flag == 0) (*_pixelAddr) = GColorBlack.argb;
       }
    }
  #endif
  graphics_release_frame_buffer(_ctx, _fBuffer);
  if (++s_progress == FADE_LEVELS) {
    s_progress = 1;
    if (_in == true) setGameState(kLevelSpecific); // Done fade in, let level choose what next
    else setGameState(kNewRoom);
  }
}

#ifdef YCGBv2

void renderBomb(GContext* _ctx, uint8_t _bombStage, int8_t _location) {

  uint8_t _bombSize = SIZE*2;
  GColor _c1 = GColorBlack;
  GColor _c2 = GColorWhite;
  if (getFlash(true) || _bombStage == 3) { 
    // _pd->graphics->setDrawMode(kDrawModeInverted);
    _c1 = GColorWhite;
    _c2 = GColorBlack;
  }

  GPoint _bLoc = GPoint(12 * SIZE, (6 + (4 * _location)) * SIZE);
  GPoint _bFuse = GPoint(_bLoc.x - 14, _bLoc.y - 28);
  #ifdef HIGH_RES
  _bFuse = GPoint(_bLoc.x - 20, _bLoc.y - 39);
  #endif
  graphics_context_set_fill_color(_ctx, _c1);
  graphics_fill_circle(_ctx, _bLoc, _bombSize);
  graphics_context_set_fill_color(_ctx, _c2);
  graphics_fill_circle(_ctx, _bLoc, _bombSize - 2);
  graphics_context_set_fill_color(_ctx, _c1);
  graphics_fill_circle(_ctx, _bLoc, _bombSize - 4);

  if (_bombStage == 3) {
    #define BOMB_LINES 32
    static int16_t _xoff[BOMB_LINES] = {0};
    static int16_t _yoff[BOMB_LINES] = {0};
    static uint8_t _w[BOMB_LINES] = {0};
    static GColor _c[BOMB_LINES];
    if (!_xoff[0]) { // Populate 
      for (uint8_t _i = 0; _i < BOMB_LINES; ++_i) {
        uint16_t _angle = rand() % TRIG_MAX_ANGLE ;
        uint8_t _len = (SIZE*4) + (rand() % (SIZE*8));
        _w[_i] = 2 + rand() % 5;
        _c[_i] = GColorWhite;
        #ifdef PBL_COLOR
          switch(rand() % 7) {
            case 0: _c[_i] = GColorIcterine; break;
            case 1: _c[_i] = GColorYellow; break;
            case 2: _c[_i] = GColorChromeYellow; break;
            case 3: _c[_i] = GColorRajah; break;
            case 4: _c[_i] = GColorOrange; break;
            case 5: _c[_i] = GColorRed; break; 
            default: _c[_i] = GColorFolly; break; 
          }
        #endif
        _xoff[_i] = ( sin_lookup(_angle) * _len / TRIG_MAX_RATIO);
        _yoff[_i] = (-cos_lookup(_angle) * _len / TRIG_MAX_RATIO);
      }
    }
    graphics_context_set_stroke_color(_ctx, GColorWhite);
    for (uint8_t _i = 0; _i < BOMB_LINES; ++_i) {
      graphics_context_set_stroke_width(_ctx, _w[_i]);
      graphics_context_set_stroke_color(_ctx, _c[_i]);
      graphics_draw_line(_ctx, _bLoc, GPoint(_bLoc.x + _xoff[_i], _bLoc.y + _yoff[_i]));
    }
  } else {
    drawBitmapAbs(_ctx, m_fuse[ _bombStage ], _bFuse);
  }
}

void renderBoxGridBox(GContext* _ctx, uint8_t _x1, uint8_t _y1, GColor _c, int8_t _offset) {
  drawBitmapAbs(_ctx, m_clutterSprite[2], GPoint(_x1*SIZE + _offset, _y1*SIZE + _offset));
  graphics_context_set_fill_color(_ctx, _c);
  uint8_t _coff = 4;
  #ifdef HIGH_RES
  _coff = 6;
  #endif
  graphics_fill_rect(_ctx, GRect(_x1*SIZE + _coff + _offset, _y1*SIZE + _coff/2 + _offset, SIZE, SIZE), 0, 0);
}

void renderBoxGrid(GContext* _ctx, int8_t* _coloursA, int8_t* _coloursB, int8_t* _coloursC, int8_t* _offset) {
  uint8_t _o = 0;
  for (int _s = 0; _s < 3; ++_s) {
    renderBoxGridBox(_ctx, 5, 5 + (4 * _s),  getShieldColor(_coloursA[_s]), _offset[_o++]); // Top row
    renderBoxGridBox(_ctx, 8, 5 + (4 * _s),  getShieldColor(_coloursB[_s]), _offset[_o++]); // Middle row
    renderBoxGridBox(_ctx, 11, 5 + (4 * _s), getShieldColor(_coloursC[_s]), _offset[_o++]); // Bottom row
  }

}

void renderFloorArrows(GContext* _ctx, Options_t maze0[3][3], uint8_t mwin0[3][3], Options_t maze1[5][7], uint8_t mwin1[5][7], uint16_t _tickB) {
  const bool _r = m_player.m_position.x > SIZE*8 && getFlash(true);
  const int8_t MAX_X = (m_dungeon.m_difficulty == 0 ? 3 : 5);
  const int8_t MAX_Y = (m_dungeon.m_difficulty == 0 ? 3 : 7);
  for (int _x = 0; _x < MAX_X; ++_x) {
    for (int _y = 0; _y < MAX_Y; ++_y) {
      if (m_dungeon.m_difficulty == 0) {
        if (_r && mwin0[_x][_y] == 2) continue;
        GBitmap* _bmp = m_floorArrow[ (maze0[_x][_y] + _tickB) % 8 ];
        drawBitmap(_ctx, _bmp, 4 + (4*_x), 5 + (4*_y));
      } else {
        if (_r && mwin1[_x][_y] == 2) continue;
        GBitmap* _bmp = m_floorArrow[ (maze1[_x][_y] + _tickB) % 8 ];
        drawBitmap(_ctx, _bmp, 4 + (2*_x), 3 + (2*_y));
      }
    }
  }
}

void renderShortcutFloor(GContext* _ctx) {
  const int8_t _level = m_dungeon.m_level;
  for (int _x = 0; _x < 20; _x += 2) {
    drawBitmapAbs(_ctx, getFloor(true, _level), GPoint(_x*SIZE, 8*SIZE));
    drawBitmapAbs(_ctx, m_halfUpperWall[0], GPoint(_x*SIZE, 7*SIZE));
    drawBitmapAbs(_ctx, m_halfLowerWall[0], GPoint(_x*SIZE, 10*SIZE));
  }
}

void renderShortcutWalls(GContext* _ctx) {
  graphics_context_set_fill_color(_ctx, GColorBlack);
  graphics_fill_rect(_ctx, GRect(0, 0, SIZE*19, SIZE*6), 0, 0);
  graphics_fill_rect(_ctx, GRect(0, SIZE*13, SIZE*19, SIZE*8), 0, 0);
  for (int _x = 0; _x < 20; _x += 2) {
    drawBitmapAbs(_ctx, m_halfUpperWall[1], GPoint(_x*SIZE, 6*SIZE));
    drawBitmapAbs(_ctx, m_halfLowerWall[1], GPoint(_x*SIZE, 11*SIZE));
    drawBitmapAbs(_ctx, m_black, GPoint(_x*SIZE, 4*SIZE));
    drawBitmapAbs(_ctx, m_black, GPoint(_x*SIZE, 12*SIZE));
  }
}

void renderGreekText(GContext* _ctx, uint8_t _msg[TOTAL_LETTERS], uint8_t _i, uint8_t _lettersThisLevel) {
  GPoint _p = GPoint(9*SIZE - SIZE/4 + GLOBAL_OFFSET_X, 5*SIZE + _i*4*SIZE + SIZE/2 + GLOBAL_OFFSET_Y);
  switch (_lettersThisLevel) {
    case 4: _p.x += SIZE/2; break;
    case 3: _p.x += SIZE; break;
    default: break;
  }
  for (uint8_t _i = 0; _i < _lettersThisLevel; ++_i) {
    drawBitmapAbs(_ctx, m_greek[_msg[_i]], _p);
    _p.x += SIZE;
  }
}

void renderGreekFrames(GContext* _ctx, uint8_t _a[TOTAL_LETTERS], uint8_t _b[TOTAL_LETTERS], uint8_t _c[TOTAL_LETTERS], uint8_t _lettersThisLevel) {
  #ifdef PBL_BW
  graphics_context_set_fill_color(_ctx, GColorWhite);
  graphics_context_set_stroke_color(_ctx, GColorBlack);
  #else
  graphics_context_set_fill_color(_ctx, GColorPastelYellow);
  graphics_context_set_stroke_color(_ctx, GColorBlack); // Change this?
  #endif
  //
  #ifdef HIGH_RES
  uint8_t _w = 2;
  graphics_context_set_stroke_width(_ctx, 3);
  #else
  uint8_t _w = 1;
  graphics_context_set_stroke_width(_ctx, 1);
  #endif
  //
  for (int _i = 0; _i < 3; ++_i) {
    GRect _rect = GRect(8*SIZE + GLOBAL_OFFSET_X, 5*SIZE + _i*4*SIZE + GLOBAL_OFFSET_Y, 6*SIZE, 2*SIZE);
    graphics_fill_rect(_ctx, _rect, 0, 0);
    graphics_draw_rect(_ctx, GRect(_rect.origin.x-_w, _rect.origin.y-_w, _rect.size.w+(2*_w), _rect.size.h+(2*_w)));

    #ifdef HIGH_RES
      drawBitmapAbs(_ctx, m_parchment[0], GPoint(_rect.origin.x - SIZE/2, _rect.origin.y - 7));
      drawBitmapAbs(_ctx, m_parchment[1], GPoint(_rect.origin.x + 5*SIZE + 3, _rect.origin.y - 1));
    #else
      drawBitmapAbs(_ctx, m_parchment[0], GPoint(_rect.origin.x - SIZE/2, _rect.origin.y - 5));
      drawBitmapAbs(_ctx, m_parchment[1], GPoint(_rect.origin.x + 5*SIZE + 3, _rect.origin.y - 1));
    #endif
  }
  renderGreekText(_ctx, _a, 0, _lettersThisLevel);
  renderGreekText(_ctx, _b, 1, _lettersThisLevel);
  renderGreekText(_ctx, _c, 2, _lettersThisLevel);

}

void renderPi(GContext* _ctx, GPoint _centre, GRect _r, uint32_t _start, uint32_t _stop, uint16_t _angle, GColor _fill, bool _evil, uint8_t _icon) {
  #ifdef PBL_BW
    _fill = _evil ? GColorBlack : GColorWhite;
  #endif
  graphics_context_set_stroke_color(_ctx, GColorBlack);
  graphics_context_set_fill_color(_ctx, _fill);

  #ifdef PBL_BW
    const bool _special = (_start == DEG_TO_TRIGANGLE(30));
  #else
    const bool _special = false;
  #endif

  _start += _angle;
  _stop += _angle;

  if (_start > TRIG_MAX_ANGLE) _start -= TRIG_MAX_ANGLE;
  if (_stop > TRIG_MAX_ANGLE) _stop -= TRIG_MAX_ANGLE;

  // Fill
  if (_stop > _start) {
    graphics_fill_radial(_ctx, _r, GOvalScaleModeFillCircle, SIZE*4, _start, _stop);
  } else {
    graphics_fill_radial(_ctx, _r, GOvalScaleModeFillCircle, SIZE*4, _start, TRIG_MAX_ANGLE);
    graphics_fill_radial(_ctx, _r, GOvalScaleModeFillCircle, SIZE*4, 0, _stop);

  }

  // Lines
  uint8_t _len = _r.size.w / 2;
  const GPoint _p1 = GPoint(_centre.x + (sin_lookup(_start) * _len / TRIG_MAX_RATIO),
                            _centre.y - (cos_lookup(_start) * _len / TRIG_MAX_RATIO));
  const GPoint _p2 = GPoint(_centre.x + (sin_lookup(_stop) * _len / TRIG_MAX_RATIO),
                            _centre.y - (cos_lookup(_stop) * _len / TRIG_MAX_RATIO)); 
  graphics_draw_line(_ctx, _centre, _p2);
  if (_special) graphics_context_set_stroke_color(_ctx, GColorWhite);
  graphics_draw_line(_ctx, _centre, _p1);

  // Icon
  int32_t _half = _stop - _start;
  if (_half < 0) _half += TRIG_MAX_ANGLE;
  int32_t _halfAngle = _start + (_half / 2);
  if (_halfAngle > TRIG_MAX_ANGLE) _halfAngle -= TRIG_MAX_ANGLE;
  _len = SIZE * 3;
  const GPoint _pHalf = GPoint((_centre.x + (sin_lookup(_halfAngle) * _len / TRIG_MAX_RATIO) - SIZE),
                               (_centre.y - (cos_lookup(_halfAngle) * _len / TRIG_MAX_RATIO) - SIZE));
  drawBitmapAbs(_ctx, m_wheelIcon[_icon], _pHalf);
}

void renderGamble(GContext* _ctx, uint8_t _wheel, uint16_t _angle, uint8_t _clack) {

  #ifdef HIGH_RES
    const uint8_t _w = 2;
  #else
    const uint8_t _w = 1;
  #endif

  GPoint _centre = GPoint(SIZE*10 + GLOBAL_OFFSET_X, SIZE*9 + GLOBAL_OFFSET_Y);
  GRect _r = GRect(SIZE*5 + _w*2 + GLOBAL_OFFSET_X, SIZE*4 + _w*2 + GLOBAL_OFFSET_Y, SIZE*10 - _w*4, SIZE*10 - _w*4);

  // Centre
  graphics_context_set_fill_color(_ctx, GColorBlack);
  graphics_fill_rect(_ctx, GRect(_centre.x - SIZE, _centre.y - SIZE, 2*SIZE, 2*SIZE), 0, 0);

  graphics_context_set_stroke_width(_ctx, _w*2);
  if (!_wheel) {
    renderPi(_ctx, _centre, _r, DEG_TO_TRIGANGLE(330), DEG_TO_TRIGANGLE(30), _angle, GColorRed, true, kIconSkull);
    renderPi(_ctx, _centre, _r, DEG_TO_TRIGANGLE(30), DEG_TO_TRIGANGLE(90), _angle, GColorRed, true, kIconBell);
    renderPi(_ctx, _centre, _r, DEG_TO_TRIGANGLE(90), DEG_TO_TRIGANGLE(210), _angle, GColorBlue, false, kIconSign);
    renderPi(_ctx, _centre, _r, DEG_TO_TRIGANGLE(210), DEG_TO_TRIGANGLE(330), _angle, GColorGreen, false, kIconClover);
  } else {
    renderPi(_ctx, _centre, _r, DEG_TO_TRIGANGLE(330), DEG_TO_TRIGANGLE(90), _angle, GColorRed, true, _wheel == 1 ? kIconBell : kIconWind);
    renderPi(_ctx, _centre, _r, DEG_TO_TRIGANGLE(90), DEG_TO_TRIGANGLE(210), _angle, GColorBlue, false, kIconSign);
    renderPi(_ctx, _centre, _r, DEG_TO_TRIGANGLE(210), DEG_TO_TRIGANGLE(330), _angle,  _wheel == 1 ? GColorGreen : GColorPurple, false, _wheel == 1 ? kIconClover : kIconSpyglass);
  }

  // Outer 
  graphics_context_set_stroke_width(_ctx, _w);
  #ifdef PBL_BW
    graphics_context_set_stroke_color(_ctx, GColorWhite);
  #else
    graphics_context_set_stroke_color(_ctx, GColorRoseVale);
  #endif
  graphics_draw_circle(_ctx, _centre, SIZE*5);

  graphics_context_set_stroke_width(_ctx, _w*2);
  graphics_context_set_stroke_color(_ctx, GColorBlack);
  graphics_draw_circle(_ctx, _centre, SIZE*5 - _w*2);

  drawBitmapAbs(_ctx, m_clack[_clack > 0 ? 1 : 0], GPoint(SIZE*9, SIZE*3));
  #ifdef HIGH_RES
    drawBitmapAbs(_ctx, m_wheelFrame, GPoint(SIZE*6 + SIZE/2, SIZE*14 - SIZE/2));
  #else
    drawBitmapAbs(_ctx, m_wheelFrame, GPoint(SIZE*6 + SIZE/2, SIZE*14 - SIZE/2));
  #endif
}

void renderPatternUnder(GContext* _ctx, GPoint _p, uint8_t _id1, uint8_t _id2) {

  _p.x += GLOBAL_OFFSET_X;
  _p.y += GLOBAL_OFFSET_Y;

  switch (_id2) {
    case 0: _p.y -= SIZE; break;
    case 1: _p.y += SIZE; _p.x -= SIZE; break;
    case 2: _p.y += SIZE; _p.x += SIZE; break;
  }

  if (_id1 == kTriangle) {
    graphics_context_set_fill_color(_ctx, GColorBlack);
    graphics_fill_circle(_ctx, _p, SIZE);
    graphics_context_set_fill_color(_ctx, GColorWhite);
    graphics_fill_circle(_ctx, _p, SIZE - 2);
    graphics_context_set_fill_color(_ctx, GColorBlack);
    graphics_fill_circle(_ctx, _p, SIZE - 4);
  } else if (_id1 == kCircle) {
    // switch (_id2) {
    //    case 1: _p.x += SIZE/2; _p.y -= SIZE/2; break;
    //    case 2: _p.x -= SIZE/2; _p.y -= SIZE/2; break;
    // }
    graphics_context_set_fill_color(_ctx, GColorBlack);
    graphics_fill_rect(_ctx, GRect(_p.x - SIZE+0, _p.y - SIZE+0, (SIZE*2)-0, (SIZE*2)-0), 0, 0);
    graphics_context_set_fill_color(_ctx, GColorWhite);
    graphics_fill_rect(_ctx, GRect(_p.x - SIZE+1, _p.y - SIZE+1, (SIZE*2)-2, (SIZE*2)-2), 0, 0);
    graphics_context_set_fill_color(_ctx, GColorBlack);
    graphics_fill_rect(_ctx, GRect(_p.x - SIZE+2, _p.y - SIZE+2, (SIZE*2)-4, (SIZE*2)-4), 0, 0);
  } else if (_id1 == kSquare) {
    //  switch (_id2) {
    //    case 0: _p.y -= SIZE; break;
    //    case 1: _p.y -= SIZE; _p.x -= SIZE; break;
    //    case 2: _p.y -= SIZE; _p.x += SIZE; break;
    // }
    // Why two blocks...?
    // if (_id2 == 0) { // Top
    //   _p.y -= SIZE;
    // } else if (_id2 == 1) { // Bottom left
    //   _p.
    // } else if (_id2 == 2) { // Bottom right
    gpath_move_to(s_trigPath[0], _p);
    gpath_move_to(s_trigPath[1], _p);
    gpath_move_to(s_trigPath[2], _p);
    graphics_context_set_fill_color(_ctx, GColorBlack);
    gpath_draw_filled(_ctx, s_trigPath[0]);
    graphics_context_set_fill_color(_ctx, GColorWhite);
    gpath_draw_filled(_ctx, s_trigPath[1]);
    graphics_context_set_fill_color(_ctx, GColorBlack);
    gpath_draw_filled(_ctx, s_trigPath[2]);
  }
}

void renderPatternLine(GContext* _ctx, GPoint _p, uint16_t _a, GColor _c1, GColor _c2, uint8_t _w1, uint8_t _w2) {

  _p.x += GLOBAL_OFFSET_X;
  _p.y += GLOBAL_OFFSET_Y;

  #define L_W ((SIZE*3)/2)
  GPoint _p1 = GPoint(_p.x + (sin_lookup(_a) * L_W / TRIG_MAX_RATIO),
    _p.y + (cos_lookup(_a) * L_W / TRIG_MAX_RATIO));
  GPoint _p2 = GPoint(_p.x + (sin_lookup(_a + TRIG_MAX_ANGLE/2) * L_W / TRIG_MAX_RATIO),
    _p.y + (cos_lookup(_a + TRIG_MAX_ANGLE/2) * L_W / TRIG_MAX_RATIO));
  // const int _x1 = _x*SIZE + (float)(L_W*sin(_a));
  // const int _y1 = _y*SIZE + (float)(L_W*cos(_a));
  // const int _x2 = _x*SIZE + (float)(L_W*sin(_a + (float)M_PI));
  // const int _y2 = _y*SIZE + (float)(L_W*cos(_a + (float)M_PI));

  graphics_context_set_stroke_color(_ctx, GColorBlack);
  graphics_context_set_stroke_width(_ctx, _w1);
  graphics_draw_line(_ctx, _p1, _p2);
  graphics_context_set_stroke_color(_ctx, GColorWhite);
  graphics_context_set_stroke_width(_ctx, _w2);
  graphics_draw_line(_ctx, _p1, _p2);

  // _pd->graphics->drawLine(_x1, _y1, _x2, _y2, /*width=*/ _w1, _c1);
  // _pd->graphics->drawLine(_x1, _y1, _x2, _y2, /*width=*/ _w2, _c2); /// xxx
}

void renderFloorPuzzleShape(GContext* _ctx, GPoint _p, uint8_t _inner[4], uint8_t _outer[4], uint8_t _rot, uint8_t _flip) {
  
  const uint8_t _w = 3*SIZE; 
  const uint8_t _h = 2*SIZE;

  #ifdef HIGH_RES
    const uint8_t _sw = 3;
  #else
    const uint8_t _sw = 2;
  #endif

  _p.x += GLOBAL_OFFSET_X;
  _p.y += GLOBAL_OFFSET_Y;

  GColor _colours[3];
  #ifdef PBL_BW
    _colours[0] = GColorBlack;
    _colours[1] = GColorWhite;
    _colours[2] = GColorDarkGray;
    GColor _border = GColorDarkGray;
  #else 
    if (m_dungeon.m_level == 0) {
      _colours[0] = GColorGreen;
      _colours[1] = GColorYellow;
      _colours[2] = GColorRed;
    } else if (m_dungeon.m_level == 1) {
      _colours[0] = GColorGreen;
      _colours[1] = GColorWhite;
      _colours[2] = GColorBlack;
    } else {
      _colours[0] = GColorGreen;
      _colours[1] = GColorWhite;
      _colours[2] = GColorBlack;
    }
    GColor _border = GColorBlack;
  #endif

  uint8_t _innerFlip[4] = {_inner[0], _inner[1], _inner[2], _inner[3]};
  uint8_t _outerFlip[4] = {_outer[0], _outer[1], _outer[2], _outer[3]};

  if (_flip == 1) { // horizontal
    _innerFlip[0] = _inner[1];
    _innerFlip[1] = _inner[0];
    _innerFlip[2] = _inner[3];
    _innerFlip[3] = _inner[2]; 
    _outerFlip[0] = _outer[1];
    _outerFlip[1] = _outer[0];
    _outerFlip[2] = _outer[3];
    _outerFlip[3] = _outer[2]; 
  } else if (_flip == 2) { //vertical
    _innerFlip[0] = _inner[3];
    _innerFlip[1] = _inner[2];
    _innerFlip[2] = _inner[1];
    _innerFlip[3] = _inner[0]; 
    _outerFlip[0] = _outer[3];
    _outerFlip[1] = _outer[2];
    _outerFlip[2] = _outer[1];
    _outerFlip[3] = _outer[0]; 
  }

  const GRect _r1 = GRect(_p.x +  0, _p.y +  0, _w, _h);
  const GRect _r2 = GRect(_p.x + _w, _p.y +  0, _w, _h);
  const GRect _r3 = GRect(_p.x + _w, _p.y + _h, _w, _h);
  const GRect _r4 = GRect(_p.x +  0, _p.y + _h, _w, _h);
  const GRect _rAll = GRect(_p.x, _p.y, _w*2, _h*2);

  graphics_context_set_fill_color(_ctx, _colours[_outerFlip[(0 + _rot) % 4]] );
  graphics_fill_rect(_ctx, _r1, SIZE, GCornerTopLeft);
  graphics_context_set_fill_color(_ctx, _colours[_outerFlip[(1 + _rot) % 4]] );
  graphics_fill_rect(_ctx, _r2, SIZE, GCornerTopRight);
  graphics_context_set_fill_color(_ctx, _colours[_outerFlip[(2 + _rot) % 4]] );
  graphics_fill_rect(_ctx, _r3, SIZE, GCornerBottomLeft);
  graphics_context_set_fill_color(_ctx, _colours[_outerFlip[(3 + _rot) % 4]] );
  graphics_fill_rect(_ctx, _r4, SIZE, GCornerBottomRight);

  const uint16_t _rightAngle = (TRIG_MAX_ANGLE / 4);
  graphics_context_set_fill_color(_ctx, _colours[_innerFlip[(0 + _rot) % 4]] );
  graphics_fill_radial(_ctx, _r1, GOvalScaleModeFitCircle, _h, -2*_rightAngle, _rightAngle);
  graphics_context_set_fill_color(_ctx, _colours[_innerFlip[(1 + _rot) % 4]] );
  graphics_fill_radial(_ctx, _r2, GOvalScaleModeFitCircle, _h, -1*_rightAngle, 2*_rightAngle);
  graphics_context_set_fill_color(_ctx, _colours[_innerFlip[(2 + _rot) % 4]] );
  graphics_fill_radial(_ctx, _r3, GOvalScaleModeFitCircle, _h, 0, 3*_rightAngle);
  graphics_context_set_fill_color(_ctx, _colours[_innerFlip[(3 + _rot) % 4]] );
  graphics_fill_radial(_ctx, _r4, GOvalScaleModeFitCircle, _h, 1*_rightAngle, 4*_rightAngle);

  graphics_context_set_stroke_color(_ctx, _border);
  graphics_context_set_stroke_width(_ctx, _sw);
  graphics_draw_round_rect(_ctx, _rAll, SIZE);

}

void renderSpikes(GContext* _ctx, int16_t _off[3], uint8_t _layer) {
  const int16_t _y = (SIZE*4*_layer) - SIZE*2 + SIZE*6;
  const int16_t _yHole = (SIZE*4*_layer) + SIZE*4;
  for (int _i = 0; _i < 3; ++_i) {
    const uint8_t _x = (SIZE*5) + (SIZE*4*_i);
    drawBitmapAbs(_ctx, m_spearHole[0], GPoint(_x, _yHole - SIZE));
    drawBitmapAbs(_ctx, m_spear, GPoint(_x, _y - (_off[_i] / SPIKE_MULTIPLIER) ));
  }
}

void renderSpikeHoleBottom(GContext* _ctx, uint8_t _layer) {
  const int16_t _yHole = (SIZE*4*_layer) + SIZE*4;
  for (int _i = 0; _i < 3; ++_i) {
    const uint8_t _x = (SIZE*5) + (SIZE*4*_i);
    drawBitmapAbs(_ctx, m_spearHole[1], GPoint(_x, _yHole));
  }
}

void rednerUnstableMarkers(GContext* _ctx) {
  drawBitmapAbs(_ctx, m_block, GPoint(SIZE*9, SIZE*6));
  drawBitmapAbs(_ctx, m_block, GPoint(SIZE*7, SIZE*8));
  drawBitmapAbs(_ctx, m_block, GPoint(SIZE*7, SIZE*10));
  drawBitmapAbs(_ctx, m_block, GPoint(SIZE*9, SIZE*12));
}

void renderWarning(GContext* _ctx) {
  drawBitmapAbs(_ctx, m_warning, GPoint(SIZE, 0));
}

#endif // YCGBv2

#ifdef PBL_BW
  GColor getShieldColor(int8_t _value) {
    switch (_value) {
      case 0: return GColorBlack;
      case 1: return GColorWhite;
      case 2: return GColorDarkGray;
      default: return GColorLightGray;
    }
  }
#else
  GColor getShieldColor(int8_t _value) {
    switch (_value) {
      case 0: return GColorRed;
      case 1: return GColorBlack;
      case 2: return GColorWhite;
      case 3: return GColorBlue;
      default: return GColorLightGray;
    }
  }
#endif