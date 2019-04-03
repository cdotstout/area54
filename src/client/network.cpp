#if defined(ESP8266)
#include "esp8266/network_wifi.cpp"
#elif defined(ESP32)
#include "esp32/network_wifi.cpp"
#else
#warning No network specified.
#endif
