#pragma once

#include <Config.h>

#include <string>
#include <gmock/gmock.h>

namespace vcslib
{
    class DLLEXPORT HostAddress
    {
    public:
        MOCK_CONST_METHOD0(toString, std::string());
    };
}