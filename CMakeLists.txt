cmake_minimum_required(VERSION 3.5)
project(CGPRJ2023-24)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Find Vulkan
find_package(Vulkan REQUIRED)

# Conditional compilation based on operating system
if(WIN32)
    # Windows-specific settings
    set(SDL2_PATH "${CMAKE_SOURCE_DIR}/src/headers/SDL2-2.30.5/x86_64-w64-mingw32")
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_PATH}/include)
    link_directories(${SDL2_PATH}/lib)

elseif(APPLE)
    # macOS-specific settings
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})

elseif(UNIX AND NOT APPLE)
    # Linux-specific settings
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})
endif()

# Add subdirectories
add_subdirectory(src/headers/glfw-3.4)
add_subdirectory(src/headers/glm)
add_subdirectory(src/headers/soloud/contrib)

# Define the executable
add_executable(Project
        src/main.cpp
        src/Model/Include/Partita.h
        src/Model/src/Partita.cpp
        src/Model/src/Tiles.cpp
        src/Model/src/Player.cpp
        src/Model/src/Bullet.cpp
        src/Model/Include/Boss.h
        src/Model/src/Enemy.cpp
        src/Model/src/Boss.cpp
        src/Model/Include/Plane.h
        src/Model/src/Plane.cpp
        src/Model/Include/PlaneType.h
        src/Model/src/Constants.cpp
        src/Model/Include/Constants.h
        src/Engine/SoundEngine.hpp
)

# Link libraries
target_link_libraries(Project
        ${SDL2_LIBRARIES}
        soloud
        glfw
        glm
        Vulkan::Vulkan
)

# Include directories
target_include_directories(Project PRIVATE src/headers)

# Set target properties
set_target_properties(Project PROPERTIES COMPILE_OPTIONS -g)
set_target_properties(Project PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Build")

# Print CMake system information
message("cmake system name = ${CMAKE_SYSTEM_NAME}")
message("cmake host name = ${CMAKE_HOST_SYSTEM_NAME}")
message("cmake system version = ${CMAKE_SYSTEM_VERSION}")
message("unix = ${UNIX}")
message("win32 = ${WIN32}")
message("apple = ${APPLE}")


add_executable(NICCOTEST TestingDir/mainNicco.cpp
        src/Model/Include/Partita.h
        src/Model/src/Partita.cpp
        src/Model/src/Tiles.cpp
        src/Model/src/Player.cpp
        src/Model/src/Bullet.cpp
        src/Model/Include/Boss.h
        src/Model/src/Enemy.cpp
        src/Model/src/Boss.cpp
        src/Model/Include/PlaneType.h)

target_link_libraries(NICCOTEST glfw)
target_link_libraries(NICCOTEST glm)
target_link_libraries(NICCOTEST Vulkan::Vulkan)
target_include_directories(NICCOTEST PRIVATE src/headers)


add_executable(PUDDUHTEST TestingDir/mainHenry.cpp
        src/Model/Include/Partita.h
        src/Model/src/Partita.cpp
        src/Model/src/Tiles.cpp
        src/Model/src/Player.cpp
        src/Model/src/Bullet.cpp
        src/Model/Include/Boss.h
        src/Model/src/Enemy.cpp
        src/Model/src/Boss.cpp
        src/Model/Include/Plane.h
        src/Model/src/Plane.cpp
        src/Model/Include/PlaneType.h)
target_link_libraries(PUDDUHTEST glfw)
target_link_libraries(PUDDUHTEST glm)
target_link_libraries(PUDDUHTEST Vulkan::Vulkan)
target_include_directories(PUDDUHTEST PRIVATE src/headers)

add_executable(FEDERTEST TestingDir/mainFede.cpp
        src/Model/src/Plane.cpp
        src/Model/Include/Plane.h
        src/Model/Include/PlaneBuilder.hpp
        src/Model/Include/PlaneType.h)
target_link_libraries(FEDERTEST glfw)
target_link_libraries(FEDERTEST glm)
target_link_libraries(FEDERTEST Vulkan::Vulkan)
target_include_directories(FEDERTEST PRIVATE src/headers)