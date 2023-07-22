#include "app.h"
#include "Arduino.h"
#include "animated_program.h"
#include "invite_pulse.h"
#include "buildup_pulse.h"
#include "transmit_pulse.h"
#include "log.h"
#include "parser.h"
#include "transport.h"
#include <cstdint>
#include <cstring>

bool App::Init()
{
    pinMode(FSR_PIN, INPUT);

    for (int i = 0; i < kNumStrips; i++) {
        led_[i] = Led::CreateStrip(i);
        led_[i]->Init();
        switch (i) {
            case 0: led_[i]->FillColor(255, 0, 0); break;
            case 1: led_[i]->FillColor(0, 255, 0); break;
            case 2: led_[i]->FillColor(0, 0, 255); break;
        }
    }
    Led::Show();
    delay(500);

    if (!InitPrograms())
        return false;

    if (!NetworkInit())
        return false;

    EnterIdleState(0);

    return true;
}

bool App::InitPrograms()
{
    invite_pulse_ = Parser::ParseProgram(/*device=*/nullptr, kInvitePulse);
    if (!invite_pulse_)
        return false;

    buildup_pulse_ = Parser::ParseProgram(/*device=*/nullptr, kBuildupPulseJson);
    if (!buildup_pulse_)
        return false;

    transmit_pulse_ = Parser::ParseProgram(/*device=*/nullptr, kTransmitPulseJson);
    if (!transmit_pulse_)
        return false;

    return true;
}

