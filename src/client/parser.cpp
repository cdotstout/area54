#include "parser.h"

#include "animated_program.h"
#include "area54.pb.h"
#include "log.h"
#include "pb_decode.h"

class DeviceList {
public:
    using String = std::array<char, 18 + 1>;

    const String& get(uint32_t index) { return devices_[index]; }

    size_t size() { return devices_.size(); }

    static bool decode(pb_istream_t* stream, const pb_field_t* field, void** arg)
    {
        String device;
        if (!pb_read(stream, reinterpret_cast<pb_byte_t*>(device.data()), stream->bytes_left))
            return DRETF(false, "Decoding device failed: %s\n", PB_GET_ERROR(stream));

        device[device.size() - 1] = 0;
        LOG("device %s", device.data());

        auto device_list = reinterpret_cast<DeviceList*>(*arg);
        device_list->devices_.emplace_back(std::move(device));

        return true;
    }

    std::vector<String> devices_;
};

class SegmentList {
public:
    static bool decode(pb_istream_t* stream, const pb_field_t* field, void** arg)
    {
        SegmentMsg segment_msg = SegmentMsg_init_default;

        int status = pb_decode(stream, SegmentMsg_fields, &segment_msg);
        if (!status)
            return DRETF(false, "Decoding segment failed: %s\n", PB_GET_ERROR(stream));

        LOG("segment start_time %u duration %u keep_alive %u {%u,%u} {%u,%u} "
            "hue_type %u hue_start %u hue_stop %u",
            segment_msg.start_time, segment_msg.duration, segment_msg.keep_alive,
            segment_msg.begin.start_led, segment_msg.begin.stop_led, segment_msg.end.start_led,
            segment_msg.end.stop_led, segment_msg.hue_type, segment_msg.hue_start,
            segment_msg.hue_stop);

        auto segment = std::unique_ptr<AnimatedProgram::Segment>(new AnimatedProgram::Segment);

        segment->start_time = segment_msg.start_time;
        segment->end_time = segment->start_time + segment_msg.duration;
        segment->keepalive = segment_msg.keep_alive;

        segment->animation[0] = Animation{segment_msg.begin.start_led, segment_msg.begin.stop_led,
                                          segment_msg.duration};
        segment->animation[1] =
            Animation{segment_msg.end.start_led, segment_msg.end.stop_led, segment_msg.duration};

        switch (segment_msg.hue_type) {
            case SegmentMsg_HueType_NONE:
                segment->hue_gradient[0] = 0;
                segment->hue_gradient[1] = 0;
                break;
            case SegmentMsg_HueType_SINGLE:
                // Use gradient here instead?
                segment->hue_animation.reset(new Animation(
                    segment_msg.hue_start, segment_msg.hue_start, segment_msg.duration));
                break;
            case SegmentMsg_HueType_ANIMATION:
                segment->hue_animation.reset(new Animation(
                    segment_msg.hue_start, segment_msg.hue_stop, segment_msg.duration));
                break;
            case SegmentMsg_HueType_GRADIENT:
                segment->hue_gradient[0] = segment_msg.hue_start;
                segment->hue_gradient[1] = segment_msg.hue_stop;
                break;
        }

        auto list = reinterpret_cast<SegmentList*>(*arg);
        list->segments_.emplace_back(std::move(segment));

        return true;
    }

    void TakeSegments(std::vector<std::unique_ptr<AnimatedProgram::Segment>>* segments_out)
    {
        *segments_out = std::move(segments_);
    }

private:
    std::vector<std::unique_ptr<AnimatedProgram::Segment>> segments_;
};

class BrightnessStepList {
public:
    static bool decode(pb_istream_t* stream, const pb_field_t* field, void** arg)
    {
        BrightnessStepMsg bstep_msg = BrightnessStepMsg_init_default;

        int status = pb_decode(stream, BrightnessStepMsg_fields, &bstep_msg);
        if (!status)
            return DRETF(false, "Decoding bstep failed: %s\n", PB_GET_ERROR(stream));

        LOG("bstep start %u stop %u duration %u", bstep_msg.start, bstep_msg.stop,
            bstep_msg.duration);

        auto list = reinterpret_cast<BrightnessStepList*>(*arg);
        if (!list->sequence_) {
            list->sequence_ = std::unique_ptr<AnimationSequence>(new AnimationSequence());
        }
        list->sequence_->Add(Animation{bstep_msg.start, bstep_msg.stop, bstep_msg.duration});

        return true;
    }

    void TakeSequence(std::unique_ptr<AnimationSequence>* sequence_out)
    {
        *sequence_out = std::move(sequence_);
    }

private:
    std::unique_ptr<AnimationSequence> sequence_;
};

std::unique_ptr<Program> Parser::ParseProgram(const char* this_device, uint8_t* buffer,
                                              uint32_t length)
{
    LOG("ParseProgram buffer %p length %u", buffer, length);

    AnimationMsg animation = AnimationMsg_init_default;

    DeviceList device_list;
    animation.device.funcs.decode = DeviceList::decode;
    animation.device.arg = &device_list;

    SegmentList segment_list;
    animation.segments.funcs.decode = SegmentList::decode;
    animation.segments.arg = &segment_list;

    BrightnessStepList bstep_list;
    animation.brightness_steps.funcs.decode = BrightnessStepList::decode;
    animation.brightness_steps.arg = &bstep_list;

    pb_istream_t stream = pb_istream_from_buffer(buffer, length);

    int status = pb_decode(&stream, AnimationMsg_fields, &animation);
    if (!status)
        return DRETP(nullptr, "Decoding failed: %s\n", PB_GET_ERROR(&stream));

    LOG("decode success: device count %d", device_list.size());

    bool should_parse = false;
    for (uint32_t i = 0; i < device_list.size(); i++) {
        auto device = device_list.get(i);
        LOG("device[%d] %s this_device %s", i, device.data(), this_device);
        if (strcasecmp(device.data(), this_device) == 0) {
            should_parse = true;
            break;
        }
    }

    if (!should_parse)
        return nullptr;

    animation.name[sizeof(animation.name) - 1] = 0;
    LOG("name %s\n", animation.name);

    if (strcmp(animation.name, "AnimatedProgram") == 0) {
        auto program = std::unique_ptr<AnimatedProgram>(new AnimatedProgram());

        std::vector<std::unique_ptr<AnimatedProgram::Segment>> segments;
        segment_list.TakeSegments(&segments);
        program->SetSegments(std::move(segments));

        std::unique_ptr<AnimationSequence> sequence;
        bstep_list.TakeSequence(&sequence);
        program->SetBrightness(std::move(sequence));

        return std::move(program);
    }

    return DRETP(nullptr, "unknown program: %s", animation.name);
}
