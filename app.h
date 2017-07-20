#pragma once

#include "led.h"
#include "program.h"

class App {
public:
  App();

  void Init();

  void Update(uint32_t time_ms);

private:
  Led led_;
  std::unique_ptr<Program> program_;
};
