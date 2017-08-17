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
        uint8_t red = json_object["red"];
        uint8_t green = json_object["green"];
        uint8_t blue = json_object["blue"];
        LOG("got AnimatedProgram color %u:%u:%u", red, green, blue);

        program->SetColor(red, green, blue);

        if (json_object.containsKey("segments")) {
            auto segments = std::vector<AnimatedProgram::Segment>();

            JsonArray& array = json_object["segments"];

            AnimatedProgram::Segment segment;
            segment.start_time = 0;

            for (uint32_t i = 0; i < array.size(); i++) {
                JsonObject& json_segment = array[i];

                uint32_t start = 0;
                uint32_t end = 0;
                uint32_t duration = 3000;

                if (json_segment.containsKey("0")) {
                    JsonObject& object = json_segment["0"];
                    start = object["start"];
                    LOG("got first %u", start);
                    end = object["end"];
                    LOG("got end %u", end);
                    duration = object["duration"];
                    LOG("got duration %u", duration);
                    segment.animation[0] = Animation{start, end, duration};
                }

                if (json_segment.containsKey("1")) {
                    JsonObject& object = json_segment["1"];
                    start = object["start"];
                    LOG("got first %u", start);
                    end = object["end"];
                    LOG("got end %u", end);
                    duration = object["duration"];
                    LOG("got duration %u", duration);
                    segment.animation[1] = Animation{start, end, duration};
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
                uint8_t start = 0;
                uint8_t end = 0;
                uint32_t duration = 3000;

                JsonObject& brightness = brightness_array[i];
                if (brightness.containsKey("start")) {
                    start = brightness["start"];
                    LOG("got start %u", start);
                }

                if (brightness.containsKey("end")) {
                    end = brightness["end"];
                    LOG("got end %u", end);
                }

                if (brightness.containsKey("duration")) {
                    duration = brightness["duration"];
                    LOG("got duration %u", duration);
                }

                sequence->Add(Animation{start, end, duration});
            }

            program->SetBrightness(std::move(sequence));
        }

        return std::move(program);
    }

    return DRETP(nullptr, "unknown program: %s", name);
}
