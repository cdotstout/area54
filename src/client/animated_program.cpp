#pragma once

#include "animated_program.h"
#include "FastLED.h"

void AnimatedProgram::Segment::Draw(std::vector<CRGB>& led_array, uint32_t time_ms)
{
    if (time_ms < start_time)
        return;
    if (!keepalive && time_ms > end_time + 100) // fudge so it doesn't end too abruptly...
        return;

    uint32_t first = animation[0].get(time_ms - start_time);
    uint32_t last = animation[1].get(time_ms - start_time);
    if (first > last) {
        uint32_t tmp = first;
        first = last;
        last = tmp;
    }
    for (uint32_t index = first; index <= last; index++) {
        int hue;
        int saturation = 255;
        if (hue_animation) {
            hue = hue_animation->get(time_ms - start_time);
            if (saturation_animation)
                saturation = saturation_animation->get(time_ms - start_time);
        } else {
            hue = hue_gradient[0];
            if (last > first) {
                int delta = hue_gradient[1] - hue_gradient[0];
                delta *= (int)(index - first);
                delta /= (int)(last - first);
                hue += delta;
            }
        }
        if (index < led_array.size())
            led_array[index] = CRGB(CHSV(hue, saturation, 255));
    }
}
