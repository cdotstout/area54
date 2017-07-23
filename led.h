#pragma once

#include "animation.h"
#include <cstdint>

struct CRGB;

class Led {
public:
    Led(uint32_t num_leds);
    ~Led();

    void Init();

    void Clear();
    void SetColor(uint8_t red, uint8_t green, uint8_t blue);
    void SetBrightness(uint8_t brightness);
    void Show();

private:
    uint32_t num_leds_;
    CRGB* leds_ = nullptr;

    uint8_t red_ = 0;
    uint8_t green_ = 0;
    uint8_t blue_ = 0;
};
