#pragma once

#include <Config.h>
#include <vrm/usrmgt/ILoginRequest.h>

#include <gmock/gmock.h>

class DLLEXPORT MockLoginRequest : public vrm::usermanagement::ILoginRequest
{
public:
    MOCK_CONST_METHOD1(getCredential, const vrm::usermanagement::IUserCredential*(size_t));
    MOCK_CONST_METHOD0(getCredentialCount, size_t());
};