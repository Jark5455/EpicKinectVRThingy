//
// Created by yashr on 8/1/21.
//

#include <DeviceProvider.h>
#include <openvr_driver.h>

#define HMD_DLL_EXPORT extern "C" __attribute__((visibility("default")))

DeviceProvider deviceProvider;

HMD_DLL_EXPORT
void* HmdDriverFactory(const char* interfaceName, int* returnCode) {
    if (strcmp(interfaceName, IServerTrackedDeviceProvider_Version) == 0){
        return &deviceProvider;
    }

    if (returnCode){
        *returnCode = vr::VRInitError_Init_InterfaceNotFound;
    }

    return nullptr;
}