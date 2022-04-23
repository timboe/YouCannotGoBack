#include "render.h"

void drawBitmap(GContext* _ctx, GBitmap* _bitmap, int _x, int _y) {
  GRect _r = gbitmap_get_bounds(_bitmap);
  _r.origin.x = _x * SIZE;
  _r.origin.y = _y * SIZE;
#ifdef PBL_ROUND
  _r.origin.x += ROUND_OFFSET_X;
  _r.origin.y += ROUND_OFFSET_Y;
#endif
  graphics_draw_bitmap_in_rect(_ctx, _bitmap, _r);
}

void renderArrows(GContext* _ctx, int8_t _x, int8_t _yStart, int8_t _yAdd) {
  if ((getGameState() == kAwaitInput || getGameState() == kLevelSpecificWButtons) && getFrameCount() < ANIM_FPS/2) {
    drawBitmap(_ctx, m_arrow, _x, _yStart);
    drawBitmap(_ctx, m_arrow, _x, _yStart + _yAdd);
    drawBitmap(_ctx, m_arrow, _x, _yStart + _yAdd + _yAdd);
  }
}

void renderHintNumber(GContext* _ctx, GRect _r, int _value, bool _invert) {
  static char _hintText[3];
  snprintf(_hintText, 3, "%i", _value);
  renderBorderText(_ctx, _r,  fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), _hintText, 1, GTextAlignmentCenter, _invert);
}

