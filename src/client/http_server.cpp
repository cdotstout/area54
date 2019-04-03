#if USE_HTTP_SERVER

#include "http_server.h"

#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServer.h>

class EspHttpServer : public HttpServer {
public:
    EspHttpServer();

    void Loop() override;

private:
    ESP8266WebServer http_server_;
    ESP8266HTTPUpdateServer http_update_server_;
};

EspHttpServer::EspHttpServer()
{
    http_update_server_.setup(&http_server_);
    http_server_.begin();
}

void EspHttpServer::Loop() { http_server_.handleClient(); }

std::unique_ptr<HttpServer> HttpServer::Create()
{
    return std::unique_ptr<HttpServer>(new EspHttpServer);
}

#endif // USE_HTTP_SERVER
