#include "render.h"

void drawBitmap(GContext* _ctx, GBitmap* _bitmap, int _x, int _y) {
  GRect _r = gbitmap_get_bounds(_bitmap);
  _r.origin.x = _x * SIZE;
  _r.origin.y = _y * SIZE;
  graphics_draw_bitmap_in_rect(_ctx, _bitmap, _r);
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
      static char _hintText[3];
      snprintf(_hintText, 3, "%i", _hintValue);
      renderBorderText(_ctx, GRect(m_clutter.m_position[_c].x * SIZE, (m_clutter.m_position[_c].y * SIZE)-3, 16, 16),  fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), _hintText, 1, GTextAlignmentCenter);
    } else {
      drawBitmap(_ctx, getClutter(false), m_clutter.m_position[_c].x, m_clutter.m_position[_c].y);
    }
  }
  // Check shield
  int _r = 3 + (rand()%8);
  if (_hint == kShield) {
    GPoint _p = GPoint((_r + 1) * SIZE, SIZE);
    drawBitmap(_ctx, m_shieldSprite, _r, 0);
    graphics_context_set_fill_color(_ctx, getShieldA(_hintValue));
    graphics_fill_circle(_ctx, _p, 3);
    graphics_context_set_fill_color(_ctx, getShieldC(_hintValue));
    _p.x += SIZE*2;
    graphics_fill_circle(_ctx, _p, 3);
    graphics_context_set_fill_color(_ctx, getShieldB(_hintValue));
    _p.x -= SIZE;
    graphics_fill_circle(_ctx, _p, 3);
  } else if (_hint == kSpell) {
    drawBitmap(_ctx, m_tapestrySprite[0], _r, 0);
    for (int _i=1; _i<5; ++_i) drawBitmap(_ctx, m_tapestrySprite[1], _r+_i, 0);
    drawBitmap(_ctx, m_tapestrySprite[2], _r+5, 0);
    renderBorderText(_ctx, GRect(_r * SIZE, -2, 48, 16), fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), m_spellNames[_hintValue], 1, GTextAlignmentCenter);
  }
}

void drawBitmapAbs(GContext* _ctx, GBitmap* _bitmap, GPoint _p) {
  GRect _r = gbitmap_get_bounds(_bitmap);
  _r.origin = _p;
  graphics_draw_bitmap_in_rect(_ctx, _bitmap, _r);
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

void renderFloor(GContext* _ctx, int mode) {
  for (int _y = 2; _y < 18; _y += 2) {
    for (int _x = 3; _x < 15; _x += 2) {
      drawBitmap(_ctx, getFloor(), _x, _y);
    }
  }
  // Extra bits where the doors can go
  drawBitmap(_ctx, m_LDoorstep, 2, 8);
  drawBitmap(_ctx, m_RDoorstep, 15, 4);
  drawBitmap(_ctx, m_RDoorstep, 15, 8);
  drawBitmap(_ctx, m_RDoorstep, 15, 12);
}

void renderPlayer(GContext* _ctx) {
  drawBitmapAbs(_ctx, m_playerSprite[ m_player.m_playerFrame ], m_player.m_position);
}

void renderBorderText(GContext* _ctx, GRect _loc, GFont _f, const char* _buffer, uint8_t _offset, GTextAlignment _al) {

  graphics_context_set_text_color(_ctx, GColorBlack);

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
  _loc.origin.x += _offset; // O
  graphics_draw_text(_ctx, _buffer, _f, _loc, GTextOverflowModeWordWrap, _al, NULL);
}

//void setPixel(uint8_t* bitmapData, int bytesPerRow, int y, int x, uint8_t color) {
  //#ifdef PBL_COLOR
  //bitmap_data[y*bytes_per_row + x] = GColorBlack.argb;
  //#else
  //bitmap_data[y*bytes_per_row + x / 8] ^= (-color ^ bitmap_data[y*bytes_per_row + x / 8]) & (1 << (x % 8)); // in Aplite - set the bit
  //#endif
//}


#define FADE_LEVELS 16
void renderFade(Layer* _thisLayer, GContext* _ctx, bool _in) {
  static int s_progress = 1;
  GRect _b = layer_get_bounds(_thisLayer);
  GBitmap* _fBuffer = graphics_capture_frame_buffer(_ctx);
  uint8_t* _bitmapData =  gbitmap_get_data(_fBuffer);
  int _bytesPerRow = gbitmap_get_bytes_per_row(_fBuffer);
  //int _count = 0;
  int _flag = (_in == true ? s_progress : FADE_LEVELS - s_progress );
  for (int _y = 0; _y < _b.size.h; ++_y) {
    for (int _x = 0; _x < _b.size.w; ++_x) {
      if (rand() % _flag == 0) _bitmapData[_y * _bytesPerRow + _x] = GColorBlack.argb;
     }
   }
  graphics_release_frame_buffer(_ctx, _fBuffer);
  if (++s_progress == FADE_LEVELS) {
    s_progress = 1;
    if (_in == true) setGameState(kLevelSpecific); // Done fade in, let level choose what next
    else setGameState(kNewRoom);
  }
}



GColor getShieldA(int _value) {
  switch (_value) {
    case kRWb: return GColorRed;
    case kRbG: return GColorRed;
    case kbBG: return GColorBlack;
    case kbRW: return GColorBlack;
    case kBbR: return GColorBlue;
    case kBGR: default: return GColorBlue;
  }
}

GColor getShieldB(int _value) {
  switch (_value) {
    case kRWb: return GColorWhite;
    case kRbG: return GColorBlack;
    case kbBG: return GColorBlue;
    case kbRW: return GColorRed;
    case kBbR: return GColorBlack;
    case kBGR: default: return GColorGreen;
  }
}

GColor getShieldC(int _value) {
  switch (_value) {
    case kRWb: return GColorBlack;
    case kRbG: return GColorGreen;
    case kbBG: return GColorGreen;
    case kbRW: return GColorWhite;
    case kBbR: return GColorRed;
    case kBGR: default: return GColorRed;
  }
}
