//
// Created by yashr on 7/30/21.
//

#pragma once

#ifndef UNTITLED_RIGHTCONTROLLERDRIVER_H
#define UNTITLED_RIGHTCONTROLLERDRIVER_H

#include <openvr_driver.h>
#include <cstdint>
#include <library.h>

using namespace vr;

class RightControllerDriver : public ITrackedDeviceServerDriver {
public:

    virtual ~RightControllerDriver() = default;

    EVRInitError Activate(uint32_t unObjectId) override;
    void Deactivate() override;
    void EnterStandby() override;
    void* GetComponent(const char* pchComponentNameAndVersion) override;
    void DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize) override;
    DriverPose_t GetPose() override;
    void RunFrame();
private:
    uint32_t driverId;
    VRInputComponentHandle_t joystickYHandle;
    VRInputComponentHandle_t joystickXHandle;
    VRInputComponentHandle_t buttonCHandle;
    VRInputComponentHandle_t buttonZHandle;
};

#endif //UNTITLED_RIGHTCONTROLLERDRIVER_H
