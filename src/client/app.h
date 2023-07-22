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
    // Nothing lit. When presence removed, go to READY
    SENT,
    // Ready is like IDLE, but no glowy animation.  Lasts a brief time, then proceeds to IDLE.
    READY,
};

class App {
public:
    bool Init();

    void Update(uint32_t time_ms);

    static constexpr int kNumStrips = 3;
    static constexpr int kIdleHue = 150;

private:
    bool NetworkInit();
    bool InitPrograms();
    void Callback(char* topic, uint8_t* payload, unsigned int length);

    void StartIfPresent(uint32_t ms);
    void SetState(State state, uint32_t ms);
    void EnterIdleState(uint32_t ms);
    void UpdateIdleState(uint32_t time_ms);
    int  UpdatePrepareToSendState(uint32_t time_ms);
    void UpdateSendingState(uint32_t ms);
    void UpdateSentState(uint32_t ms);
    void UpdateReadyState(uint32_t ms);
    void UpdatePresence(uint32_t ms);
    void SetBuildupPulseColor(uint8_t hue);

    std::unique_ptr<Led> led_[kNumStrips];
    Program* program_[kNumStrips] { nullptr, nullptr, nullptr };

    std::unique_ptr<Program> invite_pulse_;
    std::unique_ptr<Program> buildup_pulse_;
    std::unique_ptr<Program> transmit_pulse_;
    std::unique_ptr<Program> pending_program_;
    std::unique_ptr<Network> network_;
    std::unique_ptr<Transport> transport_;
    std::unique_ptr<HttpServer> http_server_;
    std::vector<char> device_addr_;
    State state_ = IDLE;
    uint32_t state_start_ms_ = 0;
    uint32_t presence_detected_ms_ = 0;
    uint32_t presence_acquired_ms_ = 0;
    uint8_t buildup_pulse_hue_ = kIdleHue;
};
