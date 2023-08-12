#include "sound.h"

static PlaydateAPI* pd = NULL;

bool m_sfxOn = true;
bool m_musicOn = true;

FilePlayer* m_music[2];
int8_t m_playing = 0;

SamplePlayer* m_samplePlayer;
SamplePlayer* m_loopPlayer;
SamplePlayer* m_footPlayer;
SamplePlayer* m_targetPlayer;
SamplePlayer* m_fallPlayer;
SamplePlayer* m_boomPlayer;
SamplePlayer* m_clackPlayer;
SamplePlayer* m_bellPlayer;

AudioSample* m_fireSample;
AudioSample* m_chestSample;
AudioSample* m_looseSample;
AudioSample* m_winSample;
AudioSample* m_sawSample;
AudioSample* m_dark[3];
AudioSample* m_foot[4];
AudioSample* m_beep;
AudioSample* m_fall;
AudioSample* m_click;
AudioSample* m_debuf;
AudioSample* m_buf;
AudioSample* m_reminder;
AudioSample* m_boom;
AudioSample* m_password;
AudioSample* m_fuse;
AudioSample* m_hit;
AudioSample* m_stairsAudio;
AudioSample* m_targetSound;
AudioSample* m_clackSound;
AudioSample* m_bellSound;

void music(bool _onoff) {
  m_musicOn = _onoff;
  if (m_musicOn) {
    updateMusic( m_dungeon.m_level == 2 ? 1 : 0 );
  } else {
    if (pd->sound->fileplayer->isPlaying(m_music[0])) pd->sound->fileplayer->stop(m_music[0]);
    if (pd->sound->fileplayer->isPlaying(m_music[1])) pd->sound->fileplayer->stop(m_music[1]);
  }
}

void sfx(bool _onoff) {
  m_sfxOn = _onoff;
  if (!m_sfxOn) {
    stopSounds();
  }
}

void stopSounds(void) {
  sawSound(false);
  fuseSound(false);
  pd->sound->sampleplayer->setVolume(m_loopPlayer, 1.0f, 1.0f);
}

void initSound(PlaydateAPI* _pd) {
  pd = _pd;

  for (int i=0; i<2; ++i) m_music[i] = pd->sound->fileplayer->newPlayer();
  int result = pd->sound->fileplayer->loadIntoPlayer(m_music[0], "sounds/8bitDungeonLevel");
  result &= pd->sound->fileplayer->loadIntoPlayer(m_music[1], "sounds/8bitDungeonBoss");
  if (m_sfxOn) pd->sound->fileplayer->play(m_music[0], 0);
  m_playing = 0;
  pd->system->logToConsole("Loaded audio %i", result);

  m_samplePlayer = pd->sound->sampleplayer->newPlayer(); // TODO: Report, docs say newSamplePlayer
  m_loopPlayer = pd->sound->sampleplayer->newPlayer();
  m_footPlayer = pd->sound->sampleplayer->newPlayer();
  m_targetPlayer = pd->sound->sampleplayer->newPlayer();
  m_boomPlayer = pd->sound->sampleplayer->newPlayer();
  m_fallPlayer = pd->sound->sampleplayer->newPlayer();
  m_clackPlayer = pd->sound->sampleplayer->newPlayer();
  m_bellPlayer = pd->sound->sampleplayer->newPlayer();

  m_fireSample = pd->sound->sample->load("sounds/270306__littlerobotsoundfactory__explosion-02");
  m_chestSample = pd->sound->sample->load("sounds/270338__littlerobotsoundfactory__open-01");
  m_looseSample = pd->sound->sample->load("sounds/270334__littlerobotsoundfactory__jingle-lose-01");
  m_winSample = pd->sound->sample->load("sounds/270319__littlerobotsoundfactory__jingle-win-01");
  m_dark[2] = pd->sound->sample->load("sounds/270320__littlerobotsoundfactory__jump-00");
  m_dark[1] = pd->sound->sample->load("sounds/270323__littlerobotsoundfactory__jump-03");
  m_dark[0] = pd->sound->sample->load("sounds/270317__littlerobotsoundfactory__jump-01");
  m_fall = pd->sound->sample->load("sounds/270323__littlerobotsoundfactory__jump-03_modified");
  m_beep = pd->sound->sample->load("sounds/270324__littlerobotsoundfactory__menu-navigate-00");
  m_click = pd->sound->sample->load("sounds/270322__littlerobotsoundfactory__menu-navigate-02");
  m_debuf = pd->sound->sample->load("sounds/270328__littlerobotsoundfactory__hero-death-00");
  m_buf = pd->sound->sample->load("sounds/270342__littlerobotsoundfactory__pickup-03");
  m_reminder = pd->sound->sample->load("sounds/270339__littlerobotsoundfactory__pickup-02");
  m_boom = pd->sound->sample->load("sounds/270311__littlerobotsoundfactory__explosion-03");
  m_password = pd->sound->sample->load("sounds/password");
  m_hit = pd->sound->sample->load("sounds/270325__littlerobotsoundfactory__hit-02");
  m_fuse = pd->sound->sample->load("sounds/184519__soundslikewillem__fuse");
  m_sawSample = pd->sound->sample->load("sounds/108171__aarongnp__buzzsaw-addiction_modified");
  m_stairsAudio = pd->sound->sample->load("sounds/442770__qubodup__walk-down-stairs");
  m_targetSound = pd->sound->sample->load("sounds/270302__littlerobotsoundfactory__collect_point_02");
  m_clackSound = pd->sound->sample->load("sounds/270315__littlerobotsoundfactory__menu_navigate_03.wav");
  m_bellSound = pd->sound->sample->load("sounds/378799__kgeshev__bell.wav");

  m_foot[0] = pd->sound->sample->load("sounds/197778__samulis__footstep-on-stone-1");
  m_foot[1] = pd->sound->sample->load("sounds/197779__samulis__footstep-on-stone-2");
  m_foot[2] = pd->sound->sample->load("sounds/197780__samulis__footstep-on-stone-3");
  m_foot[3] = pd->sound->sample->load("sounds/197781__samulis__footstep-on-stone-4");

  pd->sound->sampleplayer->setSample(m_targetPlayer, m_targetSound);
  pd->sound->sampleplayer->setSample(m_boomPlayer, m_boom);
  pd->sound->sampleplayer->setSample(m_fallPlayer, m_fall);
  pd->sound->sampleplayer->setSample(m_clackPlayer, m_clackSound);
  pd->sound->sampleplayer->setSample(m_bellPlayer, m_bellSound);
}

