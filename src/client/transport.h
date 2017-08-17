#pragma once

#include <memory>
#include <vector>

class Transport {
public:
    virtual ~Transport() {}

    virtual void Connect(std::vector<const char*> topics) = 0;
    virtual void Loop() = 0;

    static std::unique_ptr<Transport>
    Create(std::function<void(char*, uint8_t*, unsigned int)> callback);
};
