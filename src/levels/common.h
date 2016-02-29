#pragma once
#include <pebble.h>
#include "../game.h"
#include "../sprite.h"

void addCluter(int _max, int _x, int _yUp, int _yDn);
void enterRoom(uint16_t* _state);
uint16_t randomiseChoices(int8_t* _choices);
