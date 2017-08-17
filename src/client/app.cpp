#include "app.h"
#include "animated_program.h"
#include "log.h"
#include "parser.h"
#include "transport.h"
#include <cstdint>

constexpr uint32_t kNumLeds = 54;

App::App() : led_(kNumLeds) {}

extern "C" void delay(uint32_t ms);

bool App::Init()
{
    led_.Init();
    led_.FillColor(255, 0, 0);
    led_.Show();
    delay(500);

    network_ = Network::Create();
    network_->Connect();

    led_.FillColor(0, 255, 0);
    led_.Show();
    delay(500);

    uint8_t mac[7]{};
    network_->GetMacAddress(mac);
    mac[6] = 0;

    std::vector<char> topic(32);
    snprintf(topic.data(), topic.size(), "area54/%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2],
             mac[3], mac[4], mac[5]);

    transport_ = Transport::Create([this](char* topic, uint8_t* payload, unsigned int length) {
        this->Callback(topic, payload, length);
    });
    transport_->Connect({topic.data(), "area54/all"});

    led_.FillColor(0, 0, 255);
    led_.Show();

    return true;
}

void App::Update(uint32_t time_ms)
{
    if (pending_program_) {
        program_ = std::move(pending_program_);
        program_->Start(time_ms);
    }

    if (program_) {
        uint8_t red, green, blue;
        program_->GetColor(time_ms, &red, &green, &blue);

        led_.FillColor(0, 0, 0);

        for (uint32_t segment_index = 0; segment_index < program_->segment_count();
             segment_index++) {
            uint32_t first_index, last_index;
            if (program_->GetSegment(time_ms, segment_index, &first_index, &last_index))
                led_.SetSegment(first_index, last_index, red, green, blue);
        }

        uint8_t brightness;
        program_->GetBrightness(time_ms, &brightness);
        led_.SetBrightness(brightness);
    }

    led_.Show();

    transport_->Loop();
}

void App::Callback(char* topic, uint8_t* payload, unsigned int length)
{
    LOG("Callback topic %s length %u", topic, length);
    pending_program_ = Parser::ParseProgram(reinterpret_cast<char*>(payload));
}
