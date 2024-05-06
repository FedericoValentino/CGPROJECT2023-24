cmake_minimum_required(VERSION 3.23.2)
project(CGPROJECT-2023-24)

set(CMAKE_CXX_STANDARD 17)

find_package(Vulkan REQUIRED)

add_subdirectory(src/headers/glfw-3.3.8)
add_subdirectory(src/headers/glm)


add_executable(Project src/main.cpp)
target_link_libraries(Project glfw)
target_link_libraries(Project glm)
target_link_libraries(Project Vulkan::Vulkan)
target_include_directories(Project PRIVATE src/headers)