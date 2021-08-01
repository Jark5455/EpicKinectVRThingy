//
// Created by yashr on 8/1/21.
//
#pragma once

#ifndef UNTITLED_DEVICEPROVIDER_H
#define UNTITLED_DEVICEPROVIDER_H

#include <ControllerDrivers/LeftControllerDriver.h>
#include <ControllerDrivers/RightControllerDriver.h>

#include <openvr_driver.h>

using namespace vr;

class DeviceProvider : public IServerTrackedDeviceProvider {
    public:

        /**
        Initiailze and add your drivers to OpenVR here.
        **/
        EVRInitError Init(IVRDriverContext* pDriverContext);

        /**
        Called right before your driver is unloaded.
        **/
        void Cleanup();

        /**
        Returns version of the openVR interface this driver works with.
        **/
        const char* const* GetInterfaceVersions();

        /**
        Called every frame. Update your drivers here.
        **/
        void RunFrame();

        /**
        Return true if standby mode should be blocked. False otherwise.
        **/
        bool ShouldBlockStandbyMode();

        /**
        Called when OpenVR goes into stand-by mode, so you can tell your devices to go into stand-by mode
        **/
        void EnterStandby();

        /**
        Called when OpenVR leaves stand-by mode.
        **/
        void LeaveStandby();

    private:
        RightControllerDriver* rightControllerDriver;
        LeftControllerDriver* leftControllerDriver;
};

#endif //UNTITLED_DEVICEPROVIDER_H

