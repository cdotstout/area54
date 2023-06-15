#include "led.h"
#include "log.h"
#include <cstdint>
#include <vector>

#if defined(ESP8266)
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#endif

#include "FastLED.h"

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

constexpr ESPIChipsets kLedType = LPD8806;
constexpr EOrder kColorOrder = GRB;
constexpr uint32_t kNumLeds = 54;

// Abstract: doesn't implement Init()
class FastLed : public Led {
public:
    FastLed() : num_leds_(kNumLeds), leds_(kNumLeds) {}

    void FillColor(uint8_t red, uint8_t green, uint8_t blue) override;
    void SetSegment(uint32_t first_index, uint32_t last_index, uint8_t red, uint8_t green,
                    uint8_t blue) override;
    void DrawSegment(Segment* segment, uint32_t time_ms) override;
    void SetBrightness(uint8_t brightness) override;

protected:
    uint32_t num_leds_;
    std::vector<CRGB> leds_;

    uint8_t red_ = 0;
    uint8_t green_ = 0;
    uint8_t blue_ = 0;
};

void FastLed::FillColor(uint8_t red, uint8_t green, uint8_t blue)
{
    red_ = red;
    green_ = green;
    blue_ = blue;
    fill_solid(leds_.data(), num_leds_, CRGB(red_, green_, blue_));
}

void FastLed::SetSegment(uint32_t first_index, uint32_t last_index, uint8_t red, uint8_t green,
                         uint8_t blue)
{
    if (first_index < 0)
        first_index = 0;
    if (last_index >= num_leds_)
        last_index = num_leds_ - 1;
    if (first_index > last_index)
        return;
    auto color = CRGB(red, green, blue);
    for (uint32_t i = first_index; i <= last_index; i++) {
        leds_[i] = color;
    }
}

void FastLed::DrawSegment(Segment* segment, uint32_t time_ms) { segment->Draw(leds_, time_ms); }

void FastLed::SetBrightness(uint8_t brightness) { FastLED.setBrightness(dim8_lin(brightness)); }

// static
void Led::Show() { FastLED.show(); }

class FastLedTripleStrip : public FastLed {
public:
    void Init() {
    #if defined(ESP32)
        // Rover board
        FastLED.addLeds<kLedType, 19, 18, kColorOrder>(leds_.data(), num_leds_)
            .setCorrection(TypicalLEDStrip);
        FastLED.addLeds<kLedType, 4, 0, kColorOrder>(leds_.data(), num_leds_)
            .setCorrection(TypicalLEDStrip);
        FastLED.addLeds<kLedType, 2, 15, kColorOrder>(leds_.data(), num_leds_)
            .setCorrection(TypicalLEDStrip);
    #else
        FastLED.addLeds<kLedType, 1, 2, kColorOrder>(leds_.data(), num_leds_)
            .setCorrection(TypicalLEDStrip);
        FastLED.addLeds<kLedType, 3, 4, kColorOrder>(leds_.data(), num_leds_)
            .setCorrection(TypicalLEDStrip);
        FastLED.addLeds<kLedType, 5, 6, kColorOrder>(leds_.data(), num_leds_)
            .setCorrection(TypicalLEDStrip);
    #endif
    }
};

std::unique_ptr<Led> Led::CreateTripleStrip() { return std::unique_ptr<Led>(new FastLedTripleStrip()); }

#if defined(ESP32)
class FastLedStrip2 : public FastLed {
public:
    void Init() override {
        FastLED.addLeds<kLedType, 4, 0, kColorOrder>(leds_.data(), num_leds_)
            .setCorrection(TypicalLEDStrip).clearLedData();
    }
};

class FastLedStrip1 : public FastLed {
public:
    void Init() override {
        FastLED.addLeds<kLedType, 2, 15, kColorOrder>(leds_.data(), num_leds_)
            .setCorrection(TypicalLEDStrip).clearLedData();
    }
};

class FastLedStrip0 : public FastLed {
public:
    void Init() override {
        FastLED.addLeds<kLedType, 19, 18, kColorOrder>(leds_.data(), num_leds_)
            .setCorrection(TypicalLEDStrip).clearLedData();
    }
};

std::unique_ptr<Led> Led::CreateStrip(int index) {
    switch (index) {
    case 0:
        return std::unique_ptr<Led>(new FastLedStrip0());
    case 1:
        return std::unique_ptr<Led>(new FastLedStrip1());
    case 2:
        return std::unique_ptr<Led>(new FastLedStrip2());
    default:
        return nullptr;
    }
}
#endif
