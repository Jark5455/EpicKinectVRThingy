//
// Created by yashr on 8/2/21.
//

#include "VRDriver.h"

using namespace EpicKinectVRThingy;

std::vector<std::shared_ptr<IVRDevice>> EpicKinectVRThingy::VRDriver::GetDevices() {
    return std::vector<std::shared_ptr<IVRDevice>>();
}

std::vector<vr::VREvent_t> EpicKinectVRThingy::VRDriver::GetOpenVREvents() {
    return std::vector<vr::VREvent_t>();
}

std::chrono::milliseconds EpicKinectVRThingy::VRDriver::GetLastFrameTime() {
    return std::chrono::milliseconds();
}

bool EpicKinectVRThingy::VRDriver::AddDevice(std::shared_ptr<IVRDevice> device) {
    return false;
}

SettingsValue EpicKinectVRThingy::VRDriver::GetSettingsValue() {
    return EpicKinectVRThingy::SettingsValue();
}

void EpicKinectVRThingy::VRDriver::Log(std::string msg) {
    std::string message_endl = msg + "\n";
    vr::VRDriverLog()->Log(message_endl.c_str());
}

vr::IVRDriverInput *EpicKinectVRThingy::VRDriver::GetInput() {
    return nullptr;
}

vr::CVRPropertyHelpers *EpicKinectVRThingy::VRDriver::GetProperties() {
    return nullptr;
}

vr::IVRServerDriverHost *EpicKinectVRThingy::VRDriver::GetDriverHost() {
    return nullptr;
}

vr::EVRInitError EpicKinectVRThingy::VRDriver::Init(vr::IVRDriverContext *pDriverContext) {
    if (vr::EVRInitError init_error = vr::InitServerDriverContext(pDriverContext); init_error != vr::EVRInitError::VRInitError_None){
        return init_error;
    }

    Log("Starting EpicKinectVRThingy");

    usertracker = std::thread(startUserTracking);
    nunchuk = std::thread(startNunchukListener);
    return vr::VRInitError_Unknown;
}

void EpicKinectVRThingy::VRDriver::Cleanup() {
    usertracker.detach();
    nunchuk.detach();
    niteShutdown();
}

void EpicKinectVRThingy::VRDriver::RunFrame() {

}

bool EpicKinectVRThingy::VRDriver::ShouldBlockStandbyMode() {
    return false;
}

void EpicKinectVRThingy::VRDriver::EnterStandby() {

}

void EpicKinectVRThingy::VRDriver::LeaveStandby() {

}
