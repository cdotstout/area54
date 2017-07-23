#include "parser.h"

#include "animated_program.h"
#include "log.h"
#include <ArduinoJson.h>

std::unique_ptr<Program> Parser::ParseProgram(const char* json)
{
    StaticJsonBuffer<1024> jsonBuffer;

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

        auto sequence = std::unique_ptr<AnimationSequence>(new AnimationSequence());

        if (json_object.containsKey("brightness")) {
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
        }

        program->SetColor(red, green, blue);
        program->SetBrightness(std::move(sequence));

        return std::move(program);
    }

    return DRETP(nullptr, "unknown program: %s", name);
}
