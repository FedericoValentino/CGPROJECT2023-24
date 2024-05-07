cmake_minimum_required(VERSION 3.5)
project(CGPRJ2023-24)

set(CMAKE_CXX_STANDARD 17)

find_package(Vulkan REQUIRED)

add_subdirectory(src/headers/glfw-3.4)
add_subdirectory(src/headers/glm)


add_executable(Project src/main.cpp)
target_link_libraries(Project glfw)
target_link_libraries(Project glm)
target_link_libraries(Project Vulkan::Vulkan)
target_include_directories(Project PRIVATE src/headers)

message("cmake system name = ${CMAKE_SYSTEM_NAME}")
message("cmake host name = ${CMAKE_HOST_SYSTEM_NAME}")
message("cmake system version = ${CMAKE_SYSTEM_VERSION}")
message("unix = ${UNIX}")
message("win32 = ${WIN32}")