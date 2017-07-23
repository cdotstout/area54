#pragma once

#include "program.h"

class Parser {
public:
    static std::unique_ptr<Program> ParseProgram(const char* json);
};
