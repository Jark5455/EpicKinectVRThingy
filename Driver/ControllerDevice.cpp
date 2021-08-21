//
// Created by yashr on 8/2/21.
//

#include "library.h"
#include "ControllerDevice.h"

EpicKinectVRThingy::ControllerDevice::ControllerDevice(std::string serial, Handedness handedness) : serial_(serial), handedness_(handedness) {}

std::string EpicKinectVRThingy::ControllerDevice::GetSerial() {
    return this->serial_;
}

void EpicKinectVRThingy::ControllerDevice::Update() {
    if (this->device_index_ == vr::k_unTrackedDeviceIndexInvalid)
        return;

    auto events = GetDriver()->GetOpenVREvents();
    for (auto event : events) {

        if (event.eventType == vr::EVREventType::VREvent_Input_HapticVibration) {
            if (event.data.hapticVibration.componentHandle == this->haptic_component_) {
                this->did_vibrate_ = true;
            }
        }
    }

    if (this->did_vibrate_) {
        this->vibrate_anim_state_ += (GetDriver()->GetLastFrameTime().count()/1000.f);
        if (this->vibrate_anim_state_ > 1.0f) {
            this->did_vibrate_ = false;
            this->vibrate_anim_state_ = 0.0f;
        }
    }


    auto pose = IVRDevice::MakeDefaultPose();
    auto devices = GetDriver()->GetDevices();

    auto hmd = std::find_if(devices.begin(), devices.end(), [](const std::shared_ptr<IVRDevice>& device_ptr) {return device_ptr->GetDeviceType() == DeviceType::HMD;});
    if (hmd != devices.end()){
        vr::DriverPose_t hmd_pose = (*hmd)->GetPose();

        linalg::vec<float, 3> hmd_position{ (float)hmd_pose.vecPosition[0], (float)hmd_pose.vecPosition[1], hmd_pose.vecPosition[2]};
        linalg::vec<float, 4> hmd_rotation{ (float)hmd_pose.qRotation.x, (float)hmd_pose.qRotation.y, (float)hmd_pose.qRotation.z, (float)hmd_pose.qRotation.w};

        float controller_y;
        float controller_x;

        if (userdata != nullptr){
            if (this->handedness_ == Handedness::LEFT){
                controller_y = userdata->getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().y;
                controller_x = userdata->getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().x;
            }

            if (this->handedness_ == Handedness::RIGHT){
                controller_y = userdata->getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition().y;
                controller_x = userdata->getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition().x;
            }
        } else {
            controller_y = -0.2f + 0.1f * std::sin(8 * M_PI * vibrate_anim_state_);
            controller_x = this->handedness_ == Handedness::LEFT ? -0.2f : (this->handedness_ == Handedness::RIGHT ? 0.2f : 0.f);
        }

        linalg::vec<float, 3> hmd_pose_offset = { controller_x, controller_y, -0.5f };
        hmd_pose_offset = linalg::qrot(hmd_rotation, hmd_pose_offset);
        linalg::vec<float, 3> final_pose = hmd_pose_offset + hmd_position;

        pose.vecPosition[0] = final_pose.x;
        pose.vecPosition[1] = final_pose.y;
        pose.vecPosition[2] = final_pose.z;

        pose.qRotation.w = hmd_rotation.w;
        pose.qRotation.x = hmd_rotation.x;
        pose.qRotation.y = hmd_rotation.y;
        pose.qRotation.z = hmd_rotation.z;
    }

    if (joystick_c){
        GetDriver()->GetInput()->UpdateBooleanComponent(this->c_button_click_component_, true, 0);
        GetDriver()->GetInput()->UpdateBooleanComponent(this->c_button_touch_component_, true, 0);
    } else {
        GetDriver()->GetInput()->UpdateBooleanComponent(this->c_button_click_component_, false, 0);
        GetDriver()->GetInput()->UpdateBooleanComponent(this->c_button_touch_component_, false, 0);
    }

    if (joystick_z){
        GetDriver()->GetInput()->UpdateBooleanComponent(this->z_button_click_component_, true, 0);
        GetDriver()->GetInput()->UpdateBooleanComponent(this->z_button_touch_component_, true, 0);
    } else {
        GetDriver()->GetInput()->UpdateBooleanComponent(this->z_button_click_component_, false, 0);
        GetDriver()->GetInput()->UpdateBooleanComponent(this->z_button_touch_component_, false, 0);
    }

    GetDriver()->GetInput()->UpdateScalarComponent(this->joystick_x_component_, ((float)joystick_x - 30.0f) / 100.0f, 0);
    GetDriver()->GetInput()->UpdateScalarComponent(this->joystick_y_component_, ((float)joystick_y - 30.0f) / 100.0f, 0);

    GetDriver()->GetDriverHost()->TrackedDevicePoseUpdated(this->device_index_, pose, sizeof(vr::DriverPose_t));
    this->last_pose_ = pose;
}

DeviceType EpicKinectVRThingy::ControllerDevice::GetDeviceType() {
    return DeviceType::CONTROLLER;
}

EpicKinectVRThingy::ControllerDevice::Handedness EpicKinectVRThingy::ControllerDevice::GetHandedness() {
    return this->handedness_;
}

vr::TrackedDeviceIndex_t EpicKinectVRThingy::ControllerDevice::GetDeviceIndex() {
    return this->device_index_;
}

vr::EVRInitError EpicKinectVRThingy::ControllerDevice::Activate(uint32_t unObjectId) {
    this->device_index_ = unObjectId;

    auto props = GetDriver()->GetProperties()->TrackedDeviceToPropertyContainer(this->device_index_);

    GetDriver()->GetInput()->CreateBooleanComponent(props, "/input/c/click", &this->c_button_click_component_);
    GetDriver()->GetInput()->CreateBooleanComponent(props, "/input/c/touch", &this->c_button_touch_component_);

    GetDriver()->GetInput()->CreateBooleanComponent(props, "/input/z/click", &this->z_button_click_component_);
    GetDriver()->GetInput()->CreateBooleanComponent(props, "/input/z/touch", &this->z_button_touch_component_);

    GetDriver()->GetInput()->CreateScalarComponent(props, "/input/joystick/x", &this->joystick_x_component_, vr::VRScalarType_Absolute, vr::EVRScalarUnits::VRScalarUnits_NormalizedOneSided);
    GetDriver()->GetInput()->CreateScalarComponent(props, "/input/joystick/y", &this->joystick_y_component_, vr::VRScalarType_Absolute, vr::EVRScalarUnits::VRScalarUnits_NormalizedOneSided);

    GetDriver()->GetProperties()->SetUint64Property(props, vr::Prop_CurrentUniverseId_Uint64, 2);
    GetDriver()->GetProperties()->SetStringProperty(props, vr::Prop_ModelNumber_String, "Jark is the coolest being in the universe");
    GetDriver()->GetProperties()->SetStringProperty(props, vr::Prop_RenderModelName_String, "Jark is undoubtedly the coolest being in the universe");

    if (this->handedness_ == Handedness::LEFT){
        GetDriver()->GetProperties()->SetInt32Property(props, vr::Prop_ControllerRoleHint_Int32, vr::ETrackedControllerRole::TrackedControllerRole_LeftHand);
    } else if (this->handedness_ == Handedness::RIGHT){
        GetDriver()->GetProperties()->SetInt32Property(props, vr::Prop_ControllerRoleHint_Int32, vr::ETrackedControllerRole::TrackedControllerRole_RightHand);
    } else {
        GetDriver()->GetProperties()->SetInt32Property(props, vr::Prop_ControllerRoleHint_Int32, vr::ETrackedControllerRole::TrackedControllerRole_OptOut);
    }

    GetDriver()->GetProperties()->SetStringProperty(props, vr::Prop_InputProfilePath_String, "EpicKinectVRThingy/input/controller_bindings.json");

    std::string controller_handedness_str = this->handedness_ == Handedness::LEFT ? "left" : "right";
    std::string controller_ready_file = "EpicKinectVRThingy/icons/controller_ready_" + controller_handedness_str + ".png";
    std::string controller_not_ready_file = "EpicKinectVRThingy/icons/controller_not_ready_" + controller_handedness_str + ".png";

    GetDriver()->GetProperties()->SetStringProperty(props, vr::Prop_NamedIconPathDeviceReady_String, controller_ready_file.c_str());

    GetDriver()->GetProperties()->SetStringProperty(props, vr::Prop_NamedIconPathDeviceOff_String, controller_not_ready_file.c_str());
    GetDriver()->GetProperties()->SetStringProperty(props, vr::Prop_NamedIconPathDeviceSearching_String, controller_not_ready_file.c_str());
    GetDriver()->GetProperties()->SetStringProperty(props, vr::Prop_NamedIconPathDeviceSearchingAlert_String, controller_not_ready_file.c_str());
    GetDriver()->GetProperties()->SetStringProperty(props, vr::Prop_NamedIconPathDeviceReadyAlert_String, controller_not_ready_file.c_str());
    GetDriver()->GetProperties()->SetStringProperty(props, vr::Prop_NamedIconPathDeviceNotReady_String, controller_not_ready_file.c_str());
    GetDriver()->GetProperties()->SetStringProperty(props, vr::Prop_NamedIconPathDeviceStandby_String, controller_not_ready_file.c_str());
    GetDriver()->GetProperties()->SetStringProperty(props, vr::Prop_NamedIconPathDeviceAlertLow_String, controller_not_ready_file.c_str());

    return vr::EVRInitError::VRInitError_None;
}

void EpicKinectVRThingy::ControllerDevice::Deactivate() {
    this->device_index_ = vr::k_unTrackedDeviceIndexInvalid;
}

void EpicKinectVRThingy::ControllerDevice::EnterStandby() {}

void* EpicKinectVRThingy::ControllerDevice::GetComponent(const char *pchComponentNameAndVersion) {
    return nullptr;
}

void EpicKinectVRThingy::ControllerDevice::DebugRequest(const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize) {
    if (unResponseBufferSize >= 1){
        pchResponseBuffer[0] = 0;
    }
}

vr::DriverPose_t EpicKinectVRThingy::ControllerDevice::GetPose() {
    return last_pose_;
}
