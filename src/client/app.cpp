#include "app.h"
#include "animated_program.h"
#include "log.h"
#include "parser.h"
#include "transport.h"
#include <cstdint>
#include <cstring>

extern "C" void delay(long unsigned int ms);

bool App::Init()
{
    led_ = Led::Create();
    led_->Init();
    led_->FillColor(255, 0, 0);
    led_->Show();
    delay(500);

    if (!InitInvitePulse())
        return false;

    return true;
}

bool App::InitInvitePulse()
{
    const char* kInvitePulseJson = R"(
    {
        "name": "AnimatedProgram",
        "segments": [
            {
                "duration": "3000",
                "keepalive": "0",
                "0": { "s": "0", "e": "0" },
                "1": { "s": "53", "e": "53" },
                "hue_grad": { "s": "240", "e": "240" }
            },
            {
                "start_time": "3000",
                "duration": "3000",
                "keepalive": "0",
                "0": { "s": "0", "e": "0" },
                "1": { "s": "53", "e": "53" },
                "hue_grad": { "s": "160", "e": "160" }
            }
        ],
        "brightness": [
            { "s": "0", "e": "255", "d": "1000" },
            { "s": "255", "e": "255", "d": "1000" },
            { "s": "255", "e": "0", "d": "1000" },
            { "s": "0", "e": "255", "d": "1000" },
            { "s": "255", "e": "255", "d": "1000" },
            { "s": "255", "e": "0", "d": "1000" }
        ]
    })";

    invite_pulse_ = Parser::ParseProgram(/*device=*/nullptr, kInvitePulseJson);
    if (!invite_pulse_)
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

    led_->FillColor(0, 255, 0);
    led_->Show();
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

    led_->FillColor(0, 0, 255);
    led_->Show();

    return true;
}

void App::Update(uint32_t time_ms)
{
    if (!program_) {
        program_ = invite_pulse_.get();
    }

    uint32_t program_duration = program_->get_duration();
    if (program_duration && (time_ms - program_->time_base() > program_duration)) {
        program_->Start(time_ms);
    }

    if (pending_program_) {
        program_ = pending_program_.get();
        program_->Start(time_ms);
    }

    if (program_) {
        led_->FillColor(0, 0, 0);

        for (uint32_t segment_index = 0; segment_index < program_->segment_count();
             segment_index++) {
            auto segment = program_->GetSegment(segment_index);
            if (segment)
                led_->DrawSegment(segment, time_ms - program_->time_base());
        }

        uint8_t brightness;
        program_->GetBrightness(time_ms - program_->time_base(), &brightness);
        led_->SetBrightness(brightness);
    }

    led_->Show();

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
