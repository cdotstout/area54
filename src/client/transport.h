#pragma once

#include <functional>
#include <memory>
#include <vector>

class Transport {
public:
    virtual ~Transport() {}

    virtual bool IsConnected() = 0;
    virtual bool Connect() = 0;
    virtual void Loop() = 0;

    static std::unique_ptr<Transport>
    Create(std::vector<std::string> topics,
           std::function<void(char*, uint8_t*, unsigned int)> callback);
};
