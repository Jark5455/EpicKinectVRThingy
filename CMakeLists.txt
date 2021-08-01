cmake_minimum_required(VERSION 3.20)
project(untitled)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG -g)
set(CMAKE_CXX_FLAGS_RELEASE -O3)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_library(EpicKinectVRThingy SHARED library.cpp ControllerDrivers/LeftControllerDriver.cpp ControllerDrivers/RightControllerDriver.cpp DeviceFactory.cpp DeviceProvider.cpp)
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