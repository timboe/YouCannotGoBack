#pragma once
#include "../game.h"
#include "../sprite.h"

void moveToExit(uint16_t* _state);
void addCluter(int _x, int _yUp, int _yDn);
void enterRoom(uint16_t* _state);
uint16_t randomiseChoices(PlaydateAPI* _pd, int8_t* _choices, int _stage);
void stonesCommon(uint16_t* _state, int8_t* _fire, int8_t* _correct);
void shuffler(PlaydateAPI* _pd, int8_t* _choices, int _offset, int _rand);
void renderStonesCommonFire(PlaydateAPI* _pd,
  int8_t* _correct,
  int8_t _fire,
  int16_t _state);