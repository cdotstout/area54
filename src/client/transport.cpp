#if defined(ESP8266)
#include "esp8266/transport_mqtt.cpp"
#elif defined(ESP32)
#include "esp32/transport_ble.cpp"
#else
#error No transport specified.
#endif
