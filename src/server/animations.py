ANIMATIONS = {
    'blackout': {
        "name": "AnimatedProgram",
        "segments": [
            {
                "duration": "10",
                "0": {"s": "0", "e": "0"},
                "1": {"s": "53", "e": "53"}
            }
        ],
        "brightness": [
            {"s": "0", "e": "0", "d": "10"}
        ]
    },
    'cool': {
        "name": "AnimatedProgram",
        "segments": [
            {
                "duration": "1000",
                "keepalive": "1",
                "0": { "s": "0", "e": "0" },
                "1": { "s": "0", "e": "17" },
                "hue_grad": { "s": "240", "e": "160" }
            },
            {
                "duration": "1000",
                "keepalive": "1",
                "0": { "s": "53", "e": "53" },
                "1": { "s": "53", "e": "36" },
                "hue_grad": { "s": "160", "e": "240" }
            },
            {
                "start_time": "1000",
                "duration": "1000",
                "0": { "s": "18", "e": "35" },
                "1": { "s": "18", "e": "35" },
                "hue_anim": { "s": "160", "e": "240" },
            },
            {
                "start_time": "1000",
                "duration": "1000",
                "0": { "s": "35", "e": "18" },
                "1": { "s": "35", "e": "18" },
                "hue_anim": { "s": "160", "e": "240" },
            }
        ],
        "brightness": [
            { "s": "0", "e": "255", "d": "1000" },
            { "s": "255", "e": "255", "d": "1000" },
            { "s": "255", "e": "0", "d": "500" }
        ]
    },
    'swirl_pulse': {
        "name": "AnimatedProgram",
        "segments": [
            {
                "duration": "1000",
                "0": {"s": "0", "e": "0"},
                "1": {"s": "53", "e": "53"},
                "hue_anim": {"s": "5", "e": "0"}
            }
        ],
        "brightness": [
            {"s": "20", "e": "255", "d": "500"},
            {"s": "255", "e": "20", "d": "500"}
        ]
    },
    'swirl_steady': {
        "name": "AnimatedProgram",
        "segments": [
            {
                "keepalive": "1",
                "duration": "1000",
                "0": {"s": "0", "e": "0"},
                "1": {"s": "53", "e": "53"},
                "hue_anim": {"s": "0", "e": "0"}
            }
        ],
        "brightness": [
            {"s": "20", "e": "20", "d": "1000"}
        ]
    },
    'swirl_pulse_slow': {
        "name": "AnimatedProgram",
        "segments": [
            {
                "keepalive": "1",
                "duration": "1000",
                "0": {"s": "0", "e": "0"},
                "1": {"s": "53", "e": "53"},
                "hue_anim": {"s": "0", "e": "0"}
            }
        ],
        "brightness": [
            {"s": "20", "e": "255", "d": "500"},
            {"s": "255", "e": "0", "d": "500"},
            {"s": "0", "e": "255", "d": "500"},
            {"s": "255", "e": "20", "d": "500"}
        ]
    },
    'bounce': {
    "name": "AnimatedProgram",
        "segments": [
            {
                "start_time": "250",
                "duration": "400",
                "0": {"s": "18", "e": "35"},
                "1": {"s": "35", "e": "35"},
                "hue_anim": {"s": "90", "e": "90"}
            },
            {
                "duration": "1000",
                "0": { "s": "0", "e": "0" },
                "1": { "s": "0", "e": "17" },
                "hue_grad": { "s": "100", "e": "160" }
            },
            {
                "duration": "1000",
                "0": { "s": "53", "e": "53" },
                "1": { "s": "53", "e": "36" },
                "hue_grad": { "s": "160", "e": "100" }
            },
            {
                "start_time": "1000",
                "duration": "500",
                "0": {"s": "17", "e": "17"},
                "1": {"s": "17", "e": "0"},
                "hue_anim": {"s": "0", "e": "100"}
            },
            {
                "start_time": "1000",
                "duration": "500",
                "0": {"s": "36", "e": "36"},
                "1": {"s": "36", "e": "53"},
                "hue_anim": {"s": "0", "e": "100"}
            }
        ]
    },
    'simon': {
        "name": "AnimatedProgram",
        "segments": [
            {
                "duration": "1000",
                "0": {"s": "0", "e": "0"},
                "1": {"s": "53", "e": "53"},
                "hue_anim": {"s": "200", "e": "120"}
            }
        ],
        "brightness": [
            {"s": "0", "e": "255", "d": "300"},
            {"s": "255", "e": "0", "d": "300"}
        ]
    },
    'game_over': {
        "name": "AnimatedProgram",
        "segments": [
            {
                "duration": "1300",
                "0": {"s": "0", "e": "0"},
                "1": {"s": "53", "e": "53"},
                "hue_anim": {"s": "0", "e": "0"}
            }
        ],
        "brightness": [
            {"s": "0", "e": "255", "d": "300"},
            {"s": "255", "e": "0", "d": "1000"}
        ]
    }
}
