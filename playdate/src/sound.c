#include "sound.h"

static PlaydateAPI* pd = NULL;

FilePlayer* m_music;

SamplePlayer* m_samplePlayer;
SamplePlayer* m_loopPlayer;

AudioSample* m_fireSample;
AudioSample* m_chestSample;
AudioSample* m_looseSample;
AudioSample* m_winSample;
AudioSample* m_sawSample;
AudioSample* m_dark[3];
AudioSample* m_beep;
AudioSample* m_step;
AudioSample* m_debuf;
AudioSample* m_buf;
AudioSample* m_reminder;
AudioSample* m_boom;
AudioSample* m_password;
AudioSample* m_fuse;

void initSound(PlaydateAPI* _pd) {
  pd = _pd;

  m_music = pd->sound->fileplayer->newPlayer();
  int result = pd->sound->fileplayer->loadIntoPlayer(m_music, "sounds/Echoes_of_Time_v2");
  pd->sound->fileplayer->play(m_music, 0);
  pd->system->logToConsole("Loaded audio %i", result);

  m_samplePlayer = pd->sound->sampleplayer->newPlayer(); // TODO: Report, docs say newSamplePlayer
  m_loopPlayer = pd->sound->sampleplayer->newPlayer();

  m_fireSample = pd->sound->sample->load("sounds/270306__littlerobotsoundfactory__explosion-02");
  m_chestSample = pd->sound->sample->load("sounds/270338__littlerobotsoundfactory__open-01");
  m_looseSample = pd->sound->sample->load("sounds/270334__littlerobotsoundfactory__jingle-lose-01");
  m_winSample = pd->sound->sample->load("sounds/270319__littlerobotsoundfactory__jingle-win-01");
  m_dark[0] = pd->sound->sample->load("sounds/270320__littlerobotsoundfactory__jump-00");
  m_dark[1] = pd->sound->sample->load("sounds/270320__littlerobotsoundfactory__jump-03");
  m_dark[2] = pd->sound->sample->load("sounds/270320__littlerobotsoundfactory__jump-01");
  m_beep = pd->sound->sample->load("sounds/270324__littlerobotsoundfactory__menu-navigate-00");
  m_step = pd->sound->sample->load("sounds/270324__littlerobotsoundfactory__menu-navigate-02");
  m_debuf = pd->sound->sample->load("sounds/270328__littlerobotsoundfactory__hero-death-00");
  m_buf = pd->sound->sample->load("sounds/270342__littlerobotsoundfactory__pickup-03");
  m_reminder = pd->sound->sample->load("sounds/270339__littlerobotsoundfactory__pickup-02");
  m_boom = pd->sound->sample->load("sounds/270311__littlerobotsoundfactory__explosion-03");
  m_password = pd->sound->sample->load("sounds/");
  m_fuse = pd->sound->sample->load("sounds/");
  m_sawSample = pd->sound->sample->load("sounds/108171__aarongnp__buzzsaw-addiction_modified");

}

void deinitSound() {
  pd->sound->fileplayer->freePlayer(m_music);
  pd->sound->sampleplayer->freePlayer(m_samplePlayer);
  pd->sound->sampleplayer->freePlayer(m_loopPlayer);
  m_music = NULL;
  m_samplePlayer = NULL;
  m_loopPlayer = NULL;
}

void fireSound() {
  pd->sound->sampleplayer->setSample(m_samplePlayer, m_fireSample);
  pd->sound->sampleplayer->play(m_samplePlayer, 1, 1.0f);
}

void chestSound() {
  pd->sound->sampleplayer->setSample(m_samplePlayer, m_chestSample);
  pd->sound->sampleplayer->play(m_samplePlayer, 1, 1.0f);
}

void looseSound() {
  pd->sound->sampleplayer->setSample(m_samplePlayer, m_looseSample);
  pd->sound->sampleplayer->play(m_samplePlayer, 1, 1.0f);
}

void winSound() {
  pd->sound->sampleplayer->setSample(m_samplePlayer, m_winSample);
  pd->sound->sampleplayer->play(m_samplePlayer, 1, 1.0f);
}


void sawSound(bool _start) {
  if (_start) {
    pd->sound->sampleplayer->setSample(m_loopPlayer, m_sawSample);
    pd->sound->sampleplayer->play(m_loopPlayer, 0, 1.0f);
  } else {
    pd->sound->sampleplayer->stop(m_loopPlayer);
  }
}

void sawVolume(float _v) {
  if (_v > 1.0f) _v = 1.0f;
  if (_v < 0.0f) _v = 0.0f;
  pd->sound->sampleplayer->setVolume(m_loopPlayer, _v, _v)
}

void darkSound(int _n) {
  pd->sound->sampleplayer->setSample(m_samplePlayer, m_dark[_n]);
  pd->sound->sampleplayer->play(m_samplePlayer, 1, 1.0f);
}

void beepSound() {
  pd->sound->sampleplayer->setSample(m_samplePlayer, m_beep);
  pd->sound->sampleplayer->play(m_samplePlayer, 1, 1.0f);
}

void stepSound() {
  pd->sound->sampleplayer->setSample(m_samplePlayer, m_step);
  pd->sound->sampleplayer->play(m_samplePlayer, 1, 1.0f);
}

void debufSound() {
  pd->sound->sampleplayer->setSample(m_samplePlayer, m_debuf);
  pd->sound->sampleplayer->play(m_samplePlayer, 1, 1.0f);
}

void bufSound() {
  pd->sound->sampleplayer->setSample(m_samplePlayer, m_buf);
  pd->sound->sampleplayer->play(m_samplePlayer, 1, 1.0f);
}

void reminderSound() {
  pd->sound->sampleplayer->setSample(m_samplePlayer, m_reminder);
  pd->sound->sampleplayer->play(m_samplePlayer, 1, 1.0f);
}

void boomSound() {
  pd->sound->sampleplayer->setSample(m_samplePlayer, m_boom);
  pd->sound->sampleplayer->play(m_samplePlayer, 1, 1.0f);
}

void passwordSound() {
  pd->sound->sampleplayer->setSample(m_samplePlayer, m_password);
  pd->sound->sampleplayer->play(m_samplePlayer, 1, 1.0f);
}

void fuseSound(bool _start) {
  if (_start) {
    pd->sound->sampleplayer->setSample(m_loopPlayer, m_fuse);
    pd->sound->sampleplayer->play(m_loopPlayer, 0, 1.0f);
  } else {
    pd->sound->sampleplayer->stop(m_loopPlayer);
  }
}
