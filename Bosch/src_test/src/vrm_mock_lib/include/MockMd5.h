#pragma once

#include <Config.h>

#include <string>
#include <gmock/gmock.h>

/** May not necessary coz used for return as reference. */
class DLLEXPORT MockMd5
{
public:
    MOCK_METHOD0(digest, const char*());
};
