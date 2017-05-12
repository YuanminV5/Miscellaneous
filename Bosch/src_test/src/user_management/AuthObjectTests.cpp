#include <src/user_management/AuthObject.h>

#include <CppUnitTest.h> 
#include <string> 

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(AuthObjectTests)
{
public:
	TEST_CLASS_INITIALIZE(ClassInitialize)
	{
	}

	TEST_CLASS_CLEANUP(ClassCleanup)
	{
	}

	TEST_METHOD(when_create_an_auth_object_with_type_then_type_can_be_retrieved)
	{
		// constructor test
		
		// arrange
		vrm::usermanagement::AuthObjectType type = vrm::usermanagement::AuthObjectType::Camera;

		// act
		AuthObject authObject(type);

		// assert
		Assert::IsTrue(authObject.getType() == type);
	}

	TEST_METHOD(when_create_an_auth_object_with_key_values_then_key_values_can_be_retrieved)
	{
		// constructor test

		// arrange
		vrm::usermanagement::AuthObjectType type = vrm::usermanagement::AuthObjectType::Camera;
		AuthObject::KeyValueContainer keyValueContainer;

		keyValueContainer[vrm::usermanagement::AuthObjectKey::Guid] = "guid";
		keyValueContainer[vrm::usermanagement::AuthObjectKey::Url] = "url";
		keyValueContainer[vrm::usermanagement::AuthObjectKey::CameraTrackId] = "track id";
		keyValueContainer[vrm::usermanagement::AuthObjectKey::CameraLine] = "line";

		// act
		AuthObject authObject(type, keyValueContainer);

		// assert
		for (auto i : keyValueContainer)
		{
			Assert::IsTrue(i.second == authObject.getValue(i.first));
		}
	}

	TEST_METHOD(when_get_value_for_a_key_that_does_not_exist_then_empty_string_is_returned)
	{
		// AuthObject::getValue test

		// arrange
		vrm::usermanagement::AuthObjectType type = vrm::usermanagement::AuthObjectType::Camera;
		AuthObject authObject(type);

		// act
		std::string value(authObject.getValue(vrm::usermanagement::AuthObjectKey::CameraLine));

		// assert
		Assert::IsTrue(value.empty());
	}
};