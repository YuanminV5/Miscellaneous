#pragma once

#include <Config.h>
#include <vcslib/HostAddress.h>

#include <string>
#include <gmock/gmock.h>

class DLLEXPORT MockUserId
{
public:
    MOCK_METHOD2(init, void(const vcslib::HostAddress&, const std::string&));
    MOCK_CONST_METHOD0(getUser, std::string&());
    MOCK_CONST_METHOD0(getHostAddress, const vcslib::HostAddress&());
};
