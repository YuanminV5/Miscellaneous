#pragma once

#include <Config.h>
#include <vrmsrc/config/AccessManagement.h>

#include <MockRmsServer.h>

class DLLEXPORT RMSServer
{
public:
    static RMSServer* instance();
    std::string getDeviceReplayPassword();
    AccessManagement* getAccessManagement();

private:
    RMSServer();
    static RMSServer* s_instance;
    static MockRmsServer* s_mock;
};