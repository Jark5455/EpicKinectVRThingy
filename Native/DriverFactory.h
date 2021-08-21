//
// Created by yashr on 8/2/21.
//

#pragma once

#include <cstdlib>
#include <memory>

#include <openvr_driver.h>

#include <Driver/IVRDriver.h>

#define HMD_DLL_EXPORT extern "C" __attribute__((visibility("default")))

HMD_DLL_EXPORT
void* HmdDriverFactory(const char* interfaceName, int* returnCode);

namespace EpicKinectVRThingy {
    std::shared_ptr<EpicKinectVRThingy::IVRDriver> GetDriver();
}
