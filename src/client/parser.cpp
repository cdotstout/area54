#include "parser.h"

#include "animated_program.h"
#include "log.h"
#include <ArduinoJson.h>

std::unique_ptr<Program> Parser::ParseProgram(const char* this_device, const char* json)
{
    StaticJsonDocument<4096> doc;

    auto result = deserializeJson(doc, json);
    if (result != DeserializationError::Ok)
        return DRETP(nullptr, "failed to parse json message: %s", json);

    bool should_parse = true;

    const char* device = doc["device"];
    if (device) {
        LOG("got device %s this_device %s", device, this_device);
        if (strcasecmp(device, this_device) != 0) {
            should_parse = false;
        }
    } else {
        JsonArray device_array = doc["device"];
        for (uint32_t i = 0; i < device_array.size(); i++) {
            should_parse = false;
            device = device_array[i];
            LOG("got device[%d] %s this_device %s", i, device, this_device);
            if (strcasecmp(device, this_device) == 0) {
                should_parse = true;
                break;
            }
        }
    }

    if (!should_parse)
        return nullptr;

    const char* name = doc["name"];
    if (name)
        LOG("got name %s", name);

    if (!name)
        return DRETP(nullptr, "name is null");

    if (strcmp(name, "SimpleProgram") == 0) {
        uint8_t red = doc["red"];
        uint8_t green = doc["green"];
        uint8_t blue = doc["blue"];
        uint8_t brightness = doc["brightness"];
        LOG("got SimpleProgram color %u:%u:%u brightness %u", red, green, blue, brightness);

        return SimpleProgram::Create(red, green, blue, brightness);
    }

    if (strcmp(name, "AnimatedProgram") == 0) {
        auto program = std::unique_ptr<AnimatedProgram>(new AnimatedProgram());

        if (doc.containsKey("segments")) {
            auto segments = std::vector<std::unique_ptr<AnimatedProgram::Segment>>();

            JsonArray array = doc["segments"];

            for (uint32_t i = 0; i < array.size(); i++) {
                JsonObject json_segment = array[i];

                auto segment =
                    std::unique_ptr<AnimatedProgram::Segment>(new AnimatedProgram::Segment);
                segment->start_time = 0;
                segment->keepalive = false;

                uint32_t duration = json_segment["duration"];

                if (json_segment.containsKey("0")) {
                    JsonObject object = json_segment["0"];
                    segment->animation[0] = Animation{object["s"], object["e"], duration};
                }

                if (json_segment.containsKey("1")) {
                    JsonObject object = json_segment["1"];
                    segment->animation[1] = Animation{object["s"], object["e"], duration};
                }

                if (json_segment.containsKey("start_time")) {
                    segment->start_time = json_segment["start_time"];
                }
                if (json_segment.containsKey("keepalive")) {
                    segment->keepalive = (int)json_segment["keepalive"];
                }

                if (json_segment.containsKey("hue_grad")) {
                    auto hue = json_segment["hue_grad"];
                    segment->hue_gradient[0] = hue["s"];
                    segment->hue_gradient[1] = hue["e"];
                } else if (json_segment.containsKey("hue_anim")) {
                    auto hue = json_segment["hue_anim"];
                    segment->hue_animation.reset(new Animation(hue["s"], hue["e"], duration));
                } else if (json_segment.containsKey("hue")) {
                    auto hue = json_segment["hue"];
                    segment->hue_animation.reset(new Animation(hue, hue, duration));
                } else {
                    segment->hue_animation.reset(new Animation(0, 0, duration));
                }

                if (json_segment.containsKey("sat_anim")) {
                    auto saturation = json_segment["sat_anim"];
                    segment->saturation_animation.reset(
                        new Animation(saturation["s"], saturation["e"], duration));
                }

                segment->end_time = segment->start_time + duration;
                segments.push_back(std::move(segment));
            }

            program->SetSegments(std::move(segments));
        }

        if (doc.containsKey("brightness")) {
            auto sequence = std::unique_ptr<AnimationSequence>(new AnimationSequence());

            JsonArray brightness_array = doc["brightness"];

            for (uint32_t i = 0; i < brightness_array.size(); i++) {
                JsonObject brightness = brightness_array[i];
                sequence->Add(Animation{brightness["s"], brightness["e"], brightness["d"]});
            }

            program->SetBrightness(std::move(sequence));
        }

        return std::move(program);
    }

    return DRETP(nullptr, "unknown program: %s", name);
}
