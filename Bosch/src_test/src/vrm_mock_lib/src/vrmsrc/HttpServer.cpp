#include <vrmsrc/httpserver/HttpServer.h>


HttpServer* HttpServer::s_instance = nullptr;
MockHttpServer* HttpServer::s_mock = nullptr;

HttpServer::HttpServer()
{
}

HttpServer* HttpServer::instance()
{
    if (!s_instance) {
        s_instance = new HttpServer();
    }
    return s_instance;
}

SessionDataRef HttpServer::getSessionData(const std::string& key, bool triggerKeepAlive)
{
    return s_mock->getSessionData(key, triggerKeepAlive);
}