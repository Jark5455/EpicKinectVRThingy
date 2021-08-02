//
// Created by yashr on 8/1/21.
//

#include <thread>
#include "old/ControllerDrivers/native/DeviceProvider.h"

EVRInitError DeviceProvider::Init(IVRDriverContext *pDriverContext) {
    EVRInitError initError = InitServerDriverContext(pDriverContext);
    if (initError != EVRInitError::VRInitError_None){
        return initError;
    }

    VRDriverLog()->Log("Initializing EpicKinectVRThingy controller");

    std::thread userTracker(startUserTracking);
    std::thread nunchukListener(startNunchukListener);

    while (userdata->getSkeleton().getState() != nite::SKELETON_TRACKED){
        VRDriverLog()->Log("Waiting for NiTE to start tracking");
    }

    rightControllerDriver = new RightControllerDriver();
    leftControllerDriver = new LeftControllerDriver();

    VRServerDriverHost()->TrackedDeviceAdded("EpicKinectVRThingyLeftController", TrackedDeviceClass_Controller, leftControllerDriver);
    VRServerDriverHost()->TrackedDeviceAdded("EpicKinectVRThingyRightController", TrackedDeviceClass_Controller, rightControllerDriver);

    return vr::VRInitError_None;
}

void DeviceProvider::Cleanup() {
    delete leftControllerDriver;
    delete rightControllerDriver;
    leftControllerDriver = nullptr;
    rightControllerDriver = nullptr;

    niteShutdown();
}

const char* const* DeviceProvider::GetInterfaceVersions() {
    return k_InterfaceVersions;
}

void DeviceProvider::RunFrame() {
    leftControllerDriver->RunFrame();
    rightControllerDriver->RunFrame();
}

bool DeviceProvider::ShouldBlockStandbyMode() {
    return false;
}

void DeviceProvider::EnterStandby() {}

void DeviceProvider::LeaveStandby() {}