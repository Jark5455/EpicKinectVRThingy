//
// Created by yashr on 7/29/21.
//

#include <thread>
#include <library.h>
#include <NiteSampleUtilities.h>
#include <NiteCAPI.h>

#include <unistd.h>

using namespace std;

int main(int argc, char** argv)
{
    std::thread userTracker(startUserTracking);
    std::thread nunchukListener(startNunchukListener);

    while (!wasKeyboardHit()) {

        if (userdata->getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPositionConfidence() > .5)
            printf("%s. (%5.2f, %5.2f, %5.2f)\n", "right_hand", userdata->getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition().x, userdata->getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition().y, userdata->getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition().z);

        if (userdata->getSkeleton().getJoint(nite::JOINT_RIGHT_ELBOW).getPositionConfidence() > .5)
            printf("%s. (%5.2f, %5.2f, %5.2f)\n", "right_elbow", userdata->getSkeleton().getJoint(nite::JOINT_RIGHT_ELBOW).getPosition().x, userdata->getSkeleton().getJoint(nite::JOINT_RIGHT_ELBOW).getPosition().y, userdata->getSkeleton().getJoint(nite::JOINT_RIGHT_ELBOW).getPosition().z);

        if (userdata->getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPositionConfidence() > .5)
            printf("%s. (%5.2f, %5.2f, %5.2f)\n", "left_hand", userdata->getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().x, userdata->getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().y, userdata->getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().z);

        if (userdata->getSkeleton().getJoint(nite::JOINT_LEFT_ELBOW).getPositionConfidence() > .5)
            printf("%s. (%5.2f, %5.2f, %5.2f)\n", "left_elbow", userdata->getSkeleton().getJoint(nite::JOINT_LEFT_ELBOW).getPosition().x, userdata->getSkeleton().getJoint(nite::JOINT_LEFT_ELBOW).getPosition().y, userdata->getSkeleton().getJoint(nite::JOINT_LEFT_ELBOW).getPosition().z);

        usleep(33333);
    }

    niteShutdown();
    return 0;
}