void updateMusic(uint8_t _status) {
  if (!m_musicOn) return;

  if (_status == 0) {
    if (pd->sound->fileplayer->isPlaying(m_music[1])) pd->sound->fileplayer->stop(m_music[1]);
    if (!pd->sound->fileplayer->isPlaying(m_music[0])) pd->sound->fileplayer->play(m_music[0], 0);
  } else if (_status == 1) {
    if (pd->sound->fileplayer->isPlaying(m_music[0]))  pd->sound->fileplayer->stop(m_music[0]);
    if (!pd->sound->fileplayer->isPlaying(m_music[1])) pd->sound->fileplayer->play(m_music[1], 0);
  }
}


void deinitSound() {
  return;
  pd->sound->fileplayer->freePlayer(m_music[0]);
  pd->sound->fileplayer->freePlayer(m_music[1]);
  pd->sound->sampleplayer->freePlayer(m_samplePlayer);
  pd->sound->sampleplayer->freePlayer(m_loopPlayer);
  m_music[0] = NULL;
  m_music[1] = NULL;
  m_samplePlayer = NULL;
  m_loopPlayer = NULL;
}

void fireSound() {
  if (!m_sfxOn) return;
  pd->sound->sampleplayer->setSample(m_samplePlayer, m_fireSample);
  pd->sound->sampleplayer->play(m_samplePlayer, 1, 1.0f);
}

void chestSound() {
  if (!m_sfxOn) return;
  pd->sound->sampleplayer->setSample(m_samplePlayer, m_chestSample);
  pd->sound->sampleplayer->play(m_samplePlayer, 1, 1.0f);
}

void looseSound() {
  if (!m_sfxOn) return;
  pd->sound->sampleplayer->setSample(m_samplePlayer, m_looseSample);
  pd->sound->sampleplayer->play(m_samplePlayer, 1, 1.0f);
}

void winSound() {
  if (!m_sfxOn) return;
  pd->sound->sampleplayer->setSample(m_samplePlayer, m_winSample);
  pd->sound->sampleplayer->play(m_samplePlayer, 1, 1.0f);
}

