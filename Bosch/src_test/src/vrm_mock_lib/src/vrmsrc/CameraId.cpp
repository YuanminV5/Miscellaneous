#include <vrmsrc/util/CameraId.h>

MockCameraId* CameraId::s_mock = nullptr;

unsigned short CameraId::getLine() const
{
    return s_mock->getLine();
}