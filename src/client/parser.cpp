#include "parser.h"

#include "animated_program.h"
#include "log.h"
#include <ArduinoJson.h>

std::unique_ptr<Program> Parser::ParseProgram(const char* json)
{
    StaticJsonBuffer<2048> jsonBuffer;

    JsonObject& json_object = jsonBuffer.parseObject(json);
    if (!json_object.success())
        return DRETP(nullptr, "failed to parse json message: %s", json);

    const char* name = json_object["name"];
    LOG("got name %s", name);
    if (!name)
        return DRETP(nullptr, "name is null");

    if (strcmp(name, "SimpleProgram") == 0) {
        uint8_t red = json_object["red"];
        uint8_t green = json_object["green"];
        uint8_t blue = json_object["blue"];
        uint8_t brightness = json_object["brightness"];
        LOG("got SimpleProgram color %u:%u:%u brightness %u", red, green, blue, brightness);

        return SimpleProgram::Create(red, green, blue, brightness);
    }

    if (strcmp(name, "AnimatedProgram") == 0) {
        auto program = std::unique_ptr<AnimatedProgram>(new AnimatedProgram());

        auto color_sequence = std::vector<std::unique_ptr<AnimationSequence>>(3);
        auto color = json_object["color"];

        auto hue = color["h"];
        color_sequence[0].reset(new AnimationSequence());
        color_sequence[0]->Add(Animation{hue["s"], hue["e"], hue["d"]});

        auto saturation = color["s"];
        color_sequence[1].reset(new AnimationSequence());
        color_sequence[1]->Add(Animation{saturation["s"], saturation["e"], saturation["d"]});

        auto value = color["v"];
        color_sequence[2].reset(new AnimationSequence());
        color_sequence[2]->Add(Animation{value["s"], value["e"], value["d"]});

        program->SetColor(std::move(color_sequence));

        if (json_object.containsKey("segments")) {
            auto segments = std::vector<AnimatedProgram::Segment>();

            JsonArray& array = json_object["segments"];

            AnimatedProgram::Segment segment;
            segment.start_time = 0;

            for (uint32_t i = 0; i < array.size(); i++) {
                JsonObject& json_segment = array[i];

                if (json_segment.containsKey("0")) {
                    JsonObject& object = json_segment["0"];
                    segment.animation[0] = Animation{object["s"], object["e"], object["d"]};
                }

                if (json_segment.containsKey("1")) {
                    JsonObject& object = json_segment["1"];
                    segment.animation[1] = Animation{object["s"], object["e"], object["d"]};
                }

                if (json_segment.containsKey("start_time")) {
                    segment.start_time = json_segment["start_time"];
                }

                segments.push_back(segment);
            }

            program->SetSegments(std::move(segments));
        }

        if (json_object.containsKey("brightness")) {
            auto sequence = std::unique_ptr<AnimationSequence>(new AnimationSequence());

            JsonArray& brightness_array = json_object["brightness"];

            for (uint32_t i = 0; i < brightness_array.size(); i++) {
                JsonObject& brightness = brightness_array[i];
                sequence->Add(Animation{brightness["s"], brightness["e"], brightness["d"]});
            }

            program->SetBrightness(std::move(sequence));
        }

        return std::move(program);
    }

    return DRETP(nullptr, "unknown program: %s", name);
}
