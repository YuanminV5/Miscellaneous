#include <src/user_management/LoginRequest.h>
#include <src/user_management/UserCredential.h>

#include <CppUnitTest.h>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


TEST_CLASS(LoginRequestTests)
{
public:

	typedef std::shared_ptr<vrm::usermanagement::IUserCredential> CredentialPtr;

	TEST_CLASS_INITIALIZE(ClassInitialize)
	{
	}

	TEST_CLASS_CLEANUP(ClassCleanup)
	{
	}

	TEST_METHOD(when_create_a_login_request_without_any_user_credential_then_user_credential_count_is_0)
	{
		// arrange
		std::vector<CredentialPtr> credentials;
		std::unique_ptr<LoginRequest> loginRequest = std::make_unique<LoginRequest>(credentials);

		// act
		std::size_t count = loginRequest->getCredentialCount();

		// assert
		Assert::IsTrue(count == 0);
	}

	TEST_METHOD(when_create_a_login_request_with_any_user_credential_then_user_credential_count_is_not_0)
	{
		// arrange
		std::vector<CredentialPtr> credentials;
		credentials.push_back(CredentialPtr(new UserCredential("administrator", "password")));
		std::unique_ptr<LoginRequest> loginRequest = std::make_unique<LoginRequest>(credentials);

		// act
		std::size_t count = loginRequest->getCredentialCount();

		// assert
		Assert::IsTrue(count == 1);
	}

	TEST_METHOD(when_create_a_login_request_without_any_user_credential_then_exception_thrown_on_retrieving_user_credential)
	{
		// arrange
		std::vector<CredentialPtr> credentials;
		std::unique_ptr<LoginRequest> loginRequest = std::make_unique<LoginRequest>(credentials);

		// act
		auto func = [&r = loginRequest]() -> void
		{
			r->getCredential(0);
		};

		// assert
		Assert::ExpectException<std::out_of_range>(func);
	}

	TEST_METHOD(when_retrieve_a_user_credential_with_an_index_out_of_range_then_exception_out_of_range_thrown)
	{
		// arrange
		std::vector<CredentialPtr> credentials;
		credentials.push_back(CredentialPtr(new UserCredential("administrator", "password")));
		std::unique_ptr<LoginRequest> loginRequest = std::make_unique<LoginRequest>(credentials);

		auto func = [&r = loginRequest]() -> void
		{
			r->getCredential(1);
		};

		// act
		// assert
		Assert::ExpectException<std::out_of_range>(func);
	}

	TEST_METHOD(when_add_a_user_credential_that_is_not_in_the_login_request_then_credential_count_increased)
	{
		// arrange
		std::vector<CredentialPtr> credentials;
		std::unique_ptr<LoginRequest> loginRequest = std::make_unique<LoginRequest>(credentials);

		// act
		std::size_t originalCount = loginRequest->getCredentialCount();
		auto credential = CredentialPtr(new UserCredential("administrator", "password"));
		loginRequest->addUserCredential(credential);
		std::size_t newCount = loginRequest->getCredentialCount();

		// assert
		Assert::IsTrue((newCount - originalCount) == 1);
	}

	TEST_METHOD(when_add_a_user_credential_that_is_already_in_the_login_request_then_credential_count_not_increased)
	{
		// arrange
		std::vector<CredentialPtr> credentials;
		std::unique_ptr<LoginRequest> loginRequest = std::make_unique<LoginRequest>(credentials);

		// act
		auto credential = CredentialPtr(new UserCredential("administrator", "password"));
		loginRequest->addUserCredential(credential);

		std::size_t originalCount = loginRequest->getCredentialCount();

		std::size_t newCount = loginRequest->getCredentialCount();

		// assert
		Assert::IsTrue((newCount - originalCount) == 0);
	}
};