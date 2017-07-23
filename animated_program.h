#pragma once

#include "animation.h"
#include "program.h"
#include <memory>

class AnimatedProgram : public Program {
public:
    void SetColor(uint8_t red, uint8_t green, uint8_t blue)
    {
        red_ = red;
        green_ = green;
        blue_ = blue;
    }

    void SetBrightness(std::unique_ptr<AnimationSequence> sequence)
    {
        brightness_sequence_ = std::move(sequence);
    }

    void GetColor(uint32_t time_ms, uint8_t* red_out, uint8_t* green_out,
                  uint8_t* blue_out) override
    {
        *red_out = red_;
        *green_out = green_;
        *blue_out = blue_;
    }

    void GetBrightness(uint32_t time_ms, uint8_t* brightness_out) override
    {
        *brightness_out = brightness_sequence_->get(time_ms - time_base());
    }

private:
    uint8_t red_ = 0;
    uint8_t green_ = 0;
    uint8_t blue_ = 0;

    std::unique_ptr<AnimationSequence> brightness_sequence_;
};