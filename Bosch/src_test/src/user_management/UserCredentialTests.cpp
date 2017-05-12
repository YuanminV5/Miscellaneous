#include<src/user_management/UserCredential.h>

#include <CppUnitTest.h>
#include <vector>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(UserCredentialTests)
{
public:
	TEST_CLASS_INITIALIZE(ClassInitialize)
	{
	}

	TEST_CLASS_CLEANUP(ClassCleanup)
	{
	}

	TEST_METHOD(when_create_a_user_credential_with_cert_type_and_credential_parts_then_type_and_credential_parts_can_be_retrieved)
	{
		// constructor test

		// arrange
		vrm::usermanagement::CredentialType type = vrm::usermanagement::CredentialType::Cert;
		std::string part1 = "part one";
		std::string part2 = "part two";
		UserCredential c1(type, part1, part2);

		// act
		auto t = c1.getType();

		const char* values[2] = { nullptr, nullptr };
		unsigned int size = 2;
		c1.getCredentialValues(values, size);

		// assert
		Assert::IsTrue(t == type);
		Assert::AreEqual(size, std::size_t(1));
		Assert::IsTrue(values[0] == part1);
	}

	TEST_METHOD(when_create_a_user_credential_with_token_type_and_credential_parts_then_type_and_credential_parts_can_be_retrieved)
	{
		// constructor test

		// arrange
		vrm::usermanagement::CredentialType type = vrm::usermanagement::CredentialType::Token;
		std::string part1 = "part one";
		std::string part2 = "part two";
		UserCredential c1(type, part1, part2);

		// act
		auto t = c1.getType();

		const char* values[2] = { nullptr, nullptr };
		unsigned int size = 2;
		c1.getCredentialValues(values, size);

		// assert
		Assert::IsTrue(t == type);
		Assert::AreEqual(size, std::size_t(1));
		Assert::IsTrue(values[0] == part1);
	}

	TEST_METHOD(when_create_a_user_credential_with_UserPassword_type_and_credential_parts_then_type_and_credential_parts_can_be_retrieved)
	{
		// constructor test

		// arrange
		vrm::usermanagement::CredentialType type = vrm::usermanagement::CredentialType::UserPassword;
		std::string part1 = "part one";
		std::string part2 = "part two";
		UserCredential c1(type, part1, part2);

		// act
		auto t = c1.getType();
		const char* values[2] = {nullptr, nullptr};
		unsigned int size = 2;
		c1.getCredentialValues(values, size);

		// assert
		Assert::IsTrue(t == type);
		Assert::IsTrue(size == std::size_t(2));
		Assert::IsTrue(values[0] == part1);
		Assert::IsTrue(values[1] == part2);
	}

	TEST_METHOD(when_create_a_user_credential_with_user_and_password_then_type_is_UserPassword_and_UserPassword_can_be_retrieved)
	{
		// constructor test

		// arrange
		std::string user = "administrator";
		std::string pasword = "password";
		UserCredential c1(user, pasword);

		// act
		auto t = c1.getType();

		const char* values[2] = { nullptr, nullptr };
		unsigned int size = 2;
		c1.getCredentialValues(values, size);

		// assert
		Assert::IsTrue(t == vrm::usermanagement::CredentialType::UserPassword);
		Assert::AreEqual(size, std::size_t(2));
		Assert::IsTrue(values[0] == user);
		Assert::IsTrue(values[1] == pasword);
	}

	TEST_METHOD(when_create_a_user_credential_from_another_one_then_type_and_credential_values_are_copied)
	{
		// copy constructor test

		// arrange
		std::string user = "administrator";
		std::string pasword = "password";
		UserCredential c1(user, pasword);

		// act
		UserCredential c2(c1);

		const char* values[2] = { nullptr, nullptr };
		unsigned int size = 2;
		c1.getCredentialValues(values, size);

		const char* values2[2] = { nullptr, nullptr };
		unsigned int size2 = 2;
		c2.getCredentialValues(values2, size2);

		// assert
		Assert::IsTrue(c1.getType() == c2.getType());
		Assert::IsTrue(size == size2);

		for (unsigned short index = 0; index < size; index++)
		{
			Assert::IsTrue(std::string(values[index]) == values2[index]);
		}
	}

	TEST_METHOD(when_retrieve_a_credential_value_whose_type_does_not_match_with_credential_type_cert_then_exception_runtime_error_is_thrown)
	{
		// UserCredential::getCredentialValue(CredentialValueType valueType) test

		// arrange
		std::string part1 = "part one";
		std::string part2 = "part two";
		UserCredential c1(vrm::usermanagement::CredentialType::Cert, part1, part2);

		auto func = [&c = c1]() -> void
		{
			c.getCredentialValue(vrm::usermanagement::CredentialValueType::Password);
		};

		// act
		// assert
		Assert::ExpectException<std::runtime_error>(func);
	}

	TEST_METHOD(when_retrieve_a_credential_value_whose_type_does_not_match_with_credential_type_token_then_exception_runtime_error_is_thrown)
	{
		// UserCredential::getCredentialValue(CredentialValueType valueType) test

		// arrange
		std::string part1 = "part one";
		std::string part2 = "part two";
		UserCredential c1(vrm::usermanagement::CredentialType::Token, part1, part2);

		auto func = [&c = c1]() -> void
		{
			c.getCredentialValue(vrm::usermanagement::CredentialValueType::Password);
		};

		// act
		// assert
		Assert::ExpectException<std::runtime_error>(func);
	}

	TEST_METHOD(when_retrieve_a_credential_value_whose_type_does_not_match_with_credential_type_UserPassword_then_exception_runtime_error_is_thrown)
	{
		// UserCredential::getCredentialValue(CredentialValueType valueType) test

		// arrange
		std::string part1 = "part one";
		std::string part2 = "part two";
		UserCredential c1(vrm::usermanagement::CredentialType::UserPassword, part1, part2);

		auto func = [&c = c1]() -> void
		{
			c.getCredentialValue(vrm::usermanagement::CredentialValueType::Cert);
		};

		// act
		// assert
		Assert::ExpectException<std::runtime_error>(func);
	}

	TEST_METHOD(when_retrieve_the_cert_value_from_cert_credential_then_value_can_be_retrieved)
	{
		// UserCredential::getCredentialValue(CredentialValueType valueType) test

		// arrange
		std::string part1 = "part one";
		std::string part2 = "part two";
		UserCredential c1(vrm::usermanagement::CredentialType::Cert, part1, part2);

		// act
		auto value = c1.getCredentialValue(vrm::usermanagement::CredentialValueType::Cert);

		// assert
		Assert::IsTrue(value == part1);
	}

	TEST_METHOD(when_retrieve_the_token_value_from_token_credential_then_value_can_be_retrieved)
	{
		// UserCredential::getCredentialValue(CredentialValueType valueType) test

		// arrange
		std::string part1 = "part one";
		std::string part2 = "part two";
		UserCredential c1(vrm::usermanagement::CredentialType::Token, part1, part2);

		// act
		auto value = c1.getCredentialValue(vrm::usermanagement::CredentialValueType::Token);

		// assert
		Assert::IsTrue(value == part1);
	}

	TEST_METHOD(when_retrieve_the_user_value_from_UserPassword_credential_then_value_can_be_retrieved)
	{
		// UserCredential::getCredentialValue(CredentialValueType valueType) test

		// arrange
		std::string part1 = "part one";
		std::string part2 = "part two";
		UserCredential c1(vrm::usermanagement::CredentialType::UserPassword, part1, part2);

		// act
		auto value = c1.getCredentialValue(vrm::usermanagement::CredentialValueType::User);

		// assert
		Assert::IsTrue(value == part1);
	}

	TEST_METHOD(when_retrieve_the_password_value_from_UserPassword_credential_then_value_can_be_retrieved)
	{
		// UserCredential::getCredentialValue(CredentialValueType valueType) test

		// arrange
		std::string part1 = "part one";
		std::string part2 = "part two";
		UserCredential c1(vrm::usermanagement::CredentialType::UserPassword, part1, part2);

		// act
		auto value = c1.getCredentialValue(vrm::usermanagement::CredentialValueType::Password);

		// assert
		Assert::IsTrue(value == part2);
	}
};