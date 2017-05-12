#include <src/user_management/TranscoderAuthObject.h>

#include <CppUnitTest.h> 
#include <string> 

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(TranscoderAuthObjectTests)
{
public:
	TEST_CLASS_INITIALIZE(ClassInitialize)
	{
	}

	TEST_CLASS_CLEANUP(ClassCleanup)
	{
	}

	TEST_METHOD(when_create_a_transcoder_auth_object_then_type_is_set_as_transcoder)
	{
		// constructor test

		// arrange
		// act
		TranscoderAuthObject transcoderAuthObject;

		// assert
		Assert::IsTrue(transcoderAuthObject.getType() == vrm::usermanagement::AuthObjectType::Transcoder);
	}
};