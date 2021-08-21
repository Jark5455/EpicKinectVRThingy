cmake_minimum_required(VERSION 3.20)
project(EpicKinectVRThingy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG -g)
set(CMAKE_CXX_FLAGS_RELEASE -O3)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_library(EpicKinectVRThingy SHARED library.cpp Driver/VRDriver.cpp Driver/VRDriver.h Driver/IVRDriver.h Driver/IVRDevice.h Native/DriverFactory.h Native/DriverFactory.cpp Driver/ControllerDevice.h Driver/ControllerDevice.cpp)
add_executable(EpicKinectVRThingyTest test/EpicKinectVRThingyTest.cpp)

link_directories(/usr/local/lib)
link_directories(/usr/lib)

target_link_libraries(EpicKinectVRThingy PRIVATE NiTE2)
target_link_libraries(EpicKinectVRThingy PRIVATE OpenNI2)
target_link_libraries(EpicKinectVRThingy PRIVATE python3.9)
target_link_libraries(EpicKinectVRThingy PRIVATE openvr_api64)

include_directories(/usr/local/include/libNiTE2/)
include_directories(/usr/local/include/libOpenNI2/)
include_directories(/usr/local/include/libOpenVR/)
include_directories(/usr/include/python3.9/)

include_directories(${CMAKE_SOURCE_DIR})

target_link_libraries(EpicKinectVRThingyTest PRIVATE EpicKinectVRThingy)
target_link_libraries(EpicKinectVRThingyTest PRIVATE NiTE2)
target_link_libraries(EpicKinectVRThingyTest PRIVATE OpenNI2)
target_link_libraries(EpicKinectVRThingyTest PRIVATE pthread)
target_link_libraries(EpicKinectVRThingyTest PRIVATE python3.9)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/NiTE2/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/NiTE2)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/OpenNI2/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/OpenNI2)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/left_controller_profile.json/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/left_controller_profile.json)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/right_controller_profile.json/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/right_controller_profile.json)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/driver.vrdrivermanifest DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/driver.vrdrivermanifest)