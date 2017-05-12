#pragma once

#include <Config.h>
#include <vrmsrc/util/UserId.h>
#include <vrm/usrmgt/IAuthenticationInfo.h>
#include <memory>
#include <string>
#include <gmock/gmock.h>

class DLLEXPORT SessionData
{
public:
	MOCK_CONST_METHOD0(getSessionKey, const std::string&());
	MOCK_CONST_METHOD0(getUserId, const UserId&());
	MOCK_CONST_METHOD0(getAuthInfo, const std::shared_ptr<vrm::usermanagement::IAuthenticationInfo>&());
};

typedef std::shared_ptr<SessionData> SessionDataRef;