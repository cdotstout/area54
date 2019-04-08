#pragma once

#include "program.h"

class Parser {
public:
    static std::unique_ptr<Program> ParseProgram(const char* this_device, uint8_t* buffer, uint32_t length);
};
