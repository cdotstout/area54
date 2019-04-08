import area54_pb2

class AnimationGenerator:

    @staticmethod
    def Get(devices, animation):
        msg = area54_pb2.AnimationMsg()
        for device in devices:
        	msg.device.append(device)
        msg.name = "AnimatedProgram"
        if animation == "pulse":
        	segment = msg.segments.add()
        	segment.duration = 1500;
        	segment.begin.start_led = 0;
        	segment.begin.stop_led = 0;
        	segment.end.start_led = 53;
        	segment.end.stop_led = 53;
        	segment.hue_type = segment.SINGLE;
        	segment.hue_start = 100;
        	brightness_step = msg.brightness_steps.add()
        	brightness_step.start = 0
        	brightness_step.stop = 255
        	brightness_step.duration = 750
        	brightness_step = msg.brightness_steps.add()
        	brightness_step.start = 255
        	brightness_step.stop = 0
        	brightness_step.duration = 500

        if animation == "cool":
        	segment = msg.segments.add()
        	segment.duration = 1000;
        	segment.keep_alive = 1;
        	segment.begin.start_led = 0;
        	segment.begin.stop_led = 0;
        	segment.end.start_led = 0;
        	segment.end.stop_led = 17;
        	segment.hue_type = segment.GRADIENT;
        	segment.hue_start = 240;
        	segment.hue_stop = 160;
        	segment = msg.segments.add()
        	segment.duration = 1000;
        	segment.keep_alive = 1;
        	segment.begin.start_led = 53;
        	segment.begin.stop_led = 53;
        	segment.end.start_led = 53;
        	segment.end.stop_led = 36;
        	segment.hue_type = segment.GRADIENT;
        	segment.hue_start = 160;
        	segment.hue_stop = 240;
        	segment = msg.segments.add()
        	segment.start_time = 1000;
        	segment.duration = 1000;
        	segment.begin.start_led = 18;
        	segment.begin.stop_led = 35;
        	segment.end.start_led = 18;
        	segment.end.stop_led = 35;
        	segment.hue_type = segment.ANIMATION;
        	segment.hue_start = 160;
        	segment.hue_stop = 240;
        	segment = msg.segments.add()
        	segment.start_time = 1000;
        	segment.duration = 1000;
        	segment.begin.start_led = 35;
        	segment.begin.stop_led = 18;
        	segment.end.start_led = 35;
        	segment.end.stop_led = 18;
        	segment.hue_type = segment.ANIMATION;
        	segment.hue_start = 160;
        	segment.hue_stop = 240;
        	brightness_step = msg.brightness_steps.add()
        	brightness_step.start = 0
        	brightness_step.stop = 255
        	brightness_step.duration = 1000
        	brightness_step = msg.brightness_steps.add()
        	brightness_step.start = 255
        	brightness_step.stop = 255
        	brightness_step.duration = 1000
        	brightness_step = msg.brightness_steps.add()
        	brightness_step.start = 255
        	brightness_step.stop = 0
        	brightness_step.duration = 500
        
    
        if animation == "chase1" or animation == "chase2":
        	duration = 3000
        	if (animation == "chase1"):
        		c1 = 0
        		c2 = 10
        	if (animation == "chase2"):
        		c1 = 10
        		c2 = 0
        	# left-center
        	hue_start = c1
        	hue_stop = c1
        	segment = msg.segments.add()
        	segment.duration = (int)(duration * 7/18);
        	segment.begin.start_led = 53;
        	segment.begin.stop_led = 26;
        	segment.end.start_led = 53;
        	segment.end.stop_led = 26;
        	segment.hue_type = segment.ANIMATION;
        	segment.hue_start = hue_start
        	segment.hue_stop = hue_stop
        	segment = msg.segments.add()
        	segment.start_time = (int)(duration * 1 / 9)
        	segment.duration = (int)(duration * 7/18);
        	segment.begin.start_led = 53;
        	segment.begin.stop_led = 26;
        	segment.end.start_led = 53;
        	segment.end.stop_led = 26;
        	segment.hue_type = segment.ANIMATION;
        	segment.hue_start = hue_start
        	segment.hue_stop = hue_stop
        	segment = msg.segments.add()
        	segment.start_time = (int)(duration * 2/9)
        	segment.duration = (int)(duration * 7/18)
        	segment.begin.start_led = 53;
        	segment.begin.stop_led = 26;
        	segment.end.start_led = 53;
        	segment.end.stop_led = 26;
        	segment.hue_type = segment.ANIMATION;
        	segment.hue_start = hue_start
        	segment.hue_stop = hue_stop
        	# center-right
        	hue_start = c2
        	hue_stop = c2
        	segment = msg.segments.add()
        	segment.start_time = (int)(duration * 7/18)
        	segment.duration = (int)(duration * 7/18)
        	segment.begin.start_led = 26;
        	segment.begin.stop_led = 0;
        	segment.end.start_led = 26;
        	segment.end.stop_led = 0;
        	segment.hue_type = segment.ANIMATION;
        	segment.hue_start = hue_start
        	segment.hue_stop = hue_stop
        	segment = msg.segments.add()
        	segment.start_time = (int)(duration * 9/18)
        	segment.duration = (int)(duration * 7/18);
        	segment.begin.start_led = 26;
        	segment.begin.stop_led = 0;
        	segment.end.start_led = 26;
        	segment.end.stop_led = 0;
        	segment.hue_type = segment.ANIMATION;
        	segment.hue_start = hue_start
        	segment.hue_stop = hue_stop
        	segment = msg.segments.add()
        	segment.start_time = (int)(duration * 11/18)
        	segment.duration = (int)(duration * 7/18)
        	segment.begin.start_led = 26;
        	segment.begin.stop_led = 0;
        	segment.end.start_led = 26;
        	segment.end.stop_led = 0;
        	segment.hue_type = segment.ANIMATION;
        	segment.hue_start = hue_start
        	segment.hue_stop = hue_stop

        	# right-center
        	hue_start = c1
        	hue_stop = c1
        	segment = msg.segments.add()
        	segment.duration = (int)(duration * 7/18);
        	segment.begin.start_led = 0;
        	segment.begin.stop_led = 26;
        	segment.end.start_led = 0;
        	segment.end.stop_led = 26;
        	segment.hue_type = segment.ANIMATION;
        	segment.hue_start = hue_start
        	segment.hue_stop = hue_stop
        	segment = msg.segments.add()
        	segment.start_time = (int)(duration * 1 / 9)
        	segment.duration = (int)(duration * 7/18);
        	segment.begin.start_led = 0;
        	segment.begin.stop_led = 26;
        	segment.end.start_led = 0;
        	segment.end.stop_led = 26;
        	segment.hue_type = segment.ANIMATION;
        	segment.hue_start = hue_start
        	segment.hue_stop = hue_stop
        	segment = msg.segments.add()
        	segment.start_time = (int)(duration * 2/9)
        	segment.duration = (int)(duration * 7/18)
        	segment.begin.start_led = 0;
        	segment.begin.stop_led = 26;
        	segment.end.start_led = 0;
        	segment.end.stop_led = 26;
        	segment.hue_type = segment.ANIMATION;
        	segment.hue_start = hue_start
        	segment.hue_stop = hue_stop
        	# center-left
        	hue_start = c2
        	hue_stop = c2
        	segment = msg.segments.add()
        	segment.start_time = (int)(duration * 7/18)
        	segment.duration = (int)(duration * 7/18)
        	segment.begin.start_led = 26;
        	segment.begin.stop_led = 53;
        	segment.end.start_led = 26;
        	segment.end.stop_led = 53;
        	segment.hue_type = segment.ANIMATION;
        	segment.hue_start = hue_start
        	segment.hue_stop = hue_stop
        	segment = msg.segments.add()
        	segment.start_time = (int)(duration * 9/18)
        	segment.duration = (int)(duration * 7/18);
        	segment.begin.start_led = 26;
        	segment.begin.stop_led = 53;
        	segment.end.start_led = 26;
        	segment.end.stop_led = 53;
        	segment.hue_type = segment.ANIMATION;
        	segment.hue_start = hue_start
        	segment.hue_stop = hue_stop
        	segment = msg.segments.add()
        	segment.start_time = (int)(duration * 11/18)
        	segment.duration = (int)(duration * 7/18)
        	segment.begin.start_led = 26;
        	segment.begin.stop_led = 53;
        	segment.end.start_led = 26;
        	segment.end.stop_led = 53;
        	segment.hue_type = segment.ANIMATION;
        	segment.hue_start = hue_start
        	segment.hue_stop = hue_stop

			# center piece
        	segment = msg.segments.add()
        	segment.keep_alive = 1
        	segment.start_time = (int)(duration * 7 / 18)
        	segment.duration = (int)(duration * 6 / 18)
        	segment.begin.start_led = 26
        	segment.begin.stop_led = 35
        	segment.end.start_led = 26
        	segment.end.stop_led = 18
        	segment.hue_type = segment.ANIMATION
        	segment.hue_start = c1
        	segment.hue_stop = c2

        	brightness_step = msg.brightness_steps.add()
        	brightness_step.start = 255
        	brightness_step.stop = 255
        	brightness_step.duration = (int)(duration * 9 / 10)
        	brightness_step = msg.brightness_steps.add()
        	brightness_step.start = 255
        	brightness_step.stop = 0
        	brightness_step.duration = (int)(duration * 1 / 10)

        if animation == 'corner':
        	duration = 400
        	segment = msg.segments.add();
        	segment.duration = duration
        	segment.begin.start_led = 18
        	segment.begin.stop_led = 18
        	segment.end.start_led = 18
        	segment.end.stop_led = 26
        	segment.hue_type = segment.SINGLE
        	segment.hue_start = 240
        	segment = msg.segments.add();
        	segment.start_time = duration
        	segment.duration = duration
        	segment.begin.start_led = 18
        	segment.begin.stop_led = 18
        	segment.end.start_led = 26
        	segment.end.stop_led = 26
        	segment.hue_type = segment.SINGLE
        	segment.hue_start = 240
        	segment = msg.segments.add();
        	segment.start_time = duration * 2
        	segment.duration = duration
        	segment.begin.start_led = 18
        	segment.begin.stop_led = 18
        	segment.end.start_led = 26
        	segment.end.stop_led = 18
        	segment.hue_type = segment.SINGLE
        	segment.hue_start = 240

        	segment = msg.segments.add();
        	segment.duration = duration
        	segment.begin.start_led = 17
        	segment.begin.stop_led = 17
        	segment.end.start_led = 17
        	segment.end.stop_led = 9
        	segment.hue_type = segment.SINGLE
        	segment.hue_start = 240
        	segment = msg.segments.add();
        	segment.start_time = duration
        	segment.duration = duration
        	segment.begin.start_led = 17
        	segment.begin.stop_led = 17
        	segment.end.start_led = 9
        	segment.end.stop_led = 9
        	segment.hue_type = segment.SINGLE
        	segment.hue_start = 240
        	segment = msg.segments.add();
        	segment.start_time = duration * 2
        	segment.duration = duration
        	segment.begin.start_led = 17
        	segment.begin.stop_led = 17
        	segment.end.start_led = 9
        	segment.end.stop_led = 17
        	segment.hue_type = segment.SINGLE
        	segment.hue_start = 240

        	# segment = msg.segments.add();
        	# segment.duration = duration
        	# segment.begin.start_led = 35
        	# segment.begin.stop_led = 0
        	# segment.end.start_led = 35
        	# segment.end.stop_led = 0
        	# segment.hue_type = segment.SINGLE
        	# segment.hue_start = 240
        	# segment = msg.segments.add();
        	# segment.duration = (int)(duration / 3)
        	# segment.begin.start_led = 35
        	# segment.begin.stop_led = 53
        	# segment.end.start_led = 35
        	# segment.end.stop_led = 53
        	# segment.hue_type = segment.SINGLE
        	# segment.hue_start = 240


        return msg.SerializeToString()