void renderClutter(GContext* _ctx) {
  Hints_t _hint = m_dungeon.m_roomGiveHint[ m_dungeon.m_level ][ m_dungeon.m_room];
  int _hintValue = m_dungeon.m_roomGiveHintValue[ m_dungeon.m_level ][ m_dungeon.m_room];
  for (int _c = 0; _c < m_clutter.m_nClutter; ++_c) {
    if (_c == 0 && _hint == kGreek) {
      drawBitmap(_ctx, getClutter(true), m_clutter.m_position[_c].x, m_clutter.m_position[_c].y);
      GPoint _p = GPoint((m_clutter.m_position[_c].x * SIZE) + 4, (m_clutter.m_position[_c].y * SIZE) + 2);
      drawBitmapAbs(_ctx, m_greek[ _hintValue ], _p);
    } else if (_c == 0 && _hint == kNumber) {
      drawBitmap(_ctx, getClutter(true), m_clutter.m_position[_c].x, m_clutter.m_position[_c].y);
      renderHintNumber(_ctx, GRect(m_clutter.m_position[_c].x * SIZE, (m_clutter.m_position[_c].y * SIZE)-3, 16, 16), _hintValue, true);
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
  GPoint _s = GPoint(_x1, _h);
  GPoint _e = GPoint(_x1 + _x2, _h);
  graphics_context_set_stroke_width(_ctx, 3);
  graphics_context_set_stroke_color(_ctx, GColorRed);
  graphics_draw_line(_ctx, _s, _e);
}

void drawBitmapAbs(GContext* _ctx, GBitmap* _bitmap, GPoint _p) {
  GRect _r = gbitmap_get_bounds(_bitmap);
  _r.origin = _p;
#ifdef PBL_ROUND
  _r.origin.x += ROUND_OFFSET_X;
  _r.origin.y += ROUND_OFFSET_Y;
#endif
  graphics_draw_bitmap_in_rect(_ctx, _bitmap, _r);
}

static void endRenderMsg(void* _data) {
  // Stop displaying message timeout
  if (getGameState() == kDisplayingMsg) setGameState(kLevelSpecific);
}

void renderStandingStoneGrid(GContext* _ctx, int8_t* _coloursA, int8_t* _coloursB, int8_t* _coloursC) {

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

  renderStandingStone(_ctx, 4, 10, GColorLightGray);

  for (int _s = 0; _s < 3; ++_s) {
    renderStandingStone(_ctx, 7, 6 + (4 * _s), getShieldColor(_coloursA[_s])); // Top row
    renderStandingStone(_ctx, 11, 6 + (4 * _s), getShieldColor(_coloursB[_s])); // Middle row
    renderStandingStone(_ctx, 15, 6 + (4 * _s), getShieldColor(_coloursC[_s])); // Bottom row
  }

}

void renderLinePath(GContext* _ctx, int _x1, int _y1, int _x2, int _y2) {
  GPoint _p1 = GPoint(_x1*SIZE, _y1*SIZE);
  GPoint _p2 = GPoint(_x2*SIZE, _y2*SIZE);
#ifdef PBL_ROUND
  _p1.x += ROUND_OFFSET_X;
  _p1.y += ROUND_OFFSET_Y;
  _p2.x += ROUND_OFFSET_X;
  _p2.y += ROUND_OFFSET_Y;
#endif
  graphics_context_set_stroke_width(_ctx, 7);
  graphics_context_set_stroke_color(_ctx, GColorDarkGray);
  graphics_draw_line(_ctx, _p1, _p2);
  graphics_context_set_stroke_width(_ctx, 3);
  graphics_context_set_stroke_color(_ctx, GColorLightGray);
  graphics_draw_line(_ctx, _p1, _p2);
}

void renderStandingStone(GContext* _ctx, int _x1, int _y1, GColor _c) {
  GPoint _p1 = GPoint(_x1*SIZE, _y1*SIZE);
#ifdef PBL_ROUND
  _p1.x += ROUND_OFFSET_X;
  _p1.y += ROUND_OFFSET_Y;
#endif
  graphics_context_set_fill_color(_ctx, GColorLightGray);
  graphics_fill_circle(_ctx, _p1, SIZE);
  graphics_context_set_fill_color(_ctx, GColorBlack);
  graphics_fill_circle(_ctx, _p1, SIZE - 2);
  graphics_context_set_fill_color(_ctx, _c);
  graphics_fill_circle(_ctx, _p1, SIZE - 4);
}

void renderFrame(GContext* _ctx, GRect _b) {
  graphics_context_set_fill_color(_ctx, GColorDarkGray);
  graphics_context_set_stroke_color(_ctx, GColorWhite);
  graphics_context_set_stroke_width(_ctx, 2);
#ifdef PBL_ROUND
  _b.origin.x += ROUND_OFFSET_X;
  _b.origin.y += ROUND_OFFSET_Y;
#endif
  graphics_fill_rect(_ctx, _b, 0, 0);
  graphics_draw_rect(_ctx, GRect(_b.origin.x+2, _b.origin.y+2, _b.size.w-4, _b.size.h-4));
}

void renderTextInFrame(GContext* _ctx, const char* _msg, GRect _b) {
#ifdef PBL_ROUND
  _b.origin.x += ROUND_OFFSET_X;
  _b.origin.y += ROUND_OFFSET_Y;
#endif
  graphics_context_set_fill_color(_ctx, GColorWhite);
  graphics_fill_rect(_ctx, _b, 13, 0);
  graphics_context_set_stroke_color(_ctx, GColorBlack);
  graphics_context_set_stroke_width(_ctx, 3);
  graphics_draw_rect(_ctx, GRect(_b.origin.x+2, _b.origin.y+2, _b.size.w-4, _b.size.h-4));
  graphics_context_set_text_color(_ctx, GColorBlack);
  graphics_draw_text(_ctx, _msg, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(_b.origin.x, _b.origin.y + 4, _b.size.w, _b.size.h), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
}

void renderMessage(GContext* _ctx, const char* _msg) {
  GRect _b = GRect(0*SIZE, 8*SIZE, 18*SIZE, 5*SIZE);
  renderTextInFrame(_ctx, _msg, _b);
  app_timer_register(1500, endRenderMsg, NULL);
}

void renderWalls(GContext* _ctx, bool _l, bool _rA, bool _rB, bool _rC) {
  drawBitmap(_ctx, m_outerCorner[0], 1, 0);
  drawBitmap(_ctx, m_outerCorner[1], 15, 0);
  drawBitmap(_ctx, m_outerCorner[2], 1, 18);
  drawBitmap(_ctx, m_outerCorner[3], 15, 18);
  bool _torches = (rand() % 2 == 0);
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
    if (_hint == kShield) {   // Check shield
      GPoint _p = GPoint((_r + 1) * SIZE, SIZE);
  #ifdef PBL_ROUND
    _p.x += ROUND_OFFSET_X;
    _p.y += ROUND_OFFSET_Y;
  #endif
      drawBitmap(_ctx, m_shieldSprite, _r, 0);
      graphics_context_set_fill_color(_ctx, getShieldColor(getShieldA(_hintValue)));
      graphics_fill_circle(_ctx, _p, 3);
      graphics_context_set_fill_color(_ctx, getShieldColor(getShieldC(_hintValue)));
      _p.x += SIZE*2;
      graphics_fill_circle(_ctx, _p, 3);
      graphics_context_set_fill_color(_ctx, getShieldColor(getShieldB(_hintValue)));
      _p.x -= SIZE;
      graphics_fill_circle(_ctx, _p, 3);
    } else if (_hint == kSpell) { // Check spell
      drawBitmap(_ctx, m_tapestrySprite[0], _r, 0);
      for (int _i=1; _i<5; ++_i) drawBitmap(_ctx, m_tapestrySprite[1], _r+_i, 0);
      drawBitmap(_ctx, m_tapestrySprite[2], _r+5, 0);
      renderBorderText(_ctx, GRect(_r * SIZE, -2, 48, 16), fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), m_spellNames[_hintValue], 1, GTextAlignmentCenter, false);
    } else if (_hint == kSymbol) { // Check symbol
      drawBitmap(_ctx, m_symbol[_hintValue], _r, 18);
    }
}

void renderSawFloor(GContext* _ctx, int8_t _offset) {
  for (int _x = 0; _x < 20; _x += 2) {
    for (int _y = 6; _y < 12; _y += 2) {
      drawBitmapAbs(_ctx, getFloor(false), GPoint((_x*SIZE) - _offset, _y*SIZE));
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

void renderFloor(GContext* _ctx, int _mode) {
  for (int _x = 3; _x < 15; _x += 2) {
    if (_mode == 1 && !(_x == 3 || _x == 13)) continue; // Pit
    for (int _y = 2; _y < 18; _y += 2) {
      drawBitmap(_ctx, getFloor(true), _x, _y);
    }
  }
  if (_mode == 1) { // Pit with one space on either side
    for (int _y = 4; _y < 16; _y += 2) {
      drawBitmap(_ctx, m_innerWall[0], 5,  _y);
      drawBitmap(_ctx, m_innerWall[1], 11, _y);
    }
    drawBitmap(_ctx, m_innerCorner[0], 5,  2);
    drawBitmap(_ctx, m_innerCorner[1], 11, 2);
    drawBitmap(_ctx, m_innerCorner[2], 11, 16);
    drawBitmap(_ctx, m_innerCorner[3], 5,  16);
    drawBitmap(_ctx, m_innerWall[2], 7, 2);
    drawBitmap(_ctx, m_innerWall[2], 9, 2);
    drawBitmap(_ctx, m_innerWall[3], 7, 16);
    drawBitmap(_ctx, m_innerWall[3], 9, 16);
  }
  // Extra bits where the doors can go
  drawBitmap(_ctx, m_LDoorstep, 2, 8); // TODO this on its own call
  drawBitmap(_ctx, m_RDoorstep, 15, 4);
  drawBitmap(_ctx, m_RDoorstep, 15, 8);
  drawBitmap(_ctx, m_RDoorstep, 15, 12);
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
  if (m_player.m_playerFrame == 1 || m_player.m_playerFrame == 4) --_pos.y;
  drawBitmapAbs(_ctx, m_playerSprite[ m_player.m_playerFrame ], _pos);
}

void renderBorderText(GContext* _ctx, GRect _loc, GFont _f, const char* _buffer, uint8_t _offset, GTextAlignment _al, bool _invert) {

  graphics_context_set_text_color(_ctx, GColorBlack);
  if (_invert == true) graphics_context_set_text_color(_ctx, GColorWhite);
#ifdef PBL_ROUND
  _loc.origin.x += ROUND_OFFSET_X;
  _loc.origin.y += ROUND_OFFSET_Y;
#endif

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

#define FADE_LEVELS 8
void renderFade(Layer* _thisLayer, GContext* _ctx, bool _in) {
  if (_in == false && m_dungeon.m_fallingDeath == true) m_player.m_position.y++;
  static int s_progress = 1;
  GRect _b = layer_get_bounds(_thisLayer);
  GBitmap* _fBuffer = graphics_capture_frame_buffer(_ctx);
  int _flag = (_in == true ? s_progress : FADE_LEVELS - s_progress );
  // Have to do a funny iterating for round screens
  for (int _y = 0; _y < _b.size.h; ++_y) {
    GBitmapDataRowInfo _rowInfo = gbitmap_get_data_row_info(_fBuffer, _y);
    for (int _x = _rowInfo.min_x; _x < _rowInfo.max_x; ++_x) {
      uint8_t* _pixelAddr = _rowInfo.data + _x;
      if (rand() % _flag == 0) (*_pixelAddr) = GColorBlack.argb;
     }
  }
  graphics_release_frame_buffer(_ctx, _fBuffer);
  if (++s_progress == FADE_LEVELS) {
    s_progress = 1;
    if (_in == true) setGameState(kLevelSpecific); // Done fade in, let level choose what next
    else setGameState(kNewRoom);
  }
}

GColor getShieldColor(int8_t _value) {
  switch (_value) {
    case 0: return GColorRed;
    case 1: return GColorBlack;
    case 2: return GColorWhite;
    case 3: return GColorBlue;
    default: return GColorLightGray;
  }
}
