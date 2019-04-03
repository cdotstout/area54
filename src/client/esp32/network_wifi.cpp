#include "../log.h"
#include "../network.h"

#include <WiFi.h>

class WifiNetwork : public Network {
public:
    // static constexpr char* kSsid = "area54";
    // static constexpr char* kPassword = "40420233";
    static constexpr char* kSsid = "Robot cart";
    static constexpr char* kPassword = "robodj9000";

    void Connect() override;
    void GetMacAddress(uint8_t mac[6]) override;
    long int Rssi();
};

void WifiNetwork::Connect()
{
    const char* ssid = kSsid;
    const char* password = kPassword;

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        LOG("Connecting to %s... rssi %ld", ssid, Rssi());
        delay(500);
    }

    LOG("wifi connected ip address: %s", WiFi.localIP().toString().c_str());
}

void WifiNetwork::GetMacAddress(uint8_t mac[6]) { WiFi.macAddress(mac); }

long int WifiNetwork::Rssi() { return WiFi.RSSI(); }

std::unique_ptr<Network> Network::Create() { return std::unique_ptr<Network>(new WifiNetwork()); }
