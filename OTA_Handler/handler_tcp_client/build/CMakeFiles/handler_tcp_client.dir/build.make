# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ota/Documents/handler_tcp_client

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ota/Documents/handler_tcp_client/build

# Include any dependencies generated for this target.
include CMakeFiles/handler_tcp_client.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/handler_tcp_client.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/handler_tcp_client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/handler_tcp_client.dir/flags.make

CMakeFiles/handler_tcp_client.dir/main.cpp.o: CMakeFiles/handler_tcp_client.dir/flags.make
CMakeFiles/handler_tcp_client.dir/main.cpp.o: /home/ota/Documents/handler_tcp_client/main.cpp
CMakeFiles/handler_tcp_client.dir/main.cpp.o: CMakeFiles/handler_tcp_client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ota/Documents/handler_tcp_client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/handler_tcp_client.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/handler_tcp_client.dir/main.cpp.o -MF CMakeFiles/handler_tcp_client.dir/main.cpp.o.d -o CMakeFiles/handler_tcp_client.dir/main.cpp.o -c /home/ota/Documents/handler_tcp_client/main.cpp

CMakeFiles/handler_tcp_client.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/handler_tcp_client.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ota/Documents/handler_tcp_client/main.cpp > CMakeFiles/handler_tcp_client.dir/main.cpp.i

CMakeFiles/handler_tcp_client.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/handler_tcp_client.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ota/Documents/handler_tcp_client/main.cpp -o CMakeFiles/handler_tcp_client.dir/main.cpp.s

# Object files for target handler_tcp_client
handler_tcp_client_OBJECTS = \
"CMakeFiles/handler_tcp_client.dir/main.cpp.o"

# External object files for target handler_tcp_client
handler_tcp_client_EXTERNAL_OBJECTS =

handler_tcp_client: CMakeFiles/handler_tcp_client.dir/main.cpp.o
handler_tcp_client: CMakeFiles/handler_tcp_client.dir/build.make
handler_tcp_client: /home/ota/Documents/install_folder/lib/libCommonAPI.so
handler_tcp_client: /home/ota/Documents/install_folder/lib/libCommonAPI-SomeIP.so
handler_tcp_client: /home/ota/Documents/install_folder/lib/libvsomeip3.so
handler_tcp_client: CMakeFiles/handler_tcp_client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ota/Documents/handler_tcp_client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable handler_tcp_client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/handler_tcp_client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/handler_tcp_client.dir/build: handler_tcp_client
.PHONY : CMakeFiles/handler_tcp_client.dir/build

CMakeFiles/handler_tcp_client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/handler_tcp_client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/handler_tcp_client.dir/clean

CMakeFiles/handler_tcp_client.dir/depend:
	cd /home/ota/Documents/handler_tcp_client/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ota/Documents/handler_tcp_client /home/ota/Documents/handler_tcp_client /home/ota/Documents/handler_tcp_client/build /home/ota/Documents/handler_tcp_client/build /home/ota/Documents/handler_tcp_client/build/CMakeFiles/handler_tcp_client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/handler_tcp_client.dir/depend

