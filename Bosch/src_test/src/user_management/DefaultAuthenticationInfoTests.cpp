#include <src/user_management/DefaultAuthenticationInfo.h>

#include <CppUnitTest.h> 
#include <src/user_management/Constants.h>
#include <string> 
#include <MockAuthObject.h>
#include <MockElement.h>
#include <vcslib/xml/Element.h>
#include <gmock/gmock.h> 

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using ::testing::_;
using ::testing::Return;
using ::testing::An;
using ::testing::DoAll;
using ::testing::SetArgReferee;

TEST_CLASS(DefaultAuthenticationInfoTests)
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

	TEST_METHOD(when_create_an_default_auth_info_by_defalt_then_auth_result_is_not_authenticated)
	{
		// constructor test

		// arrange
		DefaultAuthenticationInfo defaultAuthInfo;

		// act
		// assert
		Assert::IsTrue(defaultAuthInfo.getAuthenticationStatus() == vrm::usermanagement::AuthStatus::NotAuthenticated);
	}

	TEST_METHOD(when_create_an_default_auth_info_with_group_admin_then_auth_result_is_administrator_level)
	{
		// constructor test

		// arrange
		DefaultAuthenticationInfo defaultAuthInfo(vrm::usermanagement::c_groupAdmin);

		// act
		// assert
		Assert::IsTrue(defaultAuthInfo.getAuthenticationStatus() == vrm::usermanagement::AuthStatus::AdministratorLevel);
	}

	TEST_METHOD(when_create_an_default_auth_info_with_observer_group_then_auth_result_is_AdditionalUserRequired)
	{
		// constructor test

		// arrange
		DefaultAuthenticationInfo defaultAuthInfo(vrm::usermanagement::c_groupObserver);

		// act
		// assert
		Assert::IsTrue(defaultAuthInfo.getAuthenticationStatus() == vrm::usermanagement::AuthStatus::AdditionalUserRequired);
	}

	TEST_METHOD(when_create_an_default_auth_info_with_non_admin_group_then_auth_result_is_user_level)
	{
		// constructor test

		// arrange
		DefaultAuthenticationInfo defaultAuthInfo(20);

		// act
		// assert
		Assert::IsTrue(defaultAuthInfo.getAuthenticationStatus() == vrm::usermanagement::AuthStatus::UserLevel);
	}

	TEST_METHOD(when_check_perssmion_to_auth_object_if_no_specified_permission_then_return_false)
	{
		// DefaultAuthenticationInfo::hasPermissionToAuthObject test

		// arrange
		MockAuthObject mockAuthObject;

		MockElement mockElement;
		vcslib::xml::Element::s_mock = &mockElement;

		EXPECT_CALL(mockElement, getText(An<unsigned long&>()))
			.WillOnce(DoAll(SetArgReferee<0>(0), Return(true)));

		DefaultAuthenticationInfo defaultAuthInfo;

		// act
		// assert
		Assert::IsFalse(defaultAuthInfo.hasPermissionToAuthObject(&mockAuthObject, vrm::usermanagement::AuthPermission::DeleteVideo));
	}

	TEST_METHOD(when_check_perssmion_to_null_auth_object_then_return_false)
	{
		// DefaultAuthenticationInfo::hasPermissionToAuthObject test

		// arrange
		DefaultAuthenticationInfo defaultAuthInfo;

		// act
		// assert
		Assert::IsFalse(defaultAuthInfo.hasPermissionToAuthObject(nullptr, vrm::usermanagement::AuthPermission::DeleteVideo));
	}

	TEST_METHOD(when_check_perssmion_to_camera_auth_object_and_group_not_is_not_in_configured_privs_list_then_return_false)
	{
		// DefaultAuthenticationInfo::hasPermissionToAuthObject test

		// arrange
		MockAuthObject mockAuthObject;

		MockElement mockElement;
		vcslib::xml::Element::s_mock = &mockElement;

		EXPECT_CALL(mockElement, getText(An<unsigned long&>()))
			.WillOnce(DoAll(SetArgReferee<0>(1 << (unsigned char)vrm::usermanagement::AuthPermission::DeleteVideo), Return(true)));

		EXPECT_CALL(mockElement, getAttribute(An<const char*>(), An<unsigned long&>()))
			.WillOnce(DoAll(SetArgReferee<1>(0), Return(true)));

		EXPECT_CALL(mockAuthObject, getType())
			.WillOnce(Return(vrm::usermanagement::AuthObjectType::Camera));

		EXPECT_CALL(mockAuthObject, getValue(_))
			.WillRepeatedly(Return(nullptr));

		DefaultAuthenticationInfo defaultAuthInfo(vrm::usermanagement::c_groupObserver);

		// act
		// assert
		Assert::IsFalse(defaultAuthInfo.hasPermissionToAuthObject(&mockAuthObject, vrm::usermanagement::AuthPermission::DeleteVideo));
	}

	TEST_METHOD(when_check_perssmion_to_camera_auth_object_and_group_is_in_configured_privs_list_then_return_true)
	{
		// DefaultAuthenticationInfo::hasPermissionToAuthObject test

		// arrange
		MockAuthObject mockAuthObject;

		MockElement mockElement;
		vcslib::xml::Element::s_mock = &mockElement;

		EXPECT_CALL(mockElement, getText(An<unsigned long&>()))
			.WillOnce(DoAll(SetArgReferee<0>(1 << (unsigned char)vrm::usermanagement::AuthPermission::DeleteVideo), Return(true)));

		EXPECT_CALL(mockElement, getAttribute(An<const char*>(), An<unsigned long&>()))
			.WillOnce(DoAll(SetArgReferee<1>(4294967295), Return(true)));

		EXPECT_CALL(mockAuthObject, getType())
			.WillOnce(Return(vrm::usermanagement::AuthObjectType::Camera));

		EXPECT_CALL(mockAuthObject, getValue(_))
			.WillRepeatedly(Return(nullptr));

		DefaultAuthenticationInfo defaultAuthInfo(vrm::usermanagement::c_groupAdmin);

		// act
		// assert
		Assert::IsTrue(defaultAuthInfo.hasPermissionToAuthObject(&mockAuthObject, vrm::usermanagement::AuthPermission::DeleteVideo));
	}

	TEST_METHOD(when_check_perssmion_to_transcoder_auth_object_and_group_permisson_is_not_set_then_return_false)
	{
		// DefaultAuthenticationInfo::hasPermissionToAuthObject test

		// arrange
		MockAuthObject mockAuthObject;

		MockElement mockElement;
		vcslib::xml::Element::s_mock = &mockElement;

		EXPECT_CALL(mockElement, getText(An<unsigned long&>()))
			.WillOnce(DoAll(SetArgReferee<0>(1 << (unsigned char)vrm::usermanagement::AuthPermission::DeleteVideo), Return(true)));

		EXPECT_CALL(mockAuthObject, getType())
			.WillOnce(Return(vrm::usermanagement::AuthObjectType::Transcoder));

		DefaultAuthenticationInfo defaultAuthInfo(vrm::usermanagement::c_groupAdmin);

		// act
		// assert
		Assert::IsFalse(defaultAuthInfo.hasPermissionToAuthObject(&mockAuthObject, vrm::usermanagement::AuthPermission::DeleteVideo));
	}

	TEST_METHOD(when_check_perssmion_to_transcoder_auth_object_and_group_permisson_is_set_then_return_true)
	{
		// DefaultAuthenticationInfo::hasPermissionToAuthObject test

		// arrange
		MockAuthObject mockAuthObject;

		MockElement mockElement;
		vcslib::xml::Element::s_mock = &mockElement;

		EXPECT_CALL(mockElement, getText(An<unsigned long&>()))
			.WillOnce(DoAll(SetArgReferee<0>(4294967295), Return(true)));

		EXPECT_CALL(mockAuthObject, getType())
			.WillOnce(Return(vrm::usermanagement::AuthObjectType::Transcoder));

		DefaultAuthenticationInfo defaultAuthInfo(vrm::usermanagement::c_groupAdmin);

		// act
		// assert
		Assert::IsTrue(defaultAuthInfo.hasPermissionToAuthObject(&mockAuthObject, vrm::usermanagement::AuthPermission::DeleteVideo));
	}
};