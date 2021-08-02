//
// Created by yashr on 8/2/21.
//

#pragma once

#include <memory>
#include <vector>
#include <chrono>

#include <Driver/IVRDevice.h>

namespace EpicKinectVRThingy {
    typedef std::variant<std::monostate, std::string, int, float, bool> SettingsValue;

    class IVRDriver : protected  vr::IServerTrackedDeviceProvider{

        virtual std::vector<std::shared_ptr<IVRDevice>> GetDevices() = 0;
        virtual std::vector<vr::VREvent_t> GetOpenVREvents() = 0;

    };
}