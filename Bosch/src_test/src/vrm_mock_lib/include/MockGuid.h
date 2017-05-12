#pragma once

#include <Config.h>

#include <string>
#include <gmock/gmock.h>

/** May not necessary coz used for return as reference. */
class DLLEXPORT MockGuid
{
public:
    MOCK_METHOD0(toString, std::string());
};
