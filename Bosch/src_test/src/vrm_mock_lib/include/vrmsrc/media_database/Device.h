#pragma once

#include <Config.h>
#include <vcslib/Guid.h>
#include <string>
#include <gmock/gmock.h>

class DLLEXPORT Device
{
public:
    MOCK_METHOD0(getConnectUrl, std::string());
    MOCK_METHOD0(getGuid, Guid&());
};