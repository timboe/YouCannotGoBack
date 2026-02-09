#include "shapes.h"
#ifdef YCGBv2


static uint16_t s_state = 0;
static uint8_t s_win = 0;
static uint8_t s_outer[3][4] = {{0},{0},{0}};
static uint8_t s_inner[3][4] = {{0},{0},{0}};
static uint8_t s_rot = 0;
static uint8_t s_flip = 0;

void renderFloorPuzzleShape(GContext* _ctx, GPoint _p, uint8_t _i, uint8_t _rot, uint8_t _flip) {
  
  const uint8_t _w = 3*SIZE; 
  const uint8_t _h = 2*SIZE;

  const GRect _r1 = GRect(_p.x +  0, _p.y +  0, _w, _h);
  const GRect _r2 = GRect(_p.x + _w, _p.y +  0, _w, _h);
  const GRect _r3 = GRect(_p.x +  0, _p.y + _h, _w, _h);
  const GRect _r4 = GRect(_p.x + _w, _p.y + _h, _w, _h);
  const GRect _rAll = GRect(_p.x, _p.y, _w*2, _h*2);

  graphics_context_set_fill_color(_ctx, getShieldColor(s_outer[_i][(0 + _rot) % 4]));
  graphics_fill_rect(_ctx, _r1, SIZE, GCornerTopLeft);
  graphics_context_set_fill_color(_ctx, getShieldColor(s_outer[_i][(1 + _rot) % 4]));
  graphics_fill_rect(_ctx, _r2, SIZE, GCornerTopRight);
  graphics_context_set_fill_color(_ctx, getShieldColor(s_outer[_i][(2 + _rot) % 4]));
  graphics_fill_rect(_ctx, _r3, SIZE, GCornerBottomLeft);
  graphics_context_set_fill_color(_ctx, getShieldColor(s_outer[_i][(3 + _rot) % 4]));
  graphics_fill_rect(_ctx, _r4, SIZE, GCornerBottomRight);

  const uint16_t _rightAngle = (TRIG_MAX_ANGLE / 4);
  // const uint16_t _aRot = _rightAngle * _rot;
  graphics_context_set_fill_color(_ctx, getShieldColor(s_inner[_i][(0 + _rot) % 4]) );
  graphics_fill_radial(_ctx, _r1, GOvalScaleModeFitCircle, _h, -2*_rightAngle, _rightAngle);
  graphics_context_set_fill_color(_ctx, getShieldColor(s_inner[_i][(1 + _rot) % 4]) );
  graphics_fill_radial(_ctx, _r2, GOvalScaleModeFitCircle, _h, -1*_rightAngle, 2*_rightAngle);
  graphics_context_set_fill_color(_ctx, getShieldColor(s_inner[_i][(2 + _rot) % 4]) );
  graphics_fill_radial(_ctx, _r3, GOvalScaleModeFitCircle, _h, 1*_rightAngle, 4*_rightAngle);
  graphics_context_set_fill_color(_ctx, getShieldColor(s_inner[_i][(3 + _rot) % 4]) );
  graphics_fill_radial(_ctx, _r4, GOvalScaleModeFitCircle, _h, 0, 3*_rightAngle);

  graphics_context_set_stroke_color(_ctx, GColorBlack);
  graphics_context_set_stroke_width(_ctx, 3);
  graphics_draw_round_rect(_ctx, _rAll, SIZE);

}

void updateProcShapes(GContext* _ctx) {
  renderFloor(_ctx, 0);

  renderFloorPuzzleShape(_ctx, GPoint(3*SIZE, 8*SIZE), s_win, 0, 0);

  renderFloorPuzzleShape(_ctx, GPoint(9*SIZE, 2*SIZE),  0, 0, 0);
  renderFloorPuzzleShape(_ctx, GPoint(9*SIZE, 8*SIZE),  1, 0, 0);
  renderFloorPuzzleShape(_ctx, GPoint(9*SIZE, 14*SIZE), 2, 0, 0);
  
  renderClutter(_ctx);
  renderPlayer(_ctx);
  renderWalls(_ctx, true, true, true, true);
  renderWallClutter(_ctx);
  renderArrows(_ctx, 15, 4, 4);

}


bool tickShapes(bool _doInit) {
  if (_doInit == true) {
    s_state = 0;
    s_win = rand() % 3;
    m_player.m_position = GPoint(0, SIZE*9);
    addCluter(4, 0, 20); // Only left

    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 4; ++j) {
        s_outer[i][j] = rand() % MAX_SHIELD_COLOUR;
        s_inner[i][j] = rand() % MAX_SHIELD_COLOUR;
      }
    }
    s_rot = m_dungeon.m_difficulty > 0 ? rand() % 4 : 0;
    s_flip = m_dungeon.m_difficulty > 1 ? rand() % 3 : 0;
  }

  if (s_state == 0) { // start initial move
    m_player.m_target = GPoint(SIZE*5,SIZE*9);
    setGameState(kMovePlayer);
    ++s_state;
  } else if (s_state == 1) { // initial move is done
    setGameState(kAwaitInput);
    ++s_state;
  } else if (s_state == 2) {
    moveToExit(&s_state);
  } else if (s_state == 3) {
    setGameState(kFadeOut);
  }

  return false;
}


#endif