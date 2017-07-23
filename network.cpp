#include "network.h"
#include "log.h"

#include <ESP8266WiFi.h>

class WifiNetwork : public Network {
public:
    // const char* kSsid = "Jersey";
    // const char* kPassword = "Johnny5isal!v3";
    static constexpr char* kSsid = "Mojave";
    static constexpr char* kPassword = "2856d18b84c94ab1b1e3";

    void Connect() override;
    void GetMacAddress(uint8_t mac[6]) override;
};

void WifiNetwork::Connect()
{
    const char* ssid = kSsid;
    const char* password = kPassword;

    WiFi.hostname("LED2");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        LOG("Connecting to %s...", ssid);
        delay(500);
    }

    LOG("wifi connected ip address: %s", WiFi.localIP().toString().c_str());
}

void WifiNetwork::GetMacAddress(uint8_t mac[6]) { WiFi.macAddress(mac); }

std::unique_ptr<Network> Network::Create() { return std::unique_ptr<Network>(new WifiNetwork()); }
