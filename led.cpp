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

Led::Led(uint32_t num_leds) : num_leds_(num_leds) 
{
	leds_ = new CRGB[num_leds_];
}

Led::~Led() { delete leds_; }

void Led::SetColor(uint8_t red, uint8_t green, uint8_t blue)
{
	if (red == red_ && green == green_ && blue == blue_)
		return;

	red_ = red;
	green_ = green;
	blue_ = blue;
	fill_solid(leds_, num_leds_, CRGB(red_, green_, blue_));
}

void Led::SetBrightness(uint8_t brightness) { FastLED.setBrightness(dim8_lin(brightness)); }

void Led::Init()
{
	FastLED.addLeds<kLedType,kDataPin,kClockPin,kColorOrder>(leds_, num_leds_).setCorrection(TypicalLEDStrip);
	SetColor(0,0,255);
}

void Led::Show() { FastLED.show(); }
