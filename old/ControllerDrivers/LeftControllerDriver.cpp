//
// Created by yashr on 7/30/21.
//

#include "LeftControllerDriver.h"

EVRInitError LeftControllerDriver::Activate(uint32_t unObjectId) {
    driverId = unObjectId;

    PropertyContainerHandle_t  props = VRProperties()->TrackedDeviceToPropertyContainer(driverId);

    VRProperties()->SetStringProperty(props, Prop_InputProfilePath_String, "EpicKinectVRThingy/input/left_controller_profile.json");
    VRProperties()->SetInt32Property(props, Prop_ControllerRoleHint_Int32, ETrackedControllerRole::TrackedControllerRole_LeftHand);

    return VRInitError_None;
}

DriverPose_t LeftControllerDriver::GetPose() {
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

void LeftControllerDriver::RunFrame() {
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
    quat.x = userdata->getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().x;
    quat.y = userdata->getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().y;
    quat.z = userdata->getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().z;

    pose.qWorldFromDriverRotation = quat;
    pose.qDriverFromHeadRotation = quat;

    VRServerDriverHost()->TrackedDevicePoseUpdated(driverId, pose, sizeof(pose));
}

void LeftControllerDriver::Deactivate() {
    driverId = k_unTrackedDeviceIndexInvalid;
}

void* LeftControllerDriver::GetComponent(const char *pchComponentNameAndVersion) {
    if (strcmp(IVRDriverInput_Version, pchComponentNameAndVersion) == 0){
        return this;
    }

    return nullptr;
}

void LeftControllerDriver::EnterStandby() {}

void LeftControllerDriver::DebugRequest(const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize) {
    if (unResponseBufferSize >= 1) {
        pchResponseBuffer[0] = 0;
    }
}