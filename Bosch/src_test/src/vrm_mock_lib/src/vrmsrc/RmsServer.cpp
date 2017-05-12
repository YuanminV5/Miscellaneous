#include <vrmsrc/rcpserver/RmsServer.h>

RMSServer* RMSServer::s_instance = nullptr;
MockRmsServer* RMSServer::s_mock = nullptr;

RMSServer::RMSServer()
{
}

RMSServer* RMSServer::instance()
{
    if (!s_instance) {
        s_instance = new RMSServer();
    }
    return s_instance;
}

std::string RMSServer::getDeviceReplayPassword()
{
    return s_mock->getDeviceReplayPassword();
}

AccessManagement* RMSServer::getAccessManagement()
{
    return s_mock->getAccessManagement();
}