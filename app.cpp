#include "app.h"
#include <cstdint>

constexpr uint32_t kNumLeds = 160;

App::App() : led_(kNumLeds) {}

void App::Init()
{
	led_.Init();
}

void App::ShowLed(uint32_t time_ms)
{
	led_.Show(time_ms);
}

void App::Delay(uint32_t ms)
{
	led_.Delay(ms);
}
