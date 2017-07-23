#include "app.h"
#include "animated_program.h"
#include "log.h"
#include "parser.h"
#include "transport.h"
#include <cstdint>

constexpr uint32_t kNumLeds = 160;

App::App() : led_(kNumLeds) {}

bool App::Init()
{
    led_.Init();

    network_ = Network::Create();
    network_->Connect();

    uint8_t mac[7]{};
    network_->GetMacAddress(mac);
    mac[6] = 0;

    std::vector<char> topic(32);
    snprintf(topic.data(), topic.size(), "area54/%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2],
             mac[3], mac[4], mac[5]);

    transport_ = Transport::Create([this](char* topic, uint8_t* payload, unsigned int length) {
        this->Callback(topic, payload, length);
    });
    transport_->Connect(topic.data());

    auto program = SimpleProgram::Create(255, 0, 255, 50);

    // auto program = std::unique_ptr<AnimatedProgram>(new AnimatedProgram());
    // program->SetColor(255, 0, 255);

    // auto sequence = std::unique_ptr<AnimationSequence>(new
    // AnimationSequence());
    // constexpr uint32_t kMaxBrightness = 255;
    // constexpr uint32_t kDurationMs = 3000;

    // sequence->Add(Animation{0, 0, kDurationMs});
    // sequence->Add(Animation{0, kMaxBrightness, kDurationMs});
    // sequence->Add(Animation{kMaxBrightness, 0, kDurationMs});
    // sequence->Add(Animation{0, kMaxBrightness, kDurationMs});
    // sequence->Add(Animation{kMaxBrightness, 0, kDurationMs});
    // sequence->Add(Animation{0, kMaxBrightness, kDurationMs});
    // sequence->Add(Animation{kMaxBrightness, 0, kDurationMs});

    // program->SetBrightness(std::move(sequence));

    program_ = std::move(program);

    return true;
}

void App::Update(uint32_t time_ms)
{
    if (program_) {
        uint8_t red, green, blue;
        program_->GetColor(time_ms, &red, &green, &blue);
        led_.SetColor(red, green, blue);

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
    std::unique_ptr<Program> program = Parser::ParseProgram(reinterpret_cast<char*>(payload));
    if (program) {
        LOG("Updating program");
        program_ = std::move(program);
    }
}
