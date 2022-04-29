#include <stdio.h>
#include <stdlib.h>

#include "pd_api.h"
#include "game.h"
#include "sprite.h"

#ifdef _WINDLL
__declspec(dllexport)
#endif


/*
static Window* s_gameWindow;

void tapHandle(AccelAxisType _axis, int32_t _direction) {
  static bool _backlight = false;
  _backlight = !_backlight;
  light_enable(_backlight);
  vibes_long_pulse();
}


int main(void) {
  init();
  window_stack_push(s_gameWindow, true);
  app_event_loop();
  deinit();
}
*/

static void init(PlaydateAPI* _pd) {
  initSprite(_pd);
  gameWindowLoad();

  //s_gameWindow = window_create();
  //window_set_click_config_provider(s_gameWindow, (ClickConfigProvider) gameClickConfigProvider);
  //window_set_background_color(s_gameWindow, GColorBlack);
  //window_set_window_handlers(s_gameWindow, (WindowHandlers) {
  //  .load = gameWindowLoad,
  //  .unload = gameWindowUnload,
  //});

  //accel_tap_service_subscribe(tapHandle);
}

static void deinit(void) {
  //window_destroy(s_gameWindow);
  deinitSprite();
  //accel_tap_service_unsubscribe();
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