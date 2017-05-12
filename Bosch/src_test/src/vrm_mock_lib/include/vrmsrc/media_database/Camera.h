#pragma once

#include <Config.h>
#include <vrmsrc/media_database/Device.h>
#include <vrmsrc/util/CameraId.h>

#include <string>
#include <gmock/gmock.h>

class DLLEXPORT Camera
{
public:
    MOCK_CONST_METHOD0(getDevice, Device*());
    MOCK_CONST_METHOD0(getTrackId, unsigned short());
    MOCK_CONST_METHOD0(getAddress, CameraId&());
};