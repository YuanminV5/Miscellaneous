#pragma once

#include <Config.h>
#include <vrm/usrmgt/IAuthObject.h>

#include <string>
#include <gmock/gmock.h>

class DLLEXPORT MockAuthObject : public vrm::usermanagement::IAuthObject
{
public:
    MOCK_CONST_METHOD0(getType, vrm::usermanagement::AuthObjectType());
    MOCK_CONST_METHOD1(getValue, const char*(vrm::usermanagement::AuthObjectKey));
};