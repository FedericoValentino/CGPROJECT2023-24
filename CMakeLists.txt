cmake_minimum_required(VERSION 3.5)
project(CGPRJ2023-24)

set(CMAKE_CXX_STANDARD 17)

find_package(Vulkan REQUIRED)

add_subdirectory(src/headers/glfw-3.4)
add_subdirectory(src/headers/glm)


add_executable(Project src/main.cpp
        src/Model/Include/Partita.h
        src/Model/src/Partita.cpp
        src/Model/src/Tiles.cpp
        src/Model/src/Player.cpp
        src/Model/src/Bullet.cpp
        src/Model/Include/Boss.h
        src/Model/src/Enemy.cpp
        src/Model/src/Boss.cpp)
target_link_libraries(Project glfw)
target_link_libraries(Project glm)
target_link_libraries(Project Vulkan::Vulkan)
target_include_directories(Project PRIVATE src/headers)

message("cmake system name = ${CMAKE_SYSTEM_NAME}")
message("cmake host name = ${CMAKE_HOST_SYSTEM_NAME}")
message("cmake system version = ${CMAKE_SYSTEM_VERSION}")
message("unix = ${UNIX}")
message("win32 = ${WIN32}")


add_executable(NICCOTEST TestingDir/mainNicco.cpp
        src/Model/Include/Partita.h
        src/Model/src/Partita.cpp
        src/Model/src/Tiles.cpp
        src/Model/src/Player.cpp
        src/Model/src/Bullet.cpp
        src/Model/Include/Boss.h
        src/Model/src/Enemy.cpp
        src/Model/src/Boss.cpp)

target_link_libraries(NICCOTEST glfw)
target_link_libraries(NICCOTEST glm)
target_link_libraries(NICCOTEST Vulkan::Vulkan)
target_include_directories(NICCOTEST PRIVATE src/headers)


add_executable(PUDDUHTEST TestingDir/mainHenry.cpp)
target_link_libraries(PUDDUHTEST glfw)
target_link_libraries(PUDDUHTEST glm)
target_link_libraries(PUDDUHTEST Vulkan::Vulkan)
target_include_directories(PUDDUHTEST PRIVATE src/headers)

add_executable(FEDERTEST TestingDir/mainFede.cpp
        src/Model/src/Plane.cpp
        src/Model/Include/Plane.h)
target_link_libraries(FEDERTEST glfw)
target_link_libraries(FEDERTEST glm)
target_link_libraries(FEDERTEST Vulkan::Vulkan)
target_include_directories(FEDERTEST PRIVATE src/headers)