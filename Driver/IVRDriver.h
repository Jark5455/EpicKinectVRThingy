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

    public:
        virtual std::vector<std::shared_ptr<IVRDevice>> GetDevices() = 0;
        virtual std::vector<vr::VREvent_t> GetOpenVREvents() = 0;
        virtual std::chrono::milliseconds GetLastFrameTime() = 0;
        virtual bool AddDevice(std::shared_ptr<IVRDevice> device) = 0;
        virtual SettingsValue GetSettingsValue() = 0;
        virtual vr::IVRDriverInput* GetInput() = 0;
        virtual vr::CVRPropertyHelpers* GetProperties() = 0;
        virtual vr::IVRServerDriverHost* GetDriverHost() = 0;
        virtual void Log(std::string msg) = 0;
        inline const char* const* GetInterfaceVersions() override {
            return vr::k_InterfaceVersions;
        }

        virtual ~IVRDriver() {}

    };
}