#pragma once

#include <Config.h>
#include <vrmsrc/httpserver/SessionList.h>

#include <string>
#include <MockHttpServer.h>

class DLLEXPORT HttpServer
{
public:
    static HttpServer* instance();
    SessionDataRef getSessionData(const std::string& key, bool triggerKeepAlive);

private:
    HttpServer();

    static HttpServer* s_instance;
    static MockHttpServer* s_mock;
};