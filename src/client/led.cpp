#include "led.h"
#include "log.h"
#include <cstdint>
#include <vector>

#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#include "FastLED.h"

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

constexpr ESPIChipsets kLedType = LPD8806;
constexpr EOrder kColorOrder = GRB;
constexpr uint32_t kNumLeds = 54;

class FastLed : public Led {
public:
    FastLed(uint32_t num_leds) : num_leds_(num_leds), leds_(num_leds_) {}

    void Init() override;
    void Clear() override;
    void FillColor(uint8_t red, uint8_t green, uint8_t blue) override;
    void SetSegment(uint32_t first_index, uint32_t last_index, uint8_t red, uint8_t green,
                    uint8_t blue) override;
    void DrawSegment(Segment* segment, uint32_t time_ms) override;
    void SetBrightness(uint8_t brightness) override;
    void Show() override;

private:
    uint32_t num_leds_;
    std::vector<CRGB> leds_;

    uint8_t red_ = 0;
    uint8_t green_ = 0;
    uint8_t blue_ = 0;
};

void FastLed::Clear() { FastLED.clear(true); }

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

void FastLed::DrawSegment(Segment* segment, uint32_t time_ms)
{
    segment->Draw(leds_.data(), time_ms);
}

void FastLed::SetBrightness(uint8_t brightness) { FastLED.setBrightness(dim8_lin(brightness)); }

void FastLed::Init()
{
    FastLED.addLeds<kLedType, 1, 2, kColorOrder>(leds_.data(), num_leds_)
        .setCorrection(TypicalLEDStrip);
    FastLED.addLeds<kLedType, 3, 4, kColorOrder>(leds_.data(), num_leds_)
        .setCorrection(TypicalLEDStrip);
    FastLED.addLeds<kLedType, 5, 6, kColorOrder>(leds_.data(), num_leds_)
        .setCorrection(TypicalLEDStrip);
    Clear();
}

void FastLed::Show() { FastLED.show(); }

std::unique_ptr<Led> Led::Create() { return std::unique_ptr<Led>(new FastLed(kNumLeds)); }
