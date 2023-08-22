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


EXECUTE_PROCESS( COMMAND gcc -dumpmachine COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )

if("${ARCHITECTURE}" STREQUAL "arm-linux-gnueabihf")
    set(ARCH_DIR arm_linux_gnueabihf)
elseif("${ARCHITECTURE}" STREQUAL "aarch64-linux-gnu")
    set(ARCH_DIR aarch64_linux_gnu)
else()
    message("Unknown arch")
endif()

file(GLOB ANTILATENCY_LIBS ${CMAKE_SOURCE_DIR}/AntilatencySdk/Bin/Linux/${ARCH_DIR}/*.so)

add_custom_command(TARGET ${PROJECT} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ANTILATENCY_LIBS} $<TARGET_FILE_DIR:AntilatencyCopilotDemo>)

