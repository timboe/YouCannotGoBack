#include <pebble.h>
#include "game.h"
#include "sprite.h"

static Window* s_gameWindow;

static void init(void) {
  initSprite();

  s_gameWindow = window_create();
  window_set_click_config_provider(s_gameWindow, (ClickConfigProvider) gameClickConfigProvider);
  window_set_background_color(s_gameWindow, GColorBlack);
  window_set_window_handlers(s_gameWindow, (WindowHandlers) {
    .load = gameWindowLoad,
    .unload = gameWindowUnload,
  });

  //initGlobals();
  //initPersistence();
}

static void deinit(void) {
  window_destroy(s_gameWindow);
  deinitSprite();
  //destroyGlobals();
}

int main(void) {
  light_enable(1);
  init();
  window_stack_push(s_gameWindow, true);
  app_event_loop();
  deinit();
}
