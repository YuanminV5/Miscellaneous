#include <vcslib/xml/Element.h> // code to test

#include <CppUnitTest.h> // cpp unit test framework
#include <string> // standard library
#include <gmock/gmock.h> // google mock

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using ::testing::_;
using ::testing::Return;
using ::testing::An;
using ::testing::DoAll;
using ::testing::SetArgReferee;

/** This is a sample test served as a reference for unit test development in VRM. */
TEST_CLASS(SampleTest)
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

	TEST_METHOD(SampleTest1)
	{
		vcslib::xml::Element element;

		// arrange
		// prepare mock, establish connection between wrapper object and mock object.
		MockElement mockElement;
		vcslib::xml::Element::s_mock = &mockElement;

		// set expectation and mock return value
		EXPECT_CALL(mockElement, getText(An<std::string&>()))
			.WillOnce(Return(true));

		// act
		// invoke method of code under test
		std::string str("TestString");
		bool result = element.getText(str);

		// assert
		Assert::IsTrue(result);
	}
};