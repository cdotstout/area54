#pragma once

#include <cstdint>
#include <memory>

class Led {
public:
    virtual ~Led() {}

    virtual void Init() = 0;
    virtual void Clear() = 0;
    virtual void FillColor(uint8_t red, uint8_t green, uint8_t blue) = 0;
    virtual void SetSegment(uint32_t first_index, uint32_t last_index, uint8_t red, uint8_t green,
                            uint8_t blue) = 0;
    virtual void SetBrightness(uint8_t brightness) = 0;
    virtual void Show() = 0;

    static std::unique_ptr<Led> Create();
};
