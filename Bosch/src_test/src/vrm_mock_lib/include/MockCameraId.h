#pragma once

#include <Config.h>
#include <gmock/gmock.h>

class DLLEXPORT MockCameraId
{
public:
    MOCK_METHOD0(getLine, unsigned short());
};