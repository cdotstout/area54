#include "transport.h"
#include "log.h"

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#if MQTT_MAX_PACKET_SIZE != 1284
#error Unexpected MQTT_MAX_PACKET_SIZE - set to 1284 in PubSubClient.h
#endif

class MqttTransport : public Transport {
public:
    //static constexpr char* kServer = "192.168.44.102";
    static constexpr char* kServer = "192.168.0.10";
    static constexpr uint32_t kPort = 1883;

    MqttTransport(std::vector<std::string> topics,
                  std::function<void(char*, uint8_t*, unsigned int)> callback)
        : topics_(std::move(topics)), client_(wifi_client_), callback_(callback)
    {
    }

    bool IsConnected() override { return client_.connected(); }

    bool Connect() override;

    void Loop() override { client_.loop(); }

private:
    void callback(char* topic, byte* payload, unsigned int length);

    std::vector<std::string> topics_;
    WiFiClient wifi_client_;
    PubSubClient client_;
    std::function<void(char*, uint8_t*, unsigned int)> callback_;
};

bool MqttTransport::Connect()
{
    if (IsConnected())
        return true;

    randomSeed(micros());

    client_.setServer(kServer, kPort);
    client_.setCallback(callback_);

    LOG("Attempting MQTT connection to %s:%u", kServer, kPort);

    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    if (!client_.connect(clientId.c_str())) {
        LOG("connect failed, rc=%d", client_.state());
        return false;
    }

    for (auto& topic : topics_) {
        LOG("connected, subscribing to topic %s", topic.c_str());
        client_.subscribe(topic.c_str());
    }

    return true;
}

std::unique_ptr<Transport>
Transport::Create(std::vector<std::string> topics,
                  std::function<void(char*, uint8_t*, unsigned int)> callback)
{
    return std::unique_ptr<Transport>(new MqttTransport(std::move(topics), callback));
}
