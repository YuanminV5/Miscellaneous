#pragma once

#include <Config.h>
#include <rcpp/server.h>
#include <vrm/usrmgt/IAuthenticationInfo.h>
#include <string>
#include <memory>
#include <gmock/gmock.h>

class DLLEXPORT AccessManagement
{
public:
    MOCK_METHOD2(validateToken, std::shared_ptr<vrm::usermanagement::IAuthenticationInfo>(const std::string&, const rcpplus::RcpSessionRef&));
};

