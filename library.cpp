#include "library.h"

#include <python3.9/Python.h>
#include <cstdlib>

using namespace nite;

#define MAX_USERS 10
bool g_visibleUsers[MAX_USERS] = {false};
nite::SkeletonState g_skeletonStates[MAX_USERS] = {nite::SKELETON_NONE};

#define USER_MESSAGE(msg) \
{printf("[%08llu] User #%d:\t%s\n",ts, user.getId(),msg);}

void updateUserState(const nite::UserData& user, unsigned long long ts)
{
    if (user.isNew())
        USER_MESSAGE("New")
    else if (user.isVisible() && !g_visibleUsers[user.getId()])
        USER_MESSAGE("Visible")
    else if (!user.isVisible() && g_visibleUsers[user.getId()])
        USER_MESSAGE("Out of Scene")
    else if (user.isLost())
        USER_MESSAGE("Lost")

    g_visibleUsers[user.getId()] = user.isVisible();

    if(g_skeletonStates[user.getId()] != user.getSkeleton().getState())
    {
        switch(g_skeletonStates[user.getId()] = user.getSkeleton().getState())
        {
        case nite::SKELETON_NONE:
            USER_MESSAGE("Stopped tracking.")
            break;
        case nite::SKELETON_CALIBRATING:
            USER_MESSAGE("Calibrating...")
            break;
        case nite::SKELETON_TRACKED:
            USER_MESSAGE("Tracking!")
            break;
        case nite::SKELETON_CALIBRATION_ERROR_NOT_IN_POSE:
        case nite::SKELETON_CALIBRATION_ERROR_HANDS:
        case nite::SKELETON_CALIBRATION_ERROR_LEGS:
        case nite::SKELETON_CALIBRATION_ERROR_HEAD:
        case nite::SKELETON_CALIBRATION_ERROR_TORSO:
            USER_MESSAGE("Calibration Failed... :-|")
            break;
        }
    }
}

void startUserTracking() {
    UserTracker userTracker;
    Status niteRc;

    NiTE::initialize();

    niteRc = userTracker.create();
    if (niteRc != STATUS_OK){
        printf("Couldn't create user tracker\n");
        exit(3);
    }

    printf("\nStart moving around to get detected...\n(PSI pose may be required for skeleton calibration, depending on the configuration)\n");

    UserTrackerFrameRef userTrackerFrame;
    while (true){
        niteRc = userTracker.readFrame(&userTrackerFrame);
        if (niteRc != STATUS_OK){
            printf("Failed to get next frame\n");
            continue;
        }

        const nite::Array<UserData>& users = userTrackerFrame.getUsers();

        if (users.getSize() > 0){

            if (users.getSize() > 1){
                printf("More than 1 user detected. Tracking user 0\n");
            }

            for (int i = 0; i < users.getSize(); ++i){
                const UserData user = users[0];

                updateUserState(user, userTrackerFrame.getTimestamp());

                if (user.isNew()){
                    userTracker.startSkeletonTracking(user.getId());
                } else if (user.getSkeleton().getState() == SKELETON_TRACKED){
                    head = &user.getSkeleton().getJoint(JOINT_HEAD);
                    right_hand = &user.getSkeleton().getJoint(JOINT_RIGHT_HAND);
                    right_elbow = &user.getSkeleton().getJoint(JOINT_RIGHT_ELBOW);
                    left_hand = &user.getSkeleton().getJoint(JOINT_LEFT_HAND);
                    left_elbow = &user.getSkeleton().getJoint(JOINT_LEFT_ELBOW);
                }
            }
        }
    }
}

void startNunchukListener(){
    setenv("BLINKA_MCP2221", "1", 1);

    PyObject *board = PyImport_Import(PyUnicode_FromString("board"));
    PyObject *adafruit_nunchuk = PyImport_Import(PyUnicode_FromString("adafruit_nunchuk"));

    PyObject *i2c = PyObject_CallMethodNoArgs(board, PyUnicode_FromString("I2C"));
    PyObject *nc = PyObject_CallMethodOneArg(adafruit_nunchuk, PyUnicode_FromString("Nunchuk"), i2c);

    while (true){
        PyObject *joystick = PyObject_GetAttr(nc, PyUnicode_FromString("joystick"));

        joystick_x = PyLong_AsLong(PyTuple_GetItem(joystick, PyLong_AsSsize_t(PyLong_FromLong((long)0))));
        joystick_y = PyLong_AsLong(PyTuple_GetItem(joystick, PyLong_AsSsize_t(PyLong_FromLong((long)1))));

    }

}
