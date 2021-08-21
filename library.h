#pragma once

#include <NiTE.h>

extern const nite::UserData *userdata;

extern long joystick_x;
extern long joystick_y;
extern bool joystick_c;
extern bool joystick_z;

[[noreturn]] void startUserTracking();
[[noreturn]] void startNunchukListener();
