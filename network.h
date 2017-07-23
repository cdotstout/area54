#pragma once

#include <memory>

class Network {
public:
    virtual ~Network() {}

    virtual void Connect() = 0;
    virtual void GetMacAddress(uint8_t mac[6]) = 0;

    static std::unique_ptr<Network> Create();
};
