cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)

set(PROJECT AntilatencyCopilotDemo)
project(${PROJECT})
find_package(MAVSDK REQUIRED)

add_executable(${PROJECT})
add_subdirectory(Src)

target_include_directories(${PROJECT}
PUBLIC
    ${CMAKE_SOURCE_DIR}/AntilatencySdk/Api
)

target_link_libraries(${PROJECT}
    MAVSDK::mavsdk
    dl stdc++fs pthread
)

file(GLOB ANTILATENCY_LIBS ${CMAKE_SOURCE_DIR}/AntilatencySdk/Bin/Linux/arm_linux_gnueabihf/*.so)

add_custom_command(TARGET ${PROJECT} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ANTILATENCY_LIBS} $<TARGET_FILE_DIR:AntilatencyCopilotDemo>)
