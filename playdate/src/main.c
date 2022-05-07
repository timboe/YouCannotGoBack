#include <stdio.h>
#include <stdlib.h>

#include "pd_api.h"
#include "game.h"
#include "sprite.h"
#include "sound.h"

#ifdef _WINDLL
__declspec(dllexport)
#endif

static void init(PlaydateAPI* _pd) {
  initSprite(_pd);
  initSound(_pd);
  gameWindowLoad();
}

static void deinit(void) {
  deinitSprite();
  deinitSound();
  gameWindowUnload();
}

int eventHandler(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg) {
  if (event == kEventInit) {
    setPDPtr(playdate);
    init(playdate);
    playdate->display->setRefreshRate(20);
    playdate->system->setUpdateCallback(gameLoop, NULL);
    playdate->system->logToConsole("EH: init");
// Does not work in 1.9.3
//  } else if (event == kEventKeyPressed) {
//    playdate->system->logToConsole("EH: pressed %i", arg);
    gameClickConfigHandler(arg);
  } else if (event == kEventTerminate) {
    deinit();
    playdate->system->logToConsole("EH: deinit");
  } else if (event == kEventLock) {
    playdate->system->logToConsole("EH: lock");
  } else if (event == kEventUnlock) {
    playdate->system->logToConsole("EH: unlock");
  } else if (event == kEventPause) {
    playdate->system->logToConsole("EH: pause");
  } else if (event == kEventResume) {
    playdate->system->logToConsole("EH: resume");
  } else {
    playdate->system->logToConsole("EH: unknown event %i with arg %i", event, arg);
  }
  
  return 0;
}