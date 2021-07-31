#pragma once

#ifndef UNTITLED_LIBRARY_H
#define UNTITLED_LIBRARY_H

#include <NiTE.h>

extern const nite::UserData *userdata;

extern long joystick_x;
extern long joystick_y;
extern bool joystick_c;
extern bool joystick_z;

[[noreturn]] void startUserTracking();
void updateUserState(const nite::UserData& user, unsigned long long ts);
[[noreturn]] void startNunchukListener();

#endif //UNTITLED_LIBRARY_H
