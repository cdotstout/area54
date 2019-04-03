#include "../log.h"
#include "../transport.h"

#include "../log.h"
#include "../transport.h"

#include <vector>
#include <WiFi.h>
#include <WiFiUdp.h>

class UdpTransport : public Transport {
public:
    UdpTransport(std::function<void(char*, uint8_t*, unsigned int)> callback)
        : buffer_(20000), callback_(callback)
    {
    }

    bool IsConnected() override { return WiFi.status() == WL_CONNECTED; }

    bool Connect() override;

    void Loop() override 
    { 
    	int packetSize = wifi_udp_.parsePacket();
    	if (packetSize) {
    		//LOG("*** UDP available size %d buffer size %d\n", packetSize, buffer_.size());
    		if (packetSize < buffer_.size()) {
    			int ret = wifi_udp_.read(buffer_.data(), packetSize);
    			if (ret != packetSize) {
    				LOG("read failed: %d", ret);
    				return;
    			} 
    			callback_("topic", buffer_.data(), packetSize);
    		}
    	}
    }

private:
    void callback(char* topic, byte* payload, unsigned int length);

    WiFiUDP wifi_udp_;
    std::vector<unsigned char> buffer_;
    std::function<void(char*, uint8_t*, unsigned int)> callback_;
};

bool UdpTransport::Connect()
{
	//if (!wifi_udp_.beginMulticast(IPAddress(239,255,255,255), 3333)) {
	if (!wifi_udp_.beginMulticast(IPAddress(224,1,1,1), 5007)) {
		LOG("beginMulticast failed");
		return false;
	}
    return true;
}

std::unique_ptr<Transport>
Transport::Create(std::vector<std::string> topics,
                  std::function<void(char*, uint8_t*, unsigned int)> callback)
{
    return std::unique_ptr<Transport>(new UdpTransport(callback));
}
