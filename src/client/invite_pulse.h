const char* kInvitePulse = R"(
    {
        "name": "AnimatedProgram",
        "segments": [
            {
                "duration": "3000",
                "keepalive": "1",
                "0": { "s": "0", "e": "0" },
                "1": { "s": "53", "e": "53" },
                "hue_grad": { "s": "150", "e": "150" }
            }
        ],
        "brightness": [
            { "s": "0", "e": "64", "d": "1000" },
            { "s": "64", "e": "0", "d": "1000" },
            { "s": "0", "e": "0", "d": "2000" }
        ]
    })";
