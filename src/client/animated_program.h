#pragma once

#include "animation.h"
#include "led.h"
#include "program.h"
#include <memory>
#include <vector>

class AnimatedProgram : public Program {
public:
    void SetBrightness(std::unique_ptr<AnimationSequence> sequence)
    {
        brightness_sequence_ = std::move(sequence);
    }

    void GetBrightness(uint32_t time_ms, uint8_t* brightness_out) override
    {
        if (!brightness_sequence_) {
            *brightness_out = 255;
            return;
        }
        *brightness_out = brightness_sequence_->get(time_ms);
    }

    class Segment : public Led::Segment {
    public:
        void Draw(std::vector<CRGB>& led_array, uint32_t time_ms) override;

        Animation animation[2];
        uint32_t start_time;
        uint32_t end_time;
        bool keepalive;
        uint8_t hue_gradient[2];
        std::unique_ptr<Animation> hue_animation;
        std::unique_ptr<Animation> saturation_animation;
    };

    void SetSegments(std::vector<std::unique_ptr<Segment>> segments)
    {
        segments_ = std::move(segments);
    }

    uint32_t segment_count() override { return segments_.size(); }

    Led::Segment* GetSegment(uint32_t segment_index) override
    {
        return segments_[segment_index].get();
    }

    uint32_t get_duration() override { return brightness_sequence_->duration(); }

private:
    std::unique_ptr<AnimationSequence> brightness_sequence_;
    std::vector<std::unique_ptr<Segment>> segments_;
};
