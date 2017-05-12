#pragma once

#include <Config.h>
#include <vrm/usrmgt/IUserCredential.h>

#include <string>
#include <gmock/gmock.h>

class DLLEXPORT MockUserCredential : public vrm::usermanagement::IUserCredential
{
public:
    MOCK_CONST_METHOD0(getType, vrm::usermanagement::CredentialType());
    MOCK_CONST_METHOD1(getCredentialValue, const char*(vrm::usermanagement::CredentialValueType));
    MOCK_CONST_METHOD2(getCredentialValues, void(const char* [], uint32_t&));
};