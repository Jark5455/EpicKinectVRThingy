#pragma once

#include <chrono>
#include <cmath>

#include <thread>
#include <linalg.h>

#include <Driver/IVRDevice.h>
#include <Native/DriverFactory.h>

namespace EpicKinectVRThingy{
    class ControllerDevice : public IVRDevice {
    public:

        enum class Handedness {
            LEFT,
            RIGHT,
            ANY
        };

        ControllerDevice(std::string serial, Handedness handedness = Handedness::ANY);
        ~ControllerDevice() = default;

        virtual std::string GetSerial() override;
        virtual void Update() override;
        virtual vr::TrackedDeviceIndex_t GetDeviceIndex() override;
        virtual DeviceType GetDeviceType() override;
        virtual Handedness GetHandedness();

        virtual vr::EVRInitError Activate(uint32_t unObjectId) override;
        virtual void Deactivate() override;
        virtual void EnterStandby() override;
        virtual void* GetComponent(const char* pchComponentNameAndVersion) override;
        virtual void DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize) override;
        virtual vr::DriverPose_t GetPose() override;

    private:

        std::thread userTracker;
        std::thread nunchukListener;

        vr::TrackedDeviceIndex_t device_index_ = vr::k_unTrackedDeviceIndexInvalid;
        std::string serial_;
        Handedness handedness_;

        vr::DriverPose_t last_pose_;

        bool did_vibrate_ = false;
        float vibrate_anim_state_ = 0.f;

        vr::VRInputComponentHandle_t haptic_component_ = 0;

        vr::VRInputComponentHandle_t c_button_click_component_ = 0;
        vr::VRInputComponentHandle_t c_button_touch_component_ = 0;

        vr::VRInputComponentHandle_t z_button_click_component_ = 0;
        vr::VRInputComponentHandle_t z_button_touch_component_ = 0;

        vr::VRInputComponentHandle_t joystick_x_component_ = 0;
        vr::VRInputComponentHandle_t joystick_y_component_ = 0;
    };
}