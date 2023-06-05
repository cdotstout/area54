#pragma once

#include "log.h"
#include <cstdint>
#include <vector>

class Animation {
public:
    Animation(uint32_t start = 0, uint32_t end = 255, uint32_t duration = 1000)
        : start_(start), end_(end), duration_(duration)
    {
    }

    uint32_t get(uint32_t time)
    {
        if (time >= duration_ || duration_ == 0)
            return end_;
        int32_t delta = end_ - start_;
        return start_ + delta * (int32_t)time / (int32_t)duration_;
    }

    uint32_t duration() { return duration_; }

private:
    uint32_t start_;
    uint32_t end_;
    uint32_t duration_;
};

class AnimationSequence {
public:
    AnimationSequence(bool loop = false) : loop_(loop) {}

    void Add(Animation animation)
    {
        sequence_.push_back(animation);
        duration_ += animation.duration();
    }

    // time must be nondecreasing
    uint32_t get(uint32_t time)
    {
        if (time > duration_ && !loop_)
            return 0;

        int relative_time = time % duration_;

        std::vector<Animation>::iterator iter = sequence_.begin();
        while (iter != sequence_.end()) {
            if (relative_time < iter->duration()) {
                return iter->get(relative_time);
            }
            relative_time -= iter->duration();
            if (relative_time < 0) {
                // SHouldn't happen
                return 0;
            }
            iter++;
        }

        // Also shouldn't happen
        return 0;
    }

    uint32_t duration() { return duration_; }

private:
    bool loop_;
    std::vector<Animation> sequence_;
    uint32_t duration_ = 0;
};