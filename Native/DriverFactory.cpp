//
// Created by yashr on 8/2/21.
//

#include "DriverFactory.h"
#include <Driver/VRDriver.h>
#include <cstring>

static std::shared_ptr<EpicKinectVRThingy::IVRDriver> driver;

void* HmdDriverFactory(const char* interfaceName, int* returnCode){
    if (std::strcmp(interfaceName, vr::IServerTrackedDeviceProvider_Version) == 0){
        if (!driver){
            driver = std::make_shared<EpicKinectVRThingy::VRDriver>();
        }

        return driver.get();
    }

    if (returnCode){
        *returnCode = vr::VRInitError_Init_InterfaceNotFound;
    }

    return nullptr;
}

std::shared_ptr<EpicKinectVRThingy::IVRDriver> EpicKinectVRThingy::GetDriver() {
    return driver;
}