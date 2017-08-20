#pragma once

#include <memory>

class HttpServer {
public:
    virtual ~HttpServer() {}
    virtual void Loop() = 0;

    static std::unique_ptr<HttpServer> Create();
};
