#pragma once

#include "animation.h"
#include "program.h"
#include <memory>
#include <vector>

class AnimatedProgram : public Program {
public:
    void SetColor(std::vector<std::unique_ptr<AnimationSequence>> sequence)
    {
        color_sequence_ = std::move(sequence);
    }

    void GetColor(uint32_t time_ms, uint8_t* red_out, uint8_t* green_out,
                  uint8_t* blue_out) override;

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
        *brightness_out = brightness_sequence_->get(time_ms - time_base());
    }

    struct Segment {
        Animation animation[2];
        uint32_t start_time;
    };

    void SetSegments(std::vector<Segment> segments) { segments_ = std::move(segments); }

    uint32_t segment_count() override { return segments_.size(); }

    bool GetSegment(uint32_t time_ms, uint32_t segment_index, uint32_t* start_index,
                    uint32_t* end_index) override;

private:
    std::vector<std::unique_ptr<AnimationSequence>> color_sequence_;
    std::unique_ptr<AnimationSequence> brightness_sequence_;
    std::vector<Segment> segments_;
};
