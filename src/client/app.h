#pragma once

#include "http_server.h"
#include "led.h"
#include "network.h"
#include "program.h"
#include "transport.h"
#include <memory>

constexpr int FSR_PIN = 32;

enum State {
    // From IDLE: subtle glow animation. If presence detected, go to PREPARE_TO_SEND.
    IDLE,
    // From PREPARE_TO_SEND: send a message.  Count messages received to
    // inform animation. Idle animation goes away. After a short time, go to SENDING.
    PREPARE_TO_SEND,
    // Animate the transmission.  When animation completes, go to SENT.
    SENDING,
    // Nothing lit. When presence removed, then go to IDLE.
    SENT,
};

class App {
public:
    bool Init();

    void Update(uint32_t time_ms);

    static constexpr int kNumStrips = 3;

private:
    bool NetworkInit();
    bool InitPrograms();
    void Callback(char* topic, uint8_t* payload, unsigned int length);

    void SetState(State state, uint32_t ms) {
        state_ = state;
        state_start_ms_ = ms;
    }

    void EnterIdleState(uint32_t ms);
    void UpdateIdleState(uint32_t time_ms);
    int  UpdatePrepareToSendState(uint32_t time_ms);
    void UpdateSendingState(uint32_t ms);
    bool IsPresenceDetected();

    std::unique_ptr<Led> led_[kNumStrips];
    Program* program_[kNumStrips] { nullptr, nullptr, nullptr };

    std::unique_ptr<Program> invite_pulse_;
    std::unique_ptr<Program> buildup_pulse_;
    std::unique_ptr<Program> pending_program_;
    std::unique_ptr<Network> network_;
    std::unique_ptr<Transport> transport_;
    std::unique_ptr<HttpServer> http_server_;
    std::vector<char> device_addr_;
    State state_ = IDLE;
    uint32_t state_start_ms_ = 0;
};
