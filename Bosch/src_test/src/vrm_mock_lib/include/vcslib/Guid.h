#pragma once

#include <Config.h>
#include <string>
#include <MockGuid.h>

class DLLEXPORT Guid
{
public:
    std::string toString();

    static MockGuid* s_mock;
};