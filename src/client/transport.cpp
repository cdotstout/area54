#include "transport.h"
#include "log.h"

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#if MQTT_MAX_PACKET_SIZE != 1024
#error Unexpected MQTT_MAX_PACKET_SIZE - set to 1024 in PubSubClient.h
#endif

class MqttTransport : public Transport {
public:
    //static constexpr char* kServer = "192.168.44.102";
    static constexpr char* kServer = "192.168.0.10";
    static constexpr uint32_t kPort = 1883;

    MqttTransport(std::function<void(char*, uint8_t*, unsigned int)> callback)
        : client_(wifi_client_), callback_(callback)
    {
    }

    void Connect(std::vector<const char*> topics) override;

    void Loop() override
    {
        if (!client_.connected())
            LOG("PubSubClient not connected");
        client_.loop();
    }

private:
    void callback(char* topic, byte* payload, unsigned int length);

    WiFiClient wifi_client_;
    PubSubClient client_;
    std::function<void(char*, uint8_t*, unsigned int)> callback_;
};

void MqttTransport::Connect(std::vector<const char*> topics)
{
    randomSeed(micros());

    client_.setServer(kServer, kPort);
    client_.setCallback(callback_);

    // Loop until we're reconnected
    while (!client_.connected()) {
        LOG("Attempting MQTT connection to %s:%u", kServer, kPort);

        // Create a random client ID
        String clientId = "ESP8266Client-";
        clientId += String(random(0xffff), HEX);

        if (client_.connect(clientId.c_str()))
            break;

        LOG("connect failed, rc=%d", client_.state());
        LOG(" try again in 5 seconds");
        delay(5000);
    }

    for (const char* topic : topics) {
        LOG("connected, subscribing to topic %s", topic);
        client_.subscribe(topic);
    }
}

std::unique_ptr<Transport>
Transport::Create(std::function<void(char*, uint8_t*, unsigned int)> callback)
{
    return std::unique_ptr<Transport>(new MqttTransport(callback));
}
