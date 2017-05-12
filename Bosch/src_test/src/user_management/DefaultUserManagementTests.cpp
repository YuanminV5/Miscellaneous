#include <src/user_management/DefaultUserManagement.h>
#include <src/user_management/Constants.h>

#include <CppUnitTest.h>
#include <string> 

#include <vcslib/xml/Element.h>
#include <MockLoginRequest.h>
#include <MockUserCredential.h>
#include <MockElement.h>
#include <gmock/gmock.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using ::testing::_;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::An;
using ::testing::DoAll;

TEST_CLASS(DefaultUserManagementTests)
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

	TEST_METHOD(when_get_auth_info_from_null_login_request_then_exception_invalid_argument_is_thrown)
	{
		// DefaultUserManagement::getAuthInfo test

		// arrange
		DefaultUserManagement usrmgt;

		// act
		// assert
		Assert::IsTrue(usrmgt.getAuthInfo(nullptr) == nullptr);
	}

	TEST_METHOD(when_get_auth_info_from_login_requst_that_have_invalid_credential_count_then_nullptr_is_returned)
	{
		// DefaultUserManagement::getAuthInfo test

		// arrange
		DefaultUserManagement usrmgt;
		MockLoginRequest mockLoginRequest;

		EXPECT_CALL(mockLoginRequest, getCredentialCount())
			.WillOnce(Return(3));

		// act
		// assert
		Assert::IsTrue(usrmgt.getAuthInfo(&mockLoginRequest) == nullptr);
	}

	TEST_METHOD(when_get_auth_info_from_login_requst_that_have_null_credential_then_nullptr_is_returned)
	{
		// DefaultUserManagement::getAuthInfo test

		// arrange
		DefaultUserManagement usrmgt;
		MockLoginRequest mockLoginRequest;

		EXPECT_CALL(mockLoginRequest, getCredentialCount())
			.WillOnce(Return(1));

		EXPECT_CALL(mockLoginRequest, getCredential(_))
			.WillOnce(Return(nullptr));

		// act
		// assert
		Assert::IsTrue(usrmgt.getAuthInfo(&mockLoginRequest) == nullptr);
	}

	TEST_METHOD(when_get_auth_info_from_login_requst_that_have_non_UserPassword_credential_then_nullptr_is_returned)
	{
		// DefaultUserManagement::getAuthInfo test

		// arrange
		DefaultUserManagement usrmgt;
		MockLoginRequest mockLoginRequest;

		MockUserCredential mockUserCredential;

		EXPECT_CALL(mockLoginRequest, getCredentialCount())
			.WillOnce(Return(1));

		EXPECT_CALL(mockLoginRequest, getCredential(_))
			.WillOnce(Return(&mockUserCredential));

		EXPECT_CALL(mockUserCredential, getType())
			.WillOnce(Return(vrm::usermanagement::CredentialType::Token));

		// act
		// assert
		Assert::IsTrue(usrmgt.getAuthInfo(&mockLoginRequest) == nullptr);
	}

	TEST_METHOD(when_get_auth_info_but_user_does_not_exist_then_nullptr_is_returned)
	{
		// DefaultUserManagement::getAuthInfo test

		// arrange
		DefaultUserManagement usrmgt;
		MockLoginRequest mockLoginRequest;
		MockUserCredential mockUserCredential;

		MockElement mockElement;
		vcslib::xml::Element::s_mock = &mockElement;

		EXPECT_CALL(mockLoginRequest, getCredentialCount())
			.WillOnce(Return(1));

		EXPECT_CALL(mockLoginRequest, getCredential(_))
			.WillOnce(Return(&mockUserCredential));

		EXPECT_CALL(mockUserCredential, getType())
			.WillOnce(Return(vrm::usermanagement::CredentialType::UserPassword));

		EXPECT_CALL(mockUserCredential, getCredentialValue(_))
			.Times(3)
			.WillOnce(Return("user"))
			.WillOnce(Return("password"))
			.WillOnce(Return(""));

		EXPECT_CALL(mockElement, operatorBool())
			.WillOnce(Return(false));

		// act
		// assert
		Assert::IsTrue(usrmgt.getAuthInfo(&mockLoginRequest) == nullptr);
	}

	TEST_METHOD(when_get_auth_info_but_single_user_is_only_for_four_eyes_then_nullptr_is_returned)
	{
		// DefaultUserManagement::getAuthInfo test

		// arrange
		DefaultUserManagement usrmgt;
		MockLoginRequest mockLoginRequest;
		MockUserCredential mockUserCredential;

		MockElement mockElement;
		vcslib::xml::Element::s_mock = &mockElement;

		EXPECT_CALL(mockLoginRequest, getCredentialCount())
			.WillOnce(Return(1));

		EXPECT_CALL(mockLoginRequest, getCredential(_))
			.WillOnce(Return(&mockUserCredential));

		EXPECT_CALL(mockUserCredential, getType())
			.WillOnce(Return(vrm::usermanagement::CredentialType::UserPassword));

		EXPECT_CALL(mockUserCredential, getCredentialValue(_))
			.Times(3)
			.WillOnce(Return("user"))
			.WillOnce(Return("password"))
			.WillOnce(Return(""));

		EXPECT_CALL(mockElement, operatorBool())
			.WillOnce(Return(true));

		EXPECT_CALL(mockElement, getText(An<std::string&>()))
			.WillOnce(Return(true));  

		EXPECT_CALL(mockElement, getText(An<unsigned long&>()))
			.Times(2)
			.WillOnce(DoAll(SetArgReferee<0>(vrm::usermanagement::c_groupAdmin), Return(true)))  
			.WillOnce(DoAll(SetArgReferee<0>(4194304), Return(true)));  

		// act
		// assert
		Assert::IsTrue(usrmgt.getAuthInfo(&mockLoginRequest) == nullptr);
	}

	TEST_METHOD(when_get_auth_info_but_single_user_is_observer_then_nullptr_is_returned)
	{
		// DefaultUserManagement::getAuthInfo test

		// arrange
		DefaultUserManagement usrmgt;
		MockLoginRequest mockLoginRequest;
		MockUserCredential mockUserCredential;

		MockElement mockElement;
		vcslib::xml::Element::s_mock = &mockElement;


		EXPECT_CALL(mockLoginRequest, getCredentialCount())
			.WillOnce(Return(1));

		EXPECT_CALL(mockLoginRequest, getCredential(_))
			.WillOnce(Return(&mockUserCredential));

		EXPECT_CALL(mockUserCredential, getType())
			.WillOnce(Return(vrm::usermanagement::CredentialType::UserPassword));

		EXPECT_CALL(mockUserCredential, getCredentialValue(_))
			.Times(3)
			.WillOnce(Return("user"))
			.WillOnce(Return("password"))
			.WillOnce(Return(""));

		EXPECT_CALL(mockElement, operatorBool())
			.WillOnce(Return(true));

		EXPECT_CALL(mockElement, getText(An<std::string&>()))
			.WillOnce(Return(true));  

		EXPECT_CALL(mockElement, getText(An<unsigned long&>()))
			.Times(2)
			.WillOnce(DoAll(SetArgReferee<0>(vrm::usermanagement::c_groupObserver), Return(true)))  
			.WillOnce(DoAll(SetArgReferee<0>(1), Return(true)));  

		// act
		// assert
		Assert::IsTrue(usrmgt.getAuthInfo(&mockLoginRequest) == nullptr);
	}

	//***************************************************

	TEST_METHOD(when_get_auth_info_for_four_eyes_but_second_credential_is_null_then_exception_invalid_argument_is_thrown)
	{
		// DefaultUserManagement::getAuthInfo test

		// arrange
		DefaultUserManagement usrmgt;
		MockLoginRequest mockLoginRequest;
		MockUserCredential mockUserCredential;

		MockElement mockElement;
		vcslib::xml::Element::s_mock = &mockElement;

		EXPECT_CALL(mockLoginRequest, getCredentialCount())
			.WillOnce(Return(vrm::usermanagement::c_fourEyesLoginCredentialCount));

		EXPECT_CALL(mockLoginRequest, getCredential(_))
			.WillOnce(Return(&mockUserCredential))
			.WillOnce(Return(nullptr));

		EXPECT_CALL(mockUserCredential, getType())
			.WillOnce(Return(vrm::usermanagement::CredentialType::UserPassword));

		EXPECT_CALL(mockUserCredential, getCredentialValue(_))
			.Times(3)
			.WillOnce(Return("user"))
			.WillOnce(Return("password"))
			.WillOnce(Return(""));

		EXPECT_CALL(mockElement, operatorBool())
			.WillOnce(Return(true));

		EXPECT_CALL(mockElement, getText(An<std::string&>()))
			.WillOnce(Return(true));  

		EXPECT_CALL(mockElement, getText(An<unsigned long&>()))
			.Times(2)
			.WillOnce(DoAll(SetArgReferee<0>(vrm::usermanagement::c_groupObserver), Return(true)))  
			.WillOnce(DoAll(SetArgReferee<0>(1), Return(true)));  

		// act
		// assert
		Assert::IsTrue(usrmgt.getAuthInfo(&mockLoginRequest) == nullptr);
	}

	TEST_METHOD(when_get_auth_info_for_four_eyes_but_second_user_does_not_exist_then_exception_runtime_error_is_thrown)
	{
		// DefaultUserManagement::getAuthInfo test

		// arrange
		DefaultUserManagement usrmgt;
		MockLoginRequest mockLoginRequest;
		MockUserCredential mockUserCredential;

		MockElement mockElement;
		vcslib::xml::Element::s_mock = &mockElement;

		EXPECT_CALL(mockLoginRequest, getCredentialCount())
			.WillOnce(Return(vrm::usermanagement::c_fourEyesLoginCredentialCount));

		EXPECT_CALL(mockLoginRequest, getCredential(_))
			.Times(2)
			.WillOnce(Return(&mockUserCredential))
			.WillOnce(Return(&mockUserCredential));

		EXPECT_CALL(mockUserCredential, getType())
			.WillOnce(Return(vrm::usermanagement::CredentialType::UserPassword));

		EXPECT_CALL(mockUserCredential, getCredentialValue(_))
			.Times(5)
			.WillOnce(Return("user"))
			.WillOnce(Return("password"))
			.WillOnce(Return(""))
			.WillOnce(Return("user2"))
			.WillOnce(Return("password2"));

		EXPECT_CALL(mockElement, operatorBool())
			.Times(2)
			.WillOnce(Return(true))  
			.WillOnce(Return(false));  

		EXPECT_CALL(mockElement, getText(An<std::string&>()))
			.WillOnce(Return(true));  

		EXPECT_CALL(mockElement, getText(An<unsigned long&>()))
			.Times(2)
			.WillOnce(DoAll(SetArgReferee<0>(vrm::usermanagement::c_groupObserver), Return(true)))  
			.WillOnce(DoAll(SetArgReferee<0>(1), Return(true)));  

		// act
		// assert
		Assert::IsTrue(usrmgt.getAuthInfo(&mockLoginRequest) == nullptr);
	}

	TEST_METHOD(when_get_auth_info_for_four_eyes_but_no_observer_then_nullptr_is_returned)
	{
		// DefaultUserManagement::getAuthInfo test

		// arrange
		DefaultUserManagement usrmgt;
		MockLoginRequest mockLoginRequest;
		MockUserCredential mockUserCredential;

		MockElement mockElement;
		vcslib::xml::Element::s_mock = &mockElement;

		EXPECT_CALL(mockLoginRequest, getCredentialCount())
			.WillOnce(Return(vrm::usermanagement::c_fourEyesLoginCredentialCount));

		EXPECT_CALL(mockLoginRequest, getCredential(_))
			.Times(2)
			.WillOnce(Return(&mockUserCredential))
			.WillOnce(Return(&mockUserCredential));

		EXPECT_CALL(mockUserCredential, getType())
			.WillOnce(Return(vrm::usermanagement::CredentialType::UserPassword));

		EXPECT_CALL(mockUserCredential, getCredentialValue(_))
			.Times(5)
			.WillOnce(Return("user"))
			.WillOnce(Return("password"))
			.WillOnce(Return(""))
			.WillOnce(Return("user2"))
			.WillOnce(Return("password2"));

		EXPECT_CALL(mockElement, operatorBool())
			.Times(2)
			.WillOnce(Return(true))  
			.WillOnce(Return(true));  

		EXPECT_CALL(mockElement, getText(An<std::string&>()))
			.WillOnce(Return(true));  

		EXPECT_CALL(mockElement, getText(An<unsigned long&>()))
			.Times(3)
			.WillOnce(DoAll(SetArgReferee<0>(vrm::usermanagement::c_groupAdmin), Return(true))) // getText(groupId)
			.WillOnce(DoAll(SetArgReferee<0>(1), Return(true))) // getText(permission)
			.WillOnce(DoAll(SetArgReferee<0>(vrm::usermanagement::c_groupAdmin), Return(true))); // getText(groupId2)

		// act
		// assert
		Assert::IsTrue(usrmgt.getAuthInfo(&mockLoginRequest) == nullptr);
	}

	TEST_METHOD(when_get_auth_info_for_four_eyes_but_two_users_are_all_observer_then_nullptr_is_returned)
	{
		// DefaultUserManagement::getAuthInfo test

		// arrange
		DefaultUserManagement usrmgt;
		MockLoginRequest mockLoginRequest;
		MockUserCredential mockUserCredential;

		MockElement mockElement;
		vcslib::xml::Element::s_mock = &mockElement;

		EXPECT_CALL(mockLoginRequest, getCredentialCount())
			.WillOnce(Return(vrm::usermanagement::c_fourEyesLoginCredentialCount));

		EXPECT_CALL(mockLoginRequest, getCredential(_))
			.Times(2)
			.WillOnce(Return(&mockUserCredential))
			.WillOnce(Return(&mockUserCredential));

		EXPECT_CALL(mockUserCredential, getType())
			.WillOnce(Return(vrm::usermanagement::CredentialType::UserPassword));

		EXPECT_CALL(mockUserCredential, getCredentialValue(_))
			.Times(5)
			.WillOnce(Return("user"))
			.WillOnce(Return("password"))
			.WillOnce(Return(""))
			.WillOnce(Return("user2"))
			.WillOnce(Return("password2"));

		EXPECT_CALL(mockElement, operatorBool())
			.Times(2)
			.WillOnce(Return(true))  
			.WillOnce(Return(true));  

		EXPECT_CALL(mockElement, getText(An<std::string&>()))
			.WillOnce(Return(true));  

		EXPECT_CALL(mockElement, getText(An<unsigned long&>()))
			.Times(3)
			.WillOnce(DoAll(SetArgReferee<0>(vrm::usermanagement::c_groupObserver), Return(true)))  
			.WillOnce(DoAll(SetArgReferee<0>(1), Return(true)))  
			.WillOnce(DoAll(SetArgReferee<0>(vrm::usermanagement::c_groupObserver), Return(true)));  

		// act
		// assert
		Assert::IsTrue(usrmgt.getAuthInfo(&mockLoginRequest) == nullptr);
	}

	TEST_METHOD(when_get_auth_info_for_four_eyes_but_first_user_password_is_not_correct_then_nullptr_is_returned)
	{
		// DefaultUserManagement::getAuthInfo test

		// arrange
		DefaultUserManagement usrmgt;
		MockLoginRequest mockLoginRequest;
		MockUserCredential mockUserCredential;

		MockElement mockElement;
		vcslib::xml::Element::s_mock = &mockElement;

		EXPECT_CALL(mockLoginRequest, getCredentialCount())
			.WillOnce(Return(vrm::usermanagement::c_fourEyesLoginCredentialCount));

		EXPECT_CALL(mockLoginRequest, getCredential(_))
			.Times(2)
			.WillOnce(Return(&mockUserCredential))
			.WillOnce(Return(&mockUserCredential));

		EXPECT_CALL(mockUserCredential, getType())
			.WillOnce(Return(vrm::usermanagement::CredentialType::UserPassword));

		EXPECT_CALL(mockUserCredential, getCredentialValue(_))
			.Times(5)
			.WillOnce(Return("user"))
			.WillOnce(Return("password"))
			.WillOnce(Return(""))
			.WillOnce(Return("user2"))
			.WillOnce(Return("password2"));

		EXPECT_CALL(mockElement, operatorBool())
			.Times(2)
			.WillOnce(Return(true))
			.WillOnce(Return(true));

		EXPECT_CALL(mockElement, getText(An<std::string&>()))
			.Times(2)
			.WillOnce(DoAll(SetArgReferee<0>(std::string("wrongpassword")), Return(true)))
			.WillOnce(DoAll(SetArgReferee<0>(std::string("password2")), Return(true)));

		EXPECT_CALL(mockElement, getText(An<unsigned long&>()))
			.Times(3)
			.WillOnce(DoAll(SetArgReferee<0>(vrm::usermanagement::c_groupObserver), Return(true)))
			.WillOnce(DoAll(SetArgReferee<0>(1), Return(true)))
			.WillOnce(DoAll(SetArgReferee<0>(vrm::usermanagement::c_groupAdmin), Return(true)));

		// act
		// assert
		Assert::IsTrue(usrmgt.getAuthInfo(&mockLoginRequest) == nullptr);
	}

	TEST_METHOD(when_get_auth_info_for_four_eyes_but_second_user_password_is_not_correct_then_nullptr_is_returned)
	{
		// DefaultUserManagement::getAuthInfo test

		// arrange
		DefaultUserManagement usrmgt;
		MockLoginRequest mockLoginRequest;
		MockUserCredential mockUserCredential;

		MockElement mockElement;
		vcslib::xml::Element::s_mock = &mockElement;

		EXPECT_CALL(mockLoginRequest, getCredentialCount())
			.WillOnce(Return(vrm::usermanagement::c_fourEyesLoginCredentialCount));

		EXPECT_CALL(mockLoginRequest, getCredential(_))
			.Times(2)
			.WillOnce(Return(&mockUserCredential))
			.WillOnce(Return(&mockUserCredential));

		EXPECT_CALL(mockUserCredential, getType())
			.WillOnce(Return(vrm::usermanagement::CredentialType::UserPassword));

		EXPECT_CALL(mockUserCredential, getCredentialValue(_))
			.Times(5)
			.WillOnce(Return("user"))
			.WillOnce(Return("password"))
			.WillOnce(Return(""))
			.WillOnce(Return("user2"))
			.WillOnce(Return("password2"));

		EXPECT_CALL(mockElement, operatorBool())
			.Times(2)
			.WillOnce(Return(true))
			.WillOnce(Return(true));

		EXPECT_CALL(mockElement, getText(An<std::string&>()))
			.Times(2)
			.WillOnce(DoAll(SetArgReferee<0>(std::string("password")), Return(true)))
			.WillOnce(DoAll(SetArgReferee<0>(std::string("wrongpassword")), Return(true)));

		EXPECT_CALL(mockElement, getText(An<unsigned long&>()))
			.Times(3)
			.WillOnce(DoAll(SetArgReferee<0>(vrm::usermanagement::c_groupAdmin), Return(true)))
			.WillOnce(DoAll(SetArgReferee<0>(1), Return(true)))
			.WillOnce(DoAll(SetArgReferee<0>(vrm::usermanagement::c_groupObserver), Return(true)));

		// act
		// assert
		Assert::IsTrue(usrmgt.getAuthInfo(&mockLoginRequest) == nullptr);
	}

	TEST_METHOD(when_get_auth_info_for_four_eyes_then_auth_result_is_determined_by_the_normal_user)
	{
		// DefaultUserManagement::getAuthInfo test

		// arrange
		DefaultUserManagement usrmgt;
		MockLoginRequest mockLoginRequest;
		MockUserCredential mockUserCredential;

		MockElement mockElement;
		vcslib::xml::Element::s_mock = &mockElement;

		EXPECT_CALL(mockLoginRequest, getCredentialCount())
			.WillOnce(Return(vrm::usermanagement::c_fourEyesLoginCredentialCount));

		EXPECT_CALL(mockLoginRequest, getCredential(_))
			.Times(2)
			.WillOnce(Return(&mockUserCredential))
			.WillOnce(Return(&mockUserCredential));

		EXPECT_CALL(mockUserCredential, getType())
			.WillOnce(Return(vrm::usermanagement::CredentialType::UserPassword));

		EXPECT_CALL(mockUserCredential, getCredentialValue(_))
			.Times(5)
			.WillOnce(Return("user"))
			.WillOnce(Return("password"))
			.WillOnce(Return(""))
			.WillOnce(Return("user2"))
			.WillOnce(Return("password2"));

		EXPECT_CALL(mockElement, operatorBool())
			.Times(2)
			.WillOnce(Return(true)) 
			.WillOnce(Return(true));

		EXPECT_CALL(mockElement, getText(An<std::string&>()))
			.Times(2)
			.WillOnce(DoAll(SetArgReferee<0>(std::string("password")), Return(true)))
			.WillOnce(DoAll(SetArgReferee<0>(std::string("password2")), Return(true)));

		EXPECT_CALL(mockElement, getText(An<unsigned long&>()))
			.Times(3)
			.WillOnce(DoAll(SetArgReferee<0>(vrm::usermanagement::c_groupObserver), Return(true)))
			.WillOnce(DoAll(SetArgReferee<0>(1), Return(true)))
			.WillOnce(DoAll(SetArgReferee<0>(vrm::usermanagement::c_groupAdmin), Return(true)));

		// act
		auto authIfo = usrmgt.getAuthInfo(&mockLoginRequest);

		// assert
		Assert::IsTrue(authIfo->getAuthenticationStatus() == vrm::usermanagement::AuthStatus::AdministratorLevel);
	}
};