#include "sound.h"

static PlaydateAPI* pd = NULL;

FilePlayer* m_music;

SamplePlayer* m_samplePlayer;
AudioSample* m_fireSample;
AudioSample* m_chestSample;
AudioSample* m_looseSample;

void initSound(PlaydateAPI* _pd) {
  pd = _pd;

  m_music = pd->sound->fileplayer->newPlayer();
  int result = pd->sound->fileplayer->loadIntoPlayer(m_music, "sounds/Echoes_of_Time_v2");
  pd->sound->fileplayer->play(m_music, 1);
  pd->system->logToConsole("Loaded audio %i", result);

  m_samplePlayer = pd->sound->sampleplayer->newPlayer(); // TODO: Report, docs say newSamplePlayer

  m_fireSample = pd->sound->sample->load("sounds/270306__littlerobotsoundfactory__explosion-02");
  m_chestSample = pd->sound->sample->load("sounds/270338__littlerobotsoundfactory__open-01");
  m_looseSample = pd->sound->sample->load("270334__littlerobotsoundfactory__jingle-lose-01");
}

void deinitSound() {
  pd->sound->fileplayer->freePlayer(m_music);
  m_music = NULL;
  pd->sound->sampleplayer->freePlayer(m_samplePlayer);
  m_samplePlayer = NULL;
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