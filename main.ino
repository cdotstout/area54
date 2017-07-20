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
  static unsigned long start_time = 0;
  auto time_ms = millis();

  if (start_time == 0)
  	start_time = time_ms;

  g_app->Update(time_ms - start_time);

  delay(kMsPerFrame);
}
