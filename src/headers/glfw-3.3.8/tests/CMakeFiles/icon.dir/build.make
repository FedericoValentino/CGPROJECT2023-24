# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /snap/cmake/1381/bin/cmake

# The command to remove a file.
RM = /snap/cmake/1381/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/feder34/git/VulkanLearning

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/feder34/git/VulkanLearning

# Include any dependencies generated for this target.
include src/headers/glfw-3.3.8/tests/CMakeFiles/icon.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/headers/glfw-3.3.8/tests/CMakeFiles/icon.dir/compiler_depend.make

# Include the progress variables for this target.
include src/headers/glfw-3.3.8/tests/CMakeFiles/icon.dir/progress.make

# Include the compile flags for this target's objects.
include src/headers/glfw-3.3.8/tests/CMakeFiles/icon.dir/flags.make

src/headers/glfw-3.3.8/tests/CMakeFiles/icon.dir/icon.c.o: src/headers/glfw-3.3.8/tests/CMakeFiles/icon.dir/flags.make
src/headers/glfw-3.3.8/tests/CMakeFiles/icon.dir/icon.c.o: src/headers/glfw-3.3.8/tests/icon.c
src/headers/glfw-3.3.8/tests/CMakeFiles/icon.dir/icon.c.o: src/headers/glfw-3.3.8/tests/CMakeFiles/icon.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/feder34/git/VulkanLearning/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/headers/glfw-3.3.8/tests/CMakeFiles/icon.dir/icon.c.o"
	cd /home/feder34/git/VulkanLearning/src/headers/glfw-3.3.8/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/headers/glfw-3.3.8/tests/CMakeFiles/icon.dir/icon.c.o -MF CMakeFiles/icon.dir/icon.c.o.d -o CMakeFiles/icon.dir/icon.c.o -c /home/feder34/git/VulkanLearning/src/headers/glfw-3.3.8/tests/icon.c

src/headers/glfw-3.3.8/tests/CMakeFiles/icon.dir/icon.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/icon.dir/icon.c.i"
	cd /home/feder34/git/VulkanLearning/src/headers/glfw-3.3.8/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/feder34/git/VulkanLearning/src/headers/glfw-3.3.8/tests/icon.c > CMakeFiles/icon.dir/icon.c.i

src/headers/glfw-3.3.8/tests/CMakeFiles/icon.dir/icon.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/icon.dir/icon.c.s"
	cd /home/feder34/git/VulkanLearning/src/headers/glfw-3.3.8/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/feder34/git/VulkanLearning/src/headers/glfw-3.3.8/tests/icon.c -o CMakeFiles/icon.dir/icon.c.s

src/headers/glfw-3.3.8/tests/CMakeFiles/icon.dir/__/deps/glad_gl.c.o: src/headers/glfw-3.3.8/tests/CMakeFiles/icon.dir/flags.make
src/headers/glfw-3.3.8/tests/CMakeFiles/icon.dir/__/deps/glad_gl.c.o: src/headers/glfw-3.3.8/deps/glad_gl.c
src/headers/glfw-3.3.8/tests/CMakeFiles/icon.dir/__/deps/glad_gl.c.o: src/headers/glfw-3.3.8/tests/CMakeFiles/icon.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/feder34/git/VulkanLearning/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object src/headers/glfw-3.3.8/tests/CMakeFiles/icon.dir/__/deps/glad_gl.c.o"
	cd /home/feder34/git/VulkanLearning/src/headers/glfw-3.3.8/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/headers/glfw-3.3.8/tests/CMakeFiles/icon.dir/__/deps/glad_gl.c.o -MF CMakeFiles/icon.dir/__/deps/glad_gl.c.o.d -o CMakeFiles/icon.dir/__/deps/glad_gl.c.o -c /home/feder34/git/VulkanLearning/src/headers/glfw-3.3.8/deps/glad_gl.c

src/headers/glfw-3.3.8/tests/CMakeFiles/icon.dir/__/deps/glad_gl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/icon.dir/__/deps/glad_gl.c.i"
	cd /home/feder34/git/VulkanLearning/src/headers/glfw-3.3.8/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/feder34/git/VulkanLearning/src/headers/glfw-3.3.8/deps/glad_gl.c > CMakeFiles/icon.dir/__/deps/glad_gl.c.i

src/headers/glfw-3.3.8/tests/CMakeFiles/icon.dir/__/deps/glad_gl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/icon.dir/__/deps/glad_gl.c.s"
	cd /home/feder34/git/VulkanLearning/src/headers/glfw-3.3.8/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/feder34/git/VulkanLearning/src/headers/glfw-3.3.8/deps/glad_gl.c -o CMakeFiles/icon.dir/__/deps/glad_gl.c.s

# Object files for target icon
icon_OBJECTS = \
"CMakeFiles/icon.dir/icon.c.o" \
"CMakeFiles/icon.dir/__/deps/glad_gl.c.o"

# External object files for target icon
icon_EXTERNAL_OBJECTS =

src/headers/glfw-3.3.8/tests/icon: src/headers/glfw-3.3.8/tests/CMakeFiles/icon.dir/icon.c.o
src/headers/glfw-3.3.8/tests/icon: src/headers/glfw-3.3.8/tests/CMakeFiles/icon.dir/__/deps/glad_gl.c.o
src/headers/glfw-3.3.8/tests/icon: src/headers/glfw-3.3.8/tests/CMakeFiles/icon.dir/build.make
src/headers/glfw-3.3.8/tests/icon: src/headers/glfw-3.3.8/src/libglfw3.a
src/headers/glfw-3.3.8/tests/icon: /usr/lib/x86_64-linux-gnu/libm.so
src/headers/glfw-3.3.8/tests/icon: /usr/lib/x86_64-linux-gnu/librt.a
src/headers/glfw-3.3.8/tests/icon: /usr/lib/x86_64-linux-gnu/libm.so
src/headers/glfw-3.3.8/tests/icon: /usr/lib/x86_64-linux-gnu/libX11.so
src/headers/glfw-3.3.8/tests/icon: src/headers/glfw-3.3.8/tests/CMakeFiles/icon.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/feder34/git/VulkanLearning/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable icon"
	cd /home/feder34/git/VulkanLearning/src/headers/glfw-3.3.8/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/icon.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/headers/glfw-3.3.8/tests/CMakeFiles/icon.dir/build: src/headers/glfw-3.3.8/tests/icon
.PHONY : src/headers/glfw-3.3.8/tests/CMakeFiles/icon.dir/build

src/headers/glfw-3.3.8/tests/CMakeFiles/icon.dir/clean:
	cd /home/feder34/git/VulkanLearning/src/headers/glfw-3.3.8/tests && $(CMAKE_COMMAND) -P CMakeFiles/icon.dir/cmake_clean.cmake
.PHONY : src/headers/glfw-3.3.8/tests/CMakeFiles/icon.dir/clean

src/headers/glfw-3.3.8/tests/CMakeFiles/icon.dir/depend:
	cd /home/feder34/git/VulkanLearning && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/feder34/git/VulkanLearning /home/feder34/git/VulkanLearning/src/headers/glfw-3.3.8/tests /home/feder34/git/VulkanLearning /home/feder34/git/VulkanLearning/src/headers/glfw-3.3.8/tests /home/feder34/git/VulkanLearning/src/headers/glfw-3.3.8/tests/CMakeFiles/icon.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : src/headers/glfw-3.3.8/tests/CMakeFiles/icon.dir/depend

