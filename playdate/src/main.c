#include <pebble.h>
#include "game.h"
#include "sprite.h"

static Window* s_gameWindow;

void tapHandle(AccelAxisType _axis, int32_t _direction) {
  static bool _backlight = false;
  _backlight = !_backlight;
  light_enable(_backlight);
  vibes_long_pulse();
}

static void init(void) {
  initSprite();

  s_gameWindow = window_create();
  window_set_click_config_provider(s_gameWindow, (ClickConfigProvider) gameClickConfigProvider);
  window_set_background_color(s_gameWindow, GColorBlack);
  window_set_window_handlers(s_gameWindow, (WindowHandlers) {
    .load = gameWindowLoad,
    .unload = gameWindowUnload,
  });

  accel_tap_service_subscribe(tapHandle);
}

static void deinit(void) {
  window_destroy(s_gameWindow);
  deinitSprite();
  accel_tap_service_unsubscribe();
}

int main(void) {
  init();
  window_stack_push(s_gameWindow, true);
  app_event_loop();
  deinit();
}
