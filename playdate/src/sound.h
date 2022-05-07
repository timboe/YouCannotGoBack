#pragma once
#include "game.h"

void initSound(PlaydateAPI* _pd);

void deinitSound(void);

void fireSound(void);
void chestSound(void);
void looseSound(void);
void winSound(void);
void sawSound(bool _start);
void sawVolume(float _v);
void darkSound(int _n);
void beepSound(void);
void stepSound(void);
void debufSound(void);
void bufSound(void);
void reminderSound(void);
void boomSound(void);
void passwordSound(void);
void fuseSound(bool _start);

//TODO
void correctSound(void);
void stabSound(void);