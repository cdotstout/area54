#pragma once

#include "led.h"

class App {
public:
  App();

  void Init();

  void ShowLed();

  void Delay(uint32_t ms);

private:
  Led led_;
};
