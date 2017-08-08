#include "led.h"
#include "log.h"
#include <cstdint>

#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#include "FastLED.h"

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

constexpr ESPIChipsets kLedType = LPD8806;
constexpr uint8_t kDataPin = 1;
constexpr uint8_t kClockPin = 2;
constexpr EOrder kColorOrder = GRB;

Led::Led(uint32_t num_leds) : num_leds_(num_leds) { leds_ = new CRGB[num_leds_]; }

Led::~Led() { delete leds_; }

void Led::Clear() { FastLED.clear(true); }

void Led::FillColor(uint8_t red, uint8_t green, uint8_t blue)
{
    red_ = red;
    green_ = green;
    blue_ = blue;
    fill_solid(leds_, num_leds_, CRGB(red_, green_, blue_));
}

void Led::SetSegment(uint32_t first_index, uint32_t last_index, uint8_t red, uint8_t green,
                     uint8_t blue)
{
    LOG("SetSegment %u %u", first_index, last_index);
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

void Led::SetBrightness(uint8_t brightness) { FastLED.setBrightness(dim8_lin(brightness)); }

void Led::Init()
{
    FastLED.addLeds<kLedType, kDataPin, kClockPin, kColorOrder>(leds_, num_leds_)
        .setCorrection(TypicalLEDStrip);
    Clear();
}

void Led::Show() { FastLED.show(); }
