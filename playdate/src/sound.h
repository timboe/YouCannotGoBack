#pragma once
#include "game.h"

void initSound_ycgb(PlaydateAPI* _pd);

void deinitSound(void);

void music(bool _onoff);
void sfx_ycgb(bool _onoff);

void stopSounds(void);

void updateMusic_ycgb(uint8_t _status);

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
void bellSound(void);
void noSound(void);

//TODO
void correctSound(void);
