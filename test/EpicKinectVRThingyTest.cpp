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

    while (!wasKeyboardHit()) {
        if (head->getPositionConfidence() > .5)
            printf("%s. (%5.2f, %5.2f, %5.2f)\n", "Head", head->getPosition().x, head->getPosition().y, head->getPosition().z);
        usleep(33333);
    }

    niteShutdown();
    return 0;
}
