#pragma once

#include <memory>

class Transport {
public:
    virtual ~Transport() {}

    virtual void Connect(const char* topic) = 0;
    virtual void Loop() = 0;

    static std::unique_ptr<Transport>
    Create(std::function<void(char*, uint8_t*, unsigned int)> callback);
};
