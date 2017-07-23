#pragma once

#include "animation.h"
#include <memory>
#include <vector>

extern "C" unsigned long millis();

class Program {
public:
    Program() { Start(); }

    virtual ~Program() {}
    virtual void GetColor(uint32_t time_ms, uint8_t* red_out, uint8_t* green_out,
                          uint8_t* blue_out) = 0;
    virtual void GetBrightness(uint32_t time_ms, uint8_t* brightness_out) = 0;

    void Start()
    {
        time_base_ = millis();
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

    void GetColor(uint32_t time_ms, uint8_t* red_out, uint8_t* green_out,
                  uint8_t* blue_out) override
    {
        *red_out = red_;
        *green_out = green_;
        *blue_out = blue_;
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
