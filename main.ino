#include "app.h"
#include "log.h"

constexpr uint32_t kFramesPerSecond = 120;
constexpr uint32_t kMsPerFrame = 1000 / kFramesPerSecond;

App* g_app;

void setup() {
  Serial.begin(115200);
  Serial.println("setup begin");

  g_app = new App();
  g_app->Init();

  Serial.println("setup end");
}

void loop() {
  auto time_ms = millis();
  LOG("loop: time_ms %lu", time_ms);

  g_app->ShowLed(time_ms);

  g_app->Delay(kMsPerFrame);
}
