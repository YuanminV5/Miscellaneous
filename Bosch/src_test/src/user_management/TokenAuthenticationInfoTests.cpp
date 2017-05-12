#include <src/user_management/TokenAuthenticationInfo.h>

#include <CppUnitTest.h>
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

TEST_CLASS(TokenAuthenticationInfoTests)
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

	TEST_METHOD(when_create_a_token_auth_object_with_group_name_but_no_any_group_then_auth_result_is_not_authenticated)
	{
		// constructor test

		// arrange
		MockElementList mockElementList;
		vcslib::xml::ElementList::s_mock = &mockElementList;

		EXPECT_CALL(mockElementList, size())
			.WillOnce(Return(0));

		// act
		TokenAuthenticationInfo tokenAuthInfo("dummy group");

		// assert
		Assert::IsTrue(tokenAuthInfo.getAuthenticationStatus() == vrm::usermanagement::AuthStatus::NotAuthenticated);
	}

	TEST_METHOD(when_create_a_token_auth_object_with_group_name_but_group_is_not_found_then_auth_result_is_not_authenticated)
	{
		// constructor test

		// arrange
		MockElementList mockElementList;
		vcslib::xml::ElementList::s_mock = &mockElementList;

		MockElement mockElement;
		vcslib::xml::Element::s_mock = &mockElement;

		EXPECT_CALL(mockElementList, size())
			.WillRepeatedly(Return(1));

		EXPECT_CALL(mockElement, getText(An<std::string&>()))
			.WillOnce(DoAll(SetArgReferee<0>(std::string("group1")), Return(true)));

		// act
		TokenAuthenticationInfo tokenAuthInfo("dummy group");

		// assert
		Assert::IsTrue(tokenAuthInfo.getAuthenticationStatus() == vrm::usermanagement::AuthStatus::NotAuthenticated);
	}

	TEST_METHOD(when_create_a_token_auth_object_with_group_name_and_corresponding_group_id_is_valid_then_auth_result_is_determined_by_group_id)
	{
		// constructor test

		// arrange
		MockElementList mockElementList;
		vcslib::xml::ElementList::s_mock = &mockElementList;

		MockElement mockElement;
		vcslib::xml::Element::s_mock = &mockElement;

		EXPECT_CALL(mockElementList, size())
			.WillRepeatedly(Return(1));

		EXPECT_CALL(mockElement, getText(An<std::string&>()))
			.WillOnce(DoAll(SetArgReferee<0>(std::string("group1")), Return(true)));

		EXPECT_CALL(mockElement, getAttribute(An<const char*>(), An<unsigned long&>()))
			.WillOnce(DoAll(SetArgReferee<1>(55), Return(true)));

		// act
		TokenAuthenticationInfo tokenAuthInfo("group1");

		// assert
		Assert::IsTrue(tokenAuthInfo.getAuthenticationStatus() == vrm::usermanagement::AuthStatus::UserLevel);
	}

	TEST_METHOD(when_check_perssmion_to_auth_object_if_inherited_auth_info_is_null_then_return_false)
	{
		// TokenAuthenticationInfo::hasPermissionToAuthObject test

		// arrange
		// arrange
		MockElementList mockElementList;
		vcslib::xml::ElementList::s_mock = &mockElementList;

		EXPECT_CALL(mockElementList, size())
			.WillOnce(Return(0));

		TokenAuthenticationInfo tokenAuthInfo("group1");

		MockAuthObject mockAuthObject;

		// act
		// assert
		Assert::IsFalse(tokenAuthInfo.hasPermissionToAuthObject(&mockAuthObject, vrm::usermanagement::AuthPermission::DeleteVideo));
	}
};