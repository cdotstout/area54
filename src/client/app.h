#pragma once

#include "http_server.h"
#include "led.h"
#include "network.h"
#include "program.h"
#include "transport.h"
#include <memory>

class App {
public:
    bool Init();

    void Update(uint32_t time_ms);

private:
    void Callback(char* topic, uint8_t* payload, unsigned int length);

    std::unique_ptr<Led> led_;
    std::unique_ptr<Program> program_;
    std::unique_ptr<Program> pending_program_;
    std::unique_ptr<Network> network_;
    std::unique_ptr<Transport> transport_;
    std::unique_ptr<HttpServer> http_server_;
    std::vector<char> device_addr_;
};
