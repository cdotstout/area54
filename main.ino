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
  static int count;
  LOG("loop count: %u", count++);

  g_app->ShowLed();

  g_app->Delay(kMsPerFrame);
}
