#include <mruby.h>
#include <mruby/value.h>

#include <stdio.h>

#include "esp_system.h"
#include "esp_sleep.h"
#include "esp_timer.h"
#include "esp_chip_info.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static mrb_value
mrb_esp32_system_delay(mrb_state *mrb, mrb_value self) {
  mrb_int delay;
  mrb_get_args(mrb, "i", &delay);

  vTaskDelay(delay / portTICK_PERIOD_MS);

  return self;
}

static mrb_value
mrb_esp32_system_available_memory(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(esp_get_free_heap_size());
}

static mrb_value
mrb_esp32_system_sdk_version(mrb_state *mrb, mrb_value self) {
  return mrb_str_new_cstr(mrb, esp_get_idf_version());
}

static mrb_value
mrb_esp32_system_restart(mrb_state *mrb, mrb_value self) {
  esp_restart();
  return self;
}

static mrb_value
mrb_esp32_system_deep_sleep_for(mrb_state *mrb, mrb_value self) {
  mrb_int sleep_time;
  mrb_get_args(mrb, "i", &sleep_time);

  esp_deep_sleep(sleep_time);

  return self;
}

static mrb_value
mrb_esp32_esp_timer_get_time(mrb_state *mrb, mrb_value self) {
  return mrb_float_value(mrb, esp_timer_get_time());
}

static mrb_value
mrb_esp32_get_chip_model(mrb_state *mrb, mrb_value self) {
  esp_chip_info_t info;
  esp_chip_info(&info);
  
  // The integer value in info.model is one of an enum, esp_chip_model_t, found here:
  // https://github.com/espressif/esp-idf/blob/master/components/esp_hw_support/include/esp_chip_info.h
  return mrb_fixnum_value(info.model);
}

void
mrb_mruby_esp32_system_gem_init(mrb_state* mrb) {
  // ESP32 Ruby module
  struct RClass *esp32_module = mrb_define_module(mrb, "ESP32");

  // ESP32::System
  struct RClass *esp32_system_module = mrb_define_module_under(mrb, esp32_module, "System");
  mrb_define_module_function(mrb, esp32_system_module, "delay", mrb_esp32_system_delay, MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, esp32_system_module, "available_memory", mrb_esp32_system_available_memory, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, esp32_system_module, "sdk_version", mrb_esp32_system_sdk_version, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, esp32_system_module, "restart", mrb_esp32_system_restart, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, esp32_system_module, "deep_sleep_for", mrb_esp32_system_deep_sleep_for, MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, esp32_system_module, "chip_model", mrb_esp32_get_chip_model, MRB_ARGS_NONE());

  // ESP32::Timer
  struct RClass *esp32_timer_module = mrb_define_module_under(mrb, esp32_module, "Timer");
  mrb_define_module_function(mrb, esp32_timer_module, "get_time", mrb_esp32_esp_timer_get_time, MRB_ARGS_NONE());
}

void
mrb_mruby_esp32_system_gem_final(mrb_state* mrb) {
}
