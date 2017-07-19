#pragma once

#include <cstdint>
#include "animation.h"

struct CRGB;

class Led {
public:
    Led(uint32_t num_leds);
    ~Led();

    void SetColor(uint8_t red, uint8_t green, uint8_t blue);
    void Init();
    void Show(uint32_t time_ms);
    void Delay(uint32_t ms);

private:
    uint32_t num_leds_;
    CRGB* leds_ = nullptr;

    uint8_t red_ = 0;
    uint8_t green_ = 0;
    uint8_t blue_ = 0;

    Animation fade_in_ { 0, 255, 3000 };
    Animation fade_out_ { 255, 0, 3000 };
};
