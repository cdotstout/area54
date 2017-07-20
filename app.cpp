#include "app.h"
#include "animated_program.h"
#include <cstdint>

constexpr uint32_t kNumLeds = 160;

App::App() : led_(kNumLeds) {}

void App::Init()
{
	led_.Init();

	//auto program = SimpleProgram::Create(255,0,255);
	auto program = std::unique_ptr<AnimatedProgram>(new AnimatedProgram());
	program->SetColor(255, 0, 255);

	auto sequence = std::unique_ptr<AnimationSequence>(new AnimationSequence());
	constexpr uint32_t kMaxBrightness = 255;
	constexpr uint32_t kDurationMs = 3000;

	sequence->Add(Animation{0, 0, kDurationMs});
	sequence->Add(Animation{0, kMaxBrightness, kDurationMs});
	sequence->Add(Animation{kMaxBrightness, 0, kDurationMs});
	sequence->Add(Animation{0, kMaxBrightness, kDurationMs});
	sequence->Add(Animation{kMaxBrightness, 0, kDurationMs});
	sequence->Add(Animation{0, kMaxBrightness, kDurationMs});
	sequence->Add(Animation{kMaxBrightness, 0, kDurationMs});

	program->SetBrightness(std::move(sequence));

	program_ = std::move(program);
}

void App::Update(uint32_t time_ms)
{
	uint8_t red, green, blue;
	program_->GetColor(time_ms, &red, &green, &blue);
	led_.SetColor(red, green, blue);

	uint8_t brightness;
	program_->GetBrightness(time_ms, &brightness);
	led_.SetBrightness(brightness);

	led_.Show();
}
