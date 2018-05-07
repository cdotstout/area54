#include "app.h"
#include "log.h"

constexpr uint32_t kFramesPerSecond = 120;
constexpr uint32_t kMsPerFrame = 1000 / kFramesPerSecond;

std::unique_ptr<App> g_app;

static unsigned long g_frame_start;

void setup()
{
    Serial.begin(115200);

    LOG("setup begin");

    g_app = std::unique_ptr<App>(new App);
    if (!g_app->Init()) {
        g_app.reset();
        LOG("app init failed");
        return;
    }

    LOG("app init success");
}

void loop()
{
    auto time_ms = millis();

    if (g_app)
        g_app->Update(time_ms);

    if (time_ms - g_frame_start >= 1000) {
        LOG("tick");
        g_frame_start = time_ms;
    }

    //delay(kMsPerFrame);
}