# ANIMATIONS = {
#     'blackout': {
#         "name": "AnimatedProgram",
#         "segments": [
#             {
#                 "duration": "10",
#                 "0": {"s": "0", "e": "0"},
#                 "1": {"s": "53", "e": "53"}
#             }
#         ],
#         "brightness": [
#             {"s": "0", "e": "0", "d": "10"}
#         ]
#     },
#     'tbd': {
#         "name": "AnimatedProgram",
#         "segments": [
#             {
#                 "d": "500",
#                 "keepalive": "1",
#                 "0": { "s": "0", "e": "8" },
#                 "1": { "s": "0", "e": "8" },
#             },
#             {
#                 "d": "500",
#                 "keepalive": "1",
#                 "0": { "s": "8", "e": "17" },
#                 "1": { "s": "8", "e": "17" },
#             },
#             {
#                 "d": "500",
#                 "keepalive": "1",
#                 "0": { "s": "8", "e": "0" },
#                 "1": { "s": "8", "e": "0" },
#             },
#             {
#                 "d": "500",
#                 "keepalive": "1",
#                 "0": { "s": "17", "e": "8" },
#                 "1": { "s": "17", "e": "8" },
#             },
#             {
#                 "d": "500",
#                 "keepalive": "1",
#                 "0": { "s": "18", "e": "26" },
#                 "1": { "s": "18", "e": "26" },
#             },
#             {
#                 "d": "500",
#                 "keepalive": "1",
#                 "0": { "s": "26", "e": "35" },
#                 "1": { "s": "26", "e": "35" },
#             },
#             {
#                 "d": "500",
#                 "keepalive": "1",
#                 "0": { "s": "26", "e": "18" },
#                 "1": { "s": "26", "e": "18" },
#             },
#             {
#                 "d": "500",
#                 "keepalive": "1",
#                 "0": { "s": "35", "e": "26" },
#                 "1": { "s": "35", "e": "26" },
#             },
#             {
#                 "d": "500",
#                 "keepalive": "1",
#                 "0": { "s": "36", "e": "43" },
#                 "1": { "s": "36", "e": "43" },
#             },
#             {
#                 "d": "500",
#                 "keepalive": "1",
#                 "0": { "s": "43", "e": "50" },
#                 "1": { "s": "43", "e": "50" },
#             }
#         ]
#     },
#     'cool': {
#         "name": "AnimatedProgram",
#         "segments": [
#             {
#                 "duration": "1000",
#                 "keepalive": "1",
#                 "0": { "s": "0", "e": "0" },
#                 "1": { "s": "0", "e": "17" },
#                 "hue_grad": { "s": "240", "e": "160" }
#             },
#             {
#                 "duration": "1000",
#                 "keepalive": "1",
#                 "0": { "s": "53", "e": "53" },
#                 "1": { "s": "53", "e": "36" },
#                 "hue_grad": { "s": "160", "e": "240" }
#             },
#             {
#                 "start_time": "1000",
#                 "duration": "1000",
#                 "0": { "s": "18", "e": "35" },
#                 "1": { "s": "18", "e": "35" },
#                 "hue_anim": { "s": "160", "e": "240" },
#             },
#             {
#                 "start_time": "1000",
#                 "duration": "1000",
#                 "0": { "s": "35", "e": "18" },
#                 "1": { "s": "35", "e": "18" },
#                 "hue_anim": { "s": "160", "e": "240" },
#             }
#         ],
#         "brightness": [
#             { "s": "0", "e": "255", "d": "1000" },
#             { "s": "255", "e": "255", "d": "1000" },
#             { "s": "255", "e": "0", "d": "500" }
#         ]
#     },
#     'swirl_pulse': {
#         "name": "AnimatedProgram",
#         "segments": [
#             {
#                 "duration": "1000",
#                 "0": {"s": "0", "e": "0"},
#                 "1": {"s": "53", "e": "53"},
#                 "hue_anim": {"s": "5", "e": "0"}
#             }
#         ],
#         "brightness": [
#             {"s": "20", "e": "255", "d": "500"},
#             {"s": "255", "e": "20", "d": "500"}
#         ]
#     },
#     'swirl_steady': {
#         "name": "AnimatedProgram",
#         "segments": [
#             {
#                 "keepalive": "1",
#                 "duration": "1000",
#                 "0": {"s": "0", "e": "0"},
#                 "1": {"s": "53", "e": "53"},
#                 "hue_anim": {"s": "0", "e": "0"}
#             }
#         ],
#         "brightness": [
#             {"s": "20", "e": "20", "d": "1000"}
#         ]
#     },
#     'swirl_pulse_slow': {
#         "name": "AnimatedProgram",
#         "segments": [
#             {
#                 "keepalive": "1",
#                 "duration": "1000",
#                 "0": {"s": "0", "e": "0"},
#                 "1": {"s": "53", "e": "53"},
#                 "hue_anim": {"s": "0", "e": "0"}
#             }
#         ],
#         "brightness": [
#             {"s": "20", "e": "255", "d": "500"},
#             {"s": "255", "e": "0", "d": "500"},
#             {"s": "0", "e": "255", "d": "500"},
#             {"s": "255", "e": "20", "d": "500"}
#         ]
#     },
#     'bounce': {
#     "name": "AnimatedProgram",
#         "segments": [
#             {
#                 "start_time": "250",
#                 "duration": "400",
#                 "0": {"s": "18", "e": "35"},
#                 "1": {"s": "35", "e": "35"},
#                 "hue_anim": {"s": "90", "e": "90"}
#             },
#             {
#                 "duration": "1000",
#                 "0": { "s": "0", "e": "0" },
#                 "1": { "s": "0", "e": "17" },
#                 "hue_grad": { "s": "100", "e": "160" }
#             },
#             {
#                 "duration": "1000",
#                 "0": { "s": "53", "e": "53" },
#                 "1": { "s": "53", "e": "36" },
#                 "hue_grad": { "s": "160", "e": "100" }
#             },
#             {
#                 "start_time": "1000",
#                 "duration": "500",
#                 "0": {"s": "17", "e": "17"},
#                 "1": {"s": "17", "e": "0"},
#                 "hue_anim": {"s": "0", "e": "100"}
#             },
#             {
#                 "start_time": "1000",
#                 "duration": "500",
#                 "0": {"s": "36", "e": "36"},
#                 "1": {"s": "36", "e": "53"},
#                 "hue_anim": {"s": "0", "e": "100"}
#             }
#         ]
#     },
#     'pulse': {
#         "name": "AnimatedProgram",
#         "segments": [
#             {
#                 "duration": "1500",
#                 "0": {"s": "0", "e": "0"},
#                 "1": {"s": "53", "e": "53"},
#                 "hue_anim": {"s": "200", "e": "200"}
#             }
#         ],
#         "brightness": [
#             {"s": "0", "e": "255", "d": "750"},
#             {"s": "255", "e": "0", "d": "500"}
#         ]
#     },
#     'simon': {
#         "name": "AnimatedProgram",
#         "segments": [
#             {
#                 "duration": "2000",
#                 "0": {"s": "0", "e": "0"},
#                 "1": {"s": "53", "e": "53"},
#                 "hue_anim": {"s": "200", "e": "200"}
#             }
#         ],
#         "brightness": [
#             {"s": "0", "e": "255", "d": "1000"},
#             {"s": "255", "e": "0", "d": "1000"}
#         ]
#     },
#     'game_over': {
#         "name": "AnimatedProgram",
#         "segments": [
#             {
#                 "duration": "1300",
#                 "0": {"s": "0", "e": "0"},
#                 "1": {"s": "53", "e": "53"},
#                 "hue_anim": {"s": "0", "e": "0"}
#             }
#         ],
#         "brightness": [
#             {"s": "0", "e": "255", "d": "300"},
#             {"s": "255", "e": "0", "d": "1000"}
#         ]
#     }
# }
