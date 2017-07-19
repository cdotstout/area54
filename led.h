#pragma once

#include <cstdint>
#include <vector>

struct CRGB;

class Led {
public:
    Led(uint32_t num_leds);
    ~Led();

    void Init();
    void Show();
    void Delay(uint32_t ms);

private:
    uint32_t num_leds_;
    CRGB* leds_ = nullptr;
};
