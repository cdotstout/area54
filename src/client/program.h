#pragma once

#include "animation.h"
#include "led.h"
#include <limits>
#include <memory>
#include <vector>

class Program {
public:
    static constexpr uint32_t kMaxIndex = UINT32_MAX;

    virtual ~Program() {}

    virtual void GetBrightness(uint32_t time_ms, uint8_t* brightness_out) = 0;

    virtual uint32_t segment_count() { return 0; }

    virtual Led::Segment* GetSegment(uint32_t segment_index) { return nullptr; }

    virtual uint32_t get_duration() { return 0; }

    void Start(uint32_t ms)
    {
        time_base_ = ms;
        LOG("Got new timebase %u", time_base_);
    }

    uint32_t time_base() { return time_base_; }

private:
    uint32_t time_base_ = 0;
};

class SimpleProgram : public Program {
public:
    static std::unique_ptr<Program> Create(uint8_t red, uint8_t green, uint8_t blue,
                                           uint8_t brightness)
    {
        auto program = std::unique_ptr<SimpleProgram>(new SimpleProgram);
        program->red_ = red;
        program->green_ = green;
        program->blue_ = blue;
        program->brightness_ = brightness;
        return std::move(program);
    }

    void GetBrightness(uint32_t time_ms, uint8_t* brightness_out) override
    {
        *brightness_out = brightness_;
    }

private:
    uint8_t red_ = 0;
    uint8_t green_ = 0;
    uint8_t blue_ = 0;
    uint8_t brightness_ = 255;
};