void footSound() {
  if (!m_sfxOn) return;
  pd->sound->sampleplayer->setSample(m_footPlayer, m_foot[rand() % 4]);
  pd->sound->sampleplayer->play(m_footPlayer, 1, 1.0f);
}

void stairsSound() {
  if (!m_sfxOn) return;
  pd->sound->sampleplayer->setSample(m_samplePlayer, m_stairsAudio);
  pd->sound->sampleplayer->play(m_samplePlayer, 1, 1.0f);
}

void hitSound() {
  if (!m_sfxOn) return;
  pd->sound->sampleplayer->setSample(m_samplePlayer, m_hit);
  pd->sound->sampleplayer->play(m_samplePlayer, 1, 1.0f);
}

void sawSound(bool _start) {
  if (_start && m_sfxOn) {
    pd->sound->sampleplayer->setSample(m_loopPlayer, m_sawSample);
    pd->sound->sampleplayer->play(m_loopPlayer, 0, 1.0f);
  } else {
    pd->sound->sampleplayer->stop(m_loopPlayer);
  }
}

void sawVolume(float _v) {
  if (_v > 1.0f) _v = 1.0f;
  if (_v < 0.0f) _v = 0.0f;
  pd->sound->sampleplayer->setVolume(m_loopPlayer, _v, _v);
}

void darkSound(int _n) {
  if (!m_sfxOn) return;
  if (_n > 2) return;
  pd->sound->sampleplayer->setSample(m_samplePlayer, m_dark[_n]);
  pd->sound->sampleplayer->play(m_samplePlayer, 1, 1.0f);
}

void beepSound() {
  if (!m_sfxOn) return;
  pd->sound->sampleplayer->setSample(m_samplePlayer, m_beep);
  pd->sound->sampleplayer->play(m_samplePlayer, 1, 1.0f);
}

void clickSound() {
  if (!m_sfxOn) return;
  pd->sound->sampleplayer->setSample(m_samplePlayer, m_click);
  pd->sound->sampleplayer->play(m_samplePlayer, 1, 1.0f);
}

void debufSound() {
  if (!m_sfxOn) return;
  pd->sound->sampleplayer->setSample(m_samplePlayer, m_debuf);
  pd->sound->sampleplayer->play(m_samplePlayer, 1, 1.0f);
}

void bufSound() {
  if (!m_sfxOn) return;
  pd->sound->sampleplayer->setSample(m_samplePlayer, m_buf);
  pd->sound->sampleplayer->play(m_samplePlayer, 1, 1.0f);
}

void reminderSound() {
  if (!m_sfxOn) return;
  pd->sound->sampleplayer->setSample(m_samplePlayer, m_reminder);
  pd->sound->sampleplayer->play(m_samplePlayer, 1, 1.0f);
}

void boomSound() {
  if (!m_sfxOn) return;
  pd->sound->sampleplayer->play(m_boomPlayer, 1, 1.0f);
}

void fallSound() {
  if (!m_sfxOn) return;
  pd->sound->sampleplayer->play(m_fallPlayer, 1, 1.0f);
}

void passwordSound() {
  if (!m_sfxOn) return;
  pd->sound->sampleplayer->setSample(m_samplePlayer, m_password);
  pd->sound->sampleplayer->play(m_samplePlayer, 1, 1.0f);
}

void fuseSound(bool _start) {
  if (_start && m_sfxOn) {
    pd->sound->sampleplayer->setSample(m_loopPlayer, m_fuse);
    pd->sound->sampleplayer->play(m_loopPlayer, 0, 1.0f);
    pd->sound->sampleplayer->setVolume(m_loopPlayer, 1.0f, 1.0f);
  } else {
    pd->sound->sampleplayer->stop(m_loopPlayer);
  }
}

void targetSound() {
  if (!m_sfxOn) return;
  pd->sound->sampleplayer->stop(m_targetPlayer);
  pd->sound->sampleplayer->play(m_targetPlayer, 1, 1.0f);
}

void clackSound() {
  if (!m_sfxOn) return;
  pd->sound->sampleplayer->stop(m_clackPlayer);
  pd->sound->sampleplayer->play(m_clackPlayer, 1, 1.0f);
}

void bellSound() {
  if (!m_sfxOn) return;
  pd->sound->sampleplayer->play(m_bellPlayer, 1, 1.0f);
}

