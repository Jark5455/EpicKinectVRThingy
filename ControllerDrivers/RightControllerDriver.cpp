//
// Created by yashr on 7/30/21.
//
#include <thread>

#include "RightControllerDriver.h"

using namespace vr;

EVRInitError RightControllerDriver::Activate(uint32_t unObjectId) {

    std::thread userTracker(startUserTracking);
    std::thread nunchukListener(startNunchukListener);

    driverId = unObjectId;

    PropertyContainerHandle_t  props = VRProperties()->TrackedDeviceToPropertyContainer(driverId);

    VRProperties()->SetStringProperty(props, Prop_InputProfilePath_String, "EpicKinectVRThingy/input/controller_profile.json");
    VRProperties()->SetInt32Property(props, Prop_ControllerRoleHint_Int32, ETrackedControllerRole::TrackedControllerRole_RightHand);

    VRDriverInput()->CreateScalarComponent(props, "/input/joystick/y", &joystickYHandle, EVRScalarType::VRScalarType_Absolute, EVRScalarUnits::VRScalarUnits_NormalizedOneSided);
    VRDriverInput()->CreateScalarComponent(props, "/input/joystick/x", &joystickXHandle, EVRScalarType::VRScalarType_Absolute, EVRScalarUnits::VRScalarUnits_NormalizedOneSided);

    VRDriverInput()->CreateBooleanComponent(props, "/input/buttons/c", &buttonCHandle);
    VRDriverInput()->CreateBooleanComponent(props, "/input/buttons/z", &buttonZHandle);

    return VRInitError_None;
}

DriverPose_t RightControllerDriver::GetPose() {
    DriverPose_t pose = {};

    pose.poseIsValid = false;
    pose.result = TrackingResult_Calibrating_OutOfRange;
    pose.deviceIsConnected = true;

    HmdQuaternion_t quat {};
    quat.w = 1;
    quat.x = 0;
    quat.y = 0;
    quat.z = 0;

    pose.qWorldFromDriverRotation = quat;
    pose.qDriverFromHeadRotation = quat;

    return pose;
}

void RightControllerDriver::RunFrame() {
    VRDriverInput()->UpdateScalarComponent(joystickYHandle, ((float)joystick_y - 30.0f) / 200.0f, 0);
    VRDriverInput()->UpdateScalarComponent(joystickXHandle, ((float)joystick_x - 30.0f) / 200.0f, 0);

    VRDriverInput()->UpdateBooleanComponent(buttonCHandle, joystick_c, 0);
    VRDriverInput()->UpdateBooleanComponent(buttonCHandle, joystick_z, 0);

    DriverPose_t pose = {};

    if (userdata->isNew() || userdata->isLost() || !userdata->isVisible()){
        pose.poseIsValid = false;
    } else {
        pose.poseIsValid = true;
    }

    if (userdata->getSkeleton().getState() == nite::SKELETON_CALIBRATING){
        pose.result = vr::TrackingResult_Calibrating_InProgress;
        pose.poseIsValid = false;
    } else if (userdata->getSkeleton().getState() == nite::SKELETON_CALIBRATION_ERROR_NOT_IN_POSE){
        pose.result = vr::TrackingResult_Calibrating_OutOfRange;
        pose.poseIsValid = false;
    } else if (userdata->getSkeleton().getState() == nite::SKELETON_NONE){
        pose.result = vr::TrackingResult_Uninitialized;
        pose.poseIsValid = false;
    } else if (userdata->getSkeleton().getState() == nite::SKELETON_TRACKED){
        pose.result = vr::TrackingResult_Running_OK;
        pose.poseIsValid = true;
    }

    pose.deviceIsConnected = true;

    HmdQuaternion_t quat {};

    quat.w = 1;
    quat.x = userdata->getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition().x;
    quat.y = userdata->getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition().y;
    quat.z = userdata->getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition().z;

    pose.qWorldFromDriverRotation = quat;
    pose.qDriverFromHeadRotation = quat;

    VRServerDriverHost()->TrackedDevicePoseUpdated(driverId, pose, sizeof(pose));
}

void RightControllerDriver::Deactivate() {
    driverId = k_unTrackedDeviceIndexInvalid;
    niteShutdown();
}

void* RightControllerDriver::GetComponent(const char *pchComponentNameAndVersion) {
    if (strcmp(IVRDriverInput_Version, pchComponentNameAndVersion) == 0){
        return this;
    }

    return nullptr;
}

void RightControllerDriver::EnterStandby() {}

void RightControllerDriver::DebugRequest(const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize) {
    if (unResponseBufferSize >= 1) {
        pchResponseBuffer[0] = 0;
    }
}


