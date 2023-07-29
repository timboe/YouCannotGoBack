#pragma once
#include "game.h"

void initSound(PlaydateAPI* _pd);

void deinitSound(void);

void music(bool _onoff);
void sfx(bool _onoff);

void updateMusic(uint8_t _status);

void fireSound(void);
void chestSound(void);
void looseSound(void);
void winSound(void);
void sawSound(bool _start);
void sawVolume(float _v);
void darkSound(int _n);
void beepSound(void);
void clickSound(void);
void footSound(void);
void debufSound(void);
void bufSound(void);
void reminderSound(void);
void boomSound(void);
void fallSound(void);
void passwordSound(void);
void hitSound(void);
void stairsSound(void);
void fuseSound(bool _start);
void targetSound(void);
void clackSound(void);

//TODO
void correctSound(void);
