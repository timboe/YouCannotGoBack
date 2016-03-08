#pragma once
#include <pebble.h>
#include "../game.h"
#include "../sprite.h"

void addCluter(int _x, int _yUp, int _yDn);
void enterRoom(uint16_t* _state);
uint16_t randomiseChoices(int8_t* _choices, int _stage);
void stonesCommon(uint16_t* _state, int8_t* _fire, int8_t* _correct);
void shuffler(int8_t* _choices, int _offset, int _rand);
void renderStonesCommon(GContext* _ctx, int8_t* _coloursA, int8_t* _coloursB, int8_t* _coloursC, int8_t* _correct, int8_t _fire, int16_t _state);