bool App::NetworkInit()
{
    network_ = Network::Create();
    network_->Connect();

#if USE_HTTP_SERVER
    http_server_ = HttpServer::Create();
#endif

    for (int i = 0; i < kNumStrips; i++) {
        led_[i]->FillColor(0, 255, 0);
    }
    Led::Show();
    delay(500);

    uint8_t mac[7]{};
    network_->GetMacAddress(mac);
    mac[6] = 0;

    device_addr_.resize(6*3);
    snprintf(device_addr_.data(), device_addr_.size(), "%02x:%02x:%02x:%02x:%02x:%02x",
        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    std::vector<char> topic(32);
    snprintf(topic.data(), topic.size(), "area54/%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2],
             mac[3], mac[4], mac[5]);

    transport_ = Transport::Create({topic.data(), "area54/all"},
                                   [this](char* topic, uint8_t* payload, unsigned int length) {
                                       this->Callback(topic, payload, length);
                                   });

    while (!transport_->Connect()) {
        LOG("connect failed, try again in 5 seconds");
        delay(5000);
    }

    for (int i = 0; i < kNumStrips; i++) {
        led_[i]->FillColor(0, 0, 255);
    }
    Led::Show();

    return true;
}

void App::SetState(State state, uint32_t ms) {
    state_ = state;
    state_start_ms_ = ms;
    const char* state_names[] = { "IDLE", "PREPARE_TO_SEND", "SENDING", "SENT"};
    Serial.println("SetState " + String(state_names[state_]));
}

void App::EnterIdleState(uint32_t ms) {
    SetState(IDLE, ms);
    program_[0] = invite_pulse_.get();
    program_[0]->Start(ms);
    buildup_pulse_hue_ = kIdleHue;
}

void App::UpdateIdleState(uint32_t ms) {
    uint32_t program_duration = program_[0]->get_duration();

    // loop the pulsing animation
    if (program_duration && (ms - program_[0]->time_base() > program_duration)) {
        program_[0]->Start(ms);
    }

    StartIfPresent(ms);
}

void App::StartIfPresent(uint32_t ms) {
    if (presence_detected_ms_ > presence_acquired_ms_) {
        presence_acquired_ms_ = presence_detected_ms_;
        SetState(PREPARE_TO_SEND, ms);

        // TODO: send a message to allow for synchronization with others.
    }
}

void App::UpdatePresence(uint32_t ms) {
    int v = analogRead(FSR_PIN);

    //Serial.println("v=" + String(v));
    if (presence_detected_ms_) {
        if (ms - presence_detected_ms_ > 500 && v < 500) {
            presence_detected_ms_ = 0;
            Serial.println("PresenceRemoved: v=" + String(v));
        }
    } else if (v > 1500) {
        presence_detected_ms_ = ms;
        Serial.println("PresenceDetected: v=" + String(v));
    }
}

void App::SetBuildupPulseColor(uint8_t hue) {
    std::vector<Program*> programs { buildup_pulse_.get(), transmit_pulse_.get() };

    for (auto* program : programs) {
        auto animated_program = static_cast<AnimatedProgram*>(program);

        for (int i = 0; i < animated_program->segment_count(); i++) {
            auto segment = static_cast<AnimatedProgram::Segment*>(animated_program->GetSegment(i));
            segment->hue_gradient[0] = segment->hue_gradient[1] = hue;
        }
    }
}

// return opacity for strip 0
int App::UpdatePrepareToSendState(uint32_t ms) {
    // Fade out the idle animation
    // int opacity = 255 - 255 * (ms - state_start_ms_) / 250;
    // if (opacity > 0)
    //     return opacity;

    // After fade out completes, transmission begins with a buildup pulse.
    SetState(SENDING, ms);

    program_[0] = buildup_pulse_.get();
    program_[0]->Start(ms);

    SetBuildupPulseColor(buildup_pulse_hue_);
    if (buildup_pulse_hue_ < 240) {
        buildup_pulse_hue_ += 3;
    }
    //Serial.println("pulse hue: " + String(buildup_pulse_hue_));

    return 255;
}

void App::UpdateSendingState(uint32_t ms) {
    // Run to end of animation
    if (program_[0] && (ms - program_[0]->time_base()) > program_[0]->get_duration()) {
        program_[0] = nullptr;

        program_[1] = transmit_pulse_.get();
        program_[1]->Start(ms);
    }

    if (program_[1] && (ms - program_[1]->time_base()) > program_[1]->get_duration()) {
        program_[1] = nullptr;

        program_[2] = transmit_pulse_.get();
        program_[2]->Start(ms);
    }

    if (program_[2] && (ms - program_[2]->time_base()) > program_[2]->get_duration()) {
        program_[2] = nullptr;
        SetState(SENT, ms);

        // Inform the server
        if (transport_) {
            std::vector<uint8_t> bytes { buildup_pulse_hue_ };
            Serial.println("sent: " + String(buildup_pulse_hue_));
            transport_->Send(bytes);
        }
    }
}

void App::UpdateSentState(uint32_t ms) {
    StartIfPresent(ms);

    if (state_ == SENT && presence_detected_ms_ == 0) {
        SetState(READY, ms);
    }
}

void App::UpdateReadyState(uint32_t ms) {
    StartIfPresent(ms);

    if (ms - state_start_ms_ > 5000) {
        EnterIdleState(ms);
    }
}

void App::Update(uint32_t time_ms)
{
    int opacity = 255;

    UpdatePresence(time_ms);

    switch (state_) {
    case IDLE:
        UpdateIdleState(time_ms);
        break;

    case PREPARE_TO_SEND:
        opacity = UpdatePrepareToSendState(time_ms);
        break;

    case SENDING:
        UpdateSendingState(time_ms);
        break;

    case SENT:
        UpdateSentState(time_ms);
        break;

    case READY:
        UpdateReadyState(time_ms);
        break;
    }

    for (int i = 0; i < kNumStrips; i++) {
        led_[i]->FillColor(0, 0, 0);

        if (program_[i]) {
            for (uint32_t segment_index = 0; segment_index < program_[i]->segment_count();
                segment_index++) {
                auto segment = program_[i]->GetSegment(segment_index);
                if (segment)
                    led_[i]->DrawSegment(segment, time_ms - program_[i]->time_base());
            }

            uint8_t brightness;
            program_[i]->GetBrightness(time_ms - program_[i]->time_base(), &brightness);

            // Opacity applied only to strip 0
            // if (i == 0 && opacity < 255) {
            //     int temp = ((int)brightness * opacity) >> 8; // divide by 255/256
            //     brightness = temp;
            // }

            led_[i]->SetBrightness(brightness);
        }
    }
    Led::Show();

    if (transport_) {
        if (!transport_->IsConnected()) {
            LOG("lost connection, attempting to reconnect");
            transport_->Connect();
        }

        if (transport_->IsConnected())
            transport_->Loop();
    }

    if (http_server_)
        http_server_->Loop();
}

void App::Callback(char* topic, uint8_t* payload, unsigned int length)
{
    LOG("Callback topic %s length %u", topic, length);
    pending_program_ = Parser::ParseProgram(device_addr_.data(), reinterpret_cast<char*>(payload));
}
