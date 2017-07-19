#include "led.h"
#include <cstdint>

#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#include "FastLED.h"

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define BRIGHTNESS          96

#define DATA_PIN    1
#define CLK_PIN     2
#define LED_TYPE    LPD8806
#define COLOR_ORDER GRB

FASTLED_USING_NAMESPACE

Led::Led(uint32_t num_leds) : num_leds_(num_leds) 
{
	leds_ = new CRGB[num_leds_];
}

Led::~Led() { delete leds_; }

void Led::Init()
{
	FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds_, num_leds_).setCorrection(TypicalLEDStrip);
	FastLED.setBrightness(BRIGHTNESS);

	constexpr uint32_t kHue = 0;
	fill_rainbow(leds_, num_leds_, kHue, 7);
}

void Led::Show()
{
	FastLED.show();
}

void Led::Delay(uint32_t ms)
{  
	FastLED.delay(ms);
}
