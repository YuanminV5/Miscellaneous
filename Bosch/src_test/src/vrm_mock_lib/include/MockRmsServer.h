#pragma once

#include <Config.h>
#include <string>
#include <gmock/gmock.h>
#include <vrmsrc/config/AccessManagement.h>


class DLLEXPORT MockRmsServer
{
public:
    MOCK_CONST_METHOD0(getDeviceReplayPassword, std::string());
    MOCK_CONST_METHOD0(getAccessManagement, AccessManagement*());
};