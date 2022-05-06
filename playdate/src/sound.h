#pragma once
#include "game.h"

void initSound(PlaydateAPI* _pd);

void deinitSound();

void fireSound();
void chestSound();
void looseSound();
void winSound();
void sawSound(bool _start);
void sawVolume(float _v);
void darkSound(int _n);
void beepSound();
void stepSound();
void debufSound();
void bufSound();
void reminderSound();
void boomSound();
void passwordSound();
void fuseSound(bool _start);

//TODO
void correctSound();
void stabSound();