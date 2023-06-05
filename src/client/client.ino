#include "app.h"
#include "log.h"

std::unique_ptr<App> g_app;

const int FSR_PIN = 32;

void setup()
{
    Serial.begin(115200);
    while(!Serial)
        ;
    Serial.println("setup");

    LOG("setup begin");

    pinMode(FSR_PIN, INPUT);
}

void loop()
{
    static unsigned long g_frame_start = 0;

    auto time_ms = millis();

    if (!g_app) {
        g_app = std::unique_ptr<App>(new App);
        if (!g_app->Init()) {
            g_app.reset();
            LOG("app init failed");
            return;
        }
    }

    g_app->Update(time_ms);

    if (time_ms - g_frame_start >= 1000) {
        LOG("tick");
        g_frame_start = time_ms;
    }

    int v = analogRead(FSR_PIN);

    Serial.println("loop: v=" + String(v));
}