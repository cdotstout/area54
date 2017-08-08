#pragma once

#include "led.h"
#include "network.h"
#include "program.h"
#include "transport.h"
#include <memory>

class App {
public:
    App();

    bool Init();

    void Update(uint32_t time_ms);

private:
    void Callback(char* topic, uint8_t* payload, unsigned int length);

    Led led_;
    std::unique_ptr<Program> program_;
    std::unique_ptr<Program> pending_program_;
    std::unique_ptr<Network> network_;
    std::unique_ptr<Transport> transport_;
};
