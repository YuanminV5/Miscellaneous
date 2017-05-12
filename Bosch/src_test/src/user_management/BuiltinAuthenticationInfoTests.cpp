#include <src/user_management/BuiltinAuthenticationInfo.h>

#include <CppUnitTest.h> 
#include <string>
#include <MockAuthObject.h>
#include <gmock/gmock.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using ::testing::_;
using ::testing::Return;
using ::testing::An;
using ::testing::DoAll;
using ::testing::SetArgReferee;

TEST_CLASS(BuiltinAuthenticationInfoTests)
{
public:
	TEST_CLASS_INITIALIZE(ClassInitialize)
	{
		// enable google mock
		::testing::GTEST_FLAG(throw_on_failure) = true;
		int argc = 0;
		char **argv = NULL;
		::testing::InitGoogleMock(&argc, argv);
	}

	TEST_CLASS_CLEANUP(ClassCleanup)
	{
	}

	TEST_METHOD(when_create_a_builtin_auth_object_with_default_then_auth_result_is_not_authenticated)
	{
		// constructor test

		// arrange
		// act
		BuiltinAuthenticationInfo builtinAuthInfo;

		// assert
		Assert::IsTrue(builtinAuthInfo.getAuthenticationStatus() == vrm::usermanagement::AuthStatus::NotAuthenticated);
	}

	TEST_METHOD(when_create_a_builtin_auth_object_with_type_RecordingDevice_then_auth_result_is_user_level)
	{
		// constructor test

		// arrange
		// act
		BuiltinAuthenticationInfo builtinAuthInfo(BuiltinAuthenticationInfo::Type::RecordingDevice, std::string());

		// assert
		Assert::IsTrue(builtinAuthInfo.getAuthenticationStatus() == vrm::usermanagement::AuthStatus::UserLevel);
	}

	TEST_METHOD(when_create_a_builtin_auth_object_with_type_ReplayDevice_then_auth_result_is_user_level)
	{
		// constructor test

		// arrange
		// act
		BuiltinAuthenticationInfo builtinAuthInfo(BuiltinAuthenticationInfo::Type::ReplayDevice, std::string());

		// assert
		Assert::IsTrue(builtinAuthInfo.getAuthenticationStatus() == vrm::usermanagement::AuthStatus::UserLevel);
	}

	TEST_METHOD(when_create_a_builtin_auth_object_with_type_CbsSocketKnocker_then_auth_result_is_administrator_level)
	{
		// constructor test

		// arrange
		// act
		BuiltinAuthenticationInfo builtinAuthInfo(BuiltinAuthenticationInfo::Type::CbsSocketKnocker, std::string());

		// assert
		Assert::IsTrue(builtinAuthInfo.getAuthenticationStatus() == vrm::usermanagement::AuthStatus::AdministratorLevel);
	}

	TEST_METHOD(when_check_perssmion_to_auth_object_if_type_is_CbsSocketKnocker_then_return_false)
	{
		// constructor test

		// arrange
		BuiltinAuthenticationInfo builtinAuthInfo(BuiltinAuthenticationInfo::Type::CbsSocketKnocker, std::string());
		MockAuthObject mockAuthObject;

		// act
		// assert
		Assert::IsTrue(builtinAuthInfo.hasPermissionToAuthObject(&mockAuthObject, vrm::usermanagement::AuthPermission::DeleteVideo));
	}

	TEST_METHOD(when_check_perssmion_to_auth_object_if_specified_permission_is_not_PlaybackVideo_or_ViewLiveVideo_then_return_false)
	{
		// constructor test

		// arrange
		BuiltinAuthenticationInfo builtinAuthInfo(BuiltinAuthenticationInfo::Type::RecordingDevice, std::string());
		MockAuthObject mockAuthObject;

		// act
		// assert
		Assert::IsFalse(builtinAuthInfo.hasPermissionToAuthObject(&mockAuthObject, vrm::usermanagement::AuthPermission::ExportVideo));
	}

	TEST_METHOD(when_check_perssmion_to_null_auth_object_then_return_false)
	{
		// constructor test

		// arrange
		BuiltinAuthenticationInfo builtinAuthInfo(BuiltinAuthenticationInfo::Type::RecordingDevice, std::string());

		// act
		// assert
		Assert::IsFalse(builtinAuthInfo.hasPermissionToAuthObject(nullptr, vrm::usermanagement::AuthPermission::ViewLiveVideo));
	}

	TEST_METHOD(when_check_perssmion_to_camera_auth_object_but_address_does_not_match_then_return_false)
	{
		// DefaultAuthenticationInfo::hasPermissionToAuthObject test

		// arrange
		const char* ptr = "authUrl";
		MockAuthObject mockAuthObject;

		EXPECT_CALL(mockAuthObject, getType())
			.WillOnce(Return(vrm::usermanagement::AuthObjectType::Camera));

		EXPECT_CALL(mockAuthObject, getValue(_))
			.WillRepeatedly(Return(ptr));


		BuiltinAuthenticationInfo builtinAuthInfo(BuiltinAuthenticationInfo::Type::RecordingDevice, std::string("storedUrl"));

		// act
		// assert
		Assert::IsFalse(builtinAuthInfo.hasPermissionToAuthObject(&mockAuthObject, vrm::usermanagement::AuthPermission::ViewLiveVideo));
	}

	TEST_METHOD(when_check_perssmion_to_camera_auth_object_and_address_matchs_then_return_true)
	{
		// DefaultAuthenticationInfo::hasPermissionToAuthObject test

		// arrange
		const char* ptr = "storedUrl";
		MockAuthObject mockAuthObject;

		EXPECT_CALL(mockAuthObject, getType())
			.WillOnce(Return(vrm::usermanagement::AuthObjectType::Camera));

		EXPECT_CALL(mockAuthObject, getValue(_))
			.WillRepeatedly(Return(ptr));

		BuiltinAuthenticationInfo builtinAuthInfo(BuiltinAuthenticationInfo::Type::RecordingDevice, std::string("storedUrl"));

		// act
		// assert
		Assert::IsTrue(builtinAuthInfo.hasPermissionToAuthObject(&mockAuthObject, vrm::usermanagement::AuthPermission::ViewLiveVideo));
	}

	TEST_METHOD(when_check_perssmion_to_transcoder_auth_object_then_return_false)
	{
		// DefaultAuthenticationInfo::hasPermissionToAuthObject test

		// arrange
		MockAuthObject mockAuthObject;

		EXPECT_CALL(mockAuthObject, getType())
			.WillOnce(Return(vrm::usermanagement::AuthObjectType::Transcoder));

		BuiltinAuthenticationInfo builtinAuthInfo(BuiltinAuthenticationInfo::Type::RecordingDevice, std::string());

		// act
		// assert
		Assert::IsFalse(builtinAuthInfo.hasPermissionToAuthObject(&mockAuthObject, vrm::usermanagement::AuthPermission::ViewLiveVideo));
	}
};