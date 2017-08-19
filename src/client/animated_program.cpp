#pragma once

#include "animated_program.h"
#include "FastLED.h"

void AnimatedProgram::GetColor(uint32_t time_ms, uint8_t* red_out, uint8_t* green_out,
                               uint8_t* blue_out)
{
    uint8_t h = color_sequence_[0]->get(time_ms - time_base());
    uint8_t s = color_sequence_[1]->get(time_ms - time_base());
    uint8_t v = color_sequence_[2]->get(time_ms - time_base());

    CHSV hsv(h, s, v);
    CRGB rgb(hsv);

    *red_out = rgb.red;
    *green_out = rgb.green;
    *blue_out = rgb.blue;
}

bool AnimatedProgram::GetSegment(uint32_t time_ms, uint32_t segment_index, uint32_t* start_index,
                                 uint32_t* end_index)
{
    auto& segment = segments_[segment_index];
    if (time_ms - time_base() < segment.start_time)
        return false;
    *start_index = segment.animation[0].get(time_ms - time_base() - segment.start_time);
    *end_index = segment.animation[1].get(time_ms - time_base() - segment.start_time);
    if (*start_index > *end_index) {
        uint32_t tmp = *start_index;
        *start_index = *end_index;
        *end_index = tmp;
    }
    return true;
}