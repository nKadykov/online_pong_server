cmake_minimum_required(VERSION 3.0)
project(online_pong_server)

set(CMAKE_CXX_STANDART 23)
set(CMAKE_CXX_STANDART_REQUIRED True)

set(CMAKE_TOOLCHANE_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")

include_directories(include)

add_executable(online_pong_server include/game_session.h include/game_server.h src/game_session.cpp src/game_server.cpp src/server.cpp)

find_package(Boost REQUIRED COMPONENTS system)
target_include_directories(online_pong_server PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(online_pong_server PRIVATE Boost::system)