#include <src/user_management/CameraAuthObject.h>

#include <CppUnitTest.h> 
#include <string> 
#include <vrmsrc/media_database/Camera.h>
#include <vrmsrc/media_database/Device.h>
#include <vcslib/xml/Element.h>
#include <gmock/gmock.h> 

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using ::testing::_;
using ::testing::Return;
using ::testing::An;
using ::testing::DoAll;
using ::testing::SetArgReferee;
using ::testing::ReturnRef;

TEST_CLASS(CameraAuthObjectTests)
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

	TEST_METHOD(when_create_a_camera_auth_object_with_null_pointer_then_type_is_set_and_empty_string_is_returned_on_retrieving_value)
	{
		// constructor test

		// arrange
		CameraAuthObject cameraAuthObject(nullptr);

		// act
		std::string value(cameraAuthObject.getValue(vrm::usermanagement::AuthObjectKey::CameraLine));

		// assert
		Assert::IsTrue(cameraAuthObject.getType() == vrm::usermanagement::AuthObjectType::Camera);
		Assert::IsTrue(value.empty());
	}

	TEST_METHOD(when_create_a_camera_auth_object_with_camera_pointer_then_type_is_set_and_value_can_be_retrieved)
	{
		// constructor test

		// arrange
		Camera camera;
		Device device;

		MockGuid mockGuid;
		Guid::s_mock = &mockGuid;

		MockCameraId mockCameraId;
		CameraId::s_mock = &mockCameraId;

		EXPECT_CALL(camera, getDevice())
			.WillRepeatedly(Return(&device));

		EXPECT_CALL(camera, getTrackId())
			.WillRepeatedly(Return(0));

		EXPECT_CALL(camera, getAddress())
			.WillRepeatedly(ReturnRef(CameraId()));

		EXPECT_CALL(mockCameraId, getLine())
			.WillOnce(Return(0));

		EXPECT_CALL(device, getGuid())
			.WillOnce(ReturnRef(Guid()));

		EXPECT_CALL(device, getConnectUrl())
			.WillOnce(Return("connectedurl"));

		EXPECT_CALL(mockGuid, toString())
			.WillOnce(Return("guidid"));

		// act
		CameraAuthObject cameraAuthObject(&camera);

		// assert
		Assert::IsTrue(cameraAuthObject.getType() == vrm::usermanagement::AuthObjectType::Camera);
		Assert::IsTrue(cameraAuthObject.getValue(vrm::usermanagement::AuthObjectKey::Guid) == std::string("guidid"));
		Assert::IsTrue(cameraAuthObject.getValue(vrm::usermanagement::AuthObjectKey::Url) == std::string("connectedurl"));
		Assert::IsTrue(cameraAuthObject.getValue(vrm::usermanagement::AuthObjectKey::CameraTrackId) == std::string("0"));
		Assert::IsTrue(cameraAuthObject.getValue(vrm::usermanagement::AuthObjectKey::CameraLine) == std::string("0"));
	}

	TEST_METHOD(when_create_a_camera_auth_object_with_xml_element_then_type_is_set_and_value_can_be_retrieved)
	{
		// constructor test

		// arrange
		MockElement mockElement;
		vcslib::xml::Element::s_mock = &mockElement;

		EXPECT_CALL(mockElement, getAttribute(An<const char*>(), An<std::string&>()))
			.Times(3)
			.WillOnce(DoAll(SetArgReferee<1>(std::string("guidid")), Return(true)))
			.WillOnce(DoAll(SetArgReferee<1>(std::string("0")), Return(true)))
			.WillOnce(DoAll(SetArgReferee<1>(std::string("0")), Return(true)));

		EXPECT_CALL(mockElement, getText(An<std::string&>()))
			.WillOnce(DoAll(SetArgReferee<0>(std::string("connectedurl")), Return(true)));

		// act
		vcslib::xml::Element element;
		CameraAuthObject cameraAuthObject(element);

		// assert
		Assert::IsTrue(cameraAuthObject.getType() == vrm::usermanagement::AuthObjectType::Camera);
		Assert::IsTrue(cameraAuthObject.getValue(vrm::usermanagement::AuthObjectKey::Guid) == std::string("guidid"));
		Assert::IsTrue(cameraAuthObject.getValue(vrm::usermanagement::AuthObjectKey::Url) == std::string("connectedurl"));
		Assert::IsTrue(cameraAuthObject.getValue(vrm::usermanagement::AuthObjectKey::CameraTrackId) == std::string("0"));
		Assert::IsTrue(cameraAuthObject.getValue(vrm::usermanagement::AuthObjectKey::CameraLine) == std::string("0"));
	}
};