#pragma once

#include <Config.h>
#include <MockCameraId.h>

class DLLEXPORT CameraId
{
public:
    unsigned short getLine() const;
    static MockCameraId* s_mock;
};

