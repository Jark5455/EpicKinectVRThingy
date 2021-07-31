//
// Created by yashr on 7/30/21.
//

#pragma once

#include <openvr_driver.h>
#include <cstdint>
#include <library.h>

#ifndef UNTITLED_RIGHTCONTROLLERDRIVER_H
#define UNTITLED_RIGHTCONTROLLERDRIVER_H

using namespace vr;

class RightControllerDriver : public ITrackedDeviceServerDriver {
public:
    EVRInitError Activate(uint32_t unObjectId);
    void Deactivate();
    void EnterStandby();
    void* GetComponent(const char* pchComponentNameAndVersion);
    void DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize);
    DriverPose_t GetPose();
    void RunFrame();
private:
    uint32_t driverId;
    VRInputComponentHandle_t joystickYHandle;
    VRInputComponentHandle_t joystickXHandle;
    VRInputComponentHandle_t buttonCHandle;
    VRInputComponentHandle_t buttonZHandle;
};

#endif //UNTITLED_RIGHTCONTROLLERDRIVER_H
