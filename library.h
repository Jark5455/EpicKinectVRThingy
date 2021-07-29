#ifndef UNTITLED_LIBRARY_H
#define UNTITLED_LIBRARY_H

#include <NiTE.h>

const nite::SkeletonJoint *head = new nite::SkeletonJoint();
const nite::SkeletonJoint *right_hand = new nite::SkeletonJoint();
const nite::SkeletonJoint *right_elbow = new nite::SkeletonJoint();
const nite::SkeletonJoint *left_hand = new nite::SkeletonJoint();
const nite::SkeletonJoint *left_elbow = new nite::SkeletonJoint();

long joystick_x = 130;
long joystick_y = 130;
bool joystick_c = false;
bool joystick_z = false;

void startUserTracking();
void updateUserState(const nite::UserData& user, unsigned long long ts);

void startNunchukListener();

#endif //UNTITLED_LIBRARY_H
