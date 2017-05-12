#pragma once

#include <Config.h>
#include <string>
#include <vrmsrc/httpserver/SessionList.h>
#include <gmock/gmock.h>

class DLLEXPORT MockHttpServer
{
public:
    MOCK_METHOD2(getSessionData, SessionDataRef(const std::string&, bool));
};