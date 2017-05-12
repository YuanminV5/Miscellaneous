#pragma once

#include <Config.h>
#include <rcpp/server.h>
#include <vrmsrc/util/UserId.h>
#include <vrm/usrmgt/IAuthenticationInfo.h>
#include <memory>
#include <gmock/gmock.h>

class DLLEXPORT RMSSession
{
public:
    MOCK_CONST_METHOD0(getSource, UserId());
    MOCK_CONST_METHOD0(getRef, rcpplus::RcpSessionRef());
    MOCK_METHOD2(setLogin, void(const UserId&, const std::shared_ptr<vrm::usermanagement::IAuthenticationInfo>&));
};
