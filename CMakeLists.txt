#--------------------------------------------------------
# The CMakeLists.txt for: iPX4
# Author(s): David Battle, Mission Systems Pty Ltd
# Based on iArduSubComms by Muthukumaran Chandrasekaran
#--------------------------------------------------------
cmake_minimum_required(VERSION 3.5)

project(moos-ipx4)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $ENV{BIN_HOME})

find_package(MOOS 10.0)

set(SRC
    main.cpp
    PX4.cpp
    PX4_Info.cpp)

add_executable(iPX4 ${SRC})
message("MOOS_LIBRARIES:   " ${MOOS_DIR})


target_include_directories(iPX4 PUBLIC
    ./
    ${MOOS_DIR}/../../../include/ivp
    $ENV{MAVLINK_HOME}/include/common
    ${MOOS_INCLUDE_DIRS})

target_link_libraries(iPX4
   ${MOOS_DIR}/../../../lib/libmbutil.a
   ${MOOS_LIBRARIES}
   boost_system)
