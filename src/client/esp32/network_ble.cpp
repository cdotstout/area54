#include "../log.h"
#include "../network.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include <memory>

// TODO: generate these
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

class NetworkBle : public Network, public BLEServerCallbacks, BLECharacteristicCallbacks {
public:
	NetworkBle(std::function<void(char*, const uint8_t*, unsigned int)> callback) : callback_(callback) {}
	bool Init();

	void Connect() override;
    void GetMacAddress(uint8_t mac[6]) override;

private:
	// BLEServerCallbacks
    void onConnect(BLEServer* pServer) override 
    {
        device_connected_ = true;
        LOG("CONNECTED");
    };

    void onDisconnect(BLEServer* pServer) override
    {
      	device_connected_ = false;
      	LOG("DISCONNECTED");
    }

    // BLECharacteristicCallbacks
    void onWrite(BLECharacteristic *pCharacteristic) 
    {
      std::string rxValue = pCharacteristic->getValue();

      if (rxValue.length() > 0) {
        LOG("********* incoming message length %d", rxValue.length());
        //LOG("Received Value: %s", rxValue.c_str());
      	callback_("ble", reinterpret_cast<const uint8_t*>(rxValue.c_str()), rxValue.length());
        // for (int i = 0; i < rxValue.length(); i++) {
        //   LOG("%c ", rxValue[i]);
        // }

        // LOG("");

        // Do stuff based on the command received from the app
        // if (rxValue.find("A") != -1) { 
        //   Serial.println("Turning ON!");
        //   digitalWrite(LED, HIGH);
        // }
        // else if (rxValue.find("B") != -1) {
        //   Serial.println("Turning OFF!");
        //   digitalWrite(LED, LOW);
        // }
        //LOG("*********");
      }
    }

private:
	bool device_connected_ = false;
	std::unique_ptr<BLEServer> server_;
	std::unique_ptr<BLEService> service_;
	std::unique_ptr<BLECharacteristic> tx_characteristic_;
	std::unique_ptr<BLECharacteristic> rx_characteristic_;
	std::function<void(char*, const uint8_t*, unsigned int)> callback_;
};

bool NetworkBle::Init()
{
    BLEDevice::init("NetworkBle");

    server_ = std::unique_ptr<BLEServer>(BLEDevice::createServer());
	server_->setCallbacks(this);

	service_ = std::unique_ptr<BLEService>(server_->createService(SERVICE_UUID));

    tx_characteristic_ = std::unique_ptr<BLECharacteristic>(service_->createCharacteristic(
                      CHARACTERISTIC_UUID_TX,
                      BLECharacteristic::PROPERTY_NOTIFY
                    ));
                      
    tx_characteristic_->addDescriptor(new BLE2902());

  	rx_characteristic_ = std::unique_ptr<BLECharacteristic>(service_->createCharacteristic(
                                         CHARACTERISTIC_UUID_RX,
                                         BLECharacteristic::PROPERTY_WRITE
                                       ));
  	rx_characteristic_->setCallbacks(this);

	return true;
}

void NetworkBle::GetMacAddress(uint8_t mac[6]) 
{ 
	// TODO
}

void NetworkBle::Connect()
{
	service_->start();
  	server_->getAdvertising()->start();
  	LOG("Connect: advertising started");
}

std::unique_ptr<Network> Network::Create(std::function<void(char*, const uint8_t*, unsigned int)> callback) 
{
	BLEDevice::init("NetworkBle");

	auto network = std::unique_ptr<NetworkBle>(new NetworkBle(callback));

	if (!network->Init())
		return DRETP(nullptr, "network init failed");

	return network;
}
