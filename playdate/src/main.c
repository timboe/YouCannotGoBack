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
    #ifdef DEV
    playdate->system->logToConsole("EH: init");
    #endif
// Does not work in 1.9.3
//  } else if (event == kEventKeyPressed) {
//    playdate->system->logToConsole("EH: pressed %i", arg);
//    gameClickConfigHandler(arg);
  } else if (event == kEventTerminate) {
    deinit();
    #ifdef DEV
    playdate->system->logToConsole("EH: deinit");
    #endif
  } else if (event == kEventLock) {
    #ifdef DEV
    playdate->system->logToConsole("EH: lock");
    #endif
  } else if (event == kEventUnlock) {
    #ifdef DEV
    playdate->system->logToConsole("EH: unlock");
    #endif
  } else if (event == kEventPause) {
    #ifdef DEV
    playdate->system->logToConsole("EH: pause");
    #endif
  } else if (event == kEventResume) {
    #ifdef DEV
    playdate->system->logToConsole("EH: resume");
    #endif
  } else {
    #ifdef DEV
    playdate->system->logToConsole("EH: unknown event %i with arg %i", event, arg);
    #endif
  }
  
  return 0;
}