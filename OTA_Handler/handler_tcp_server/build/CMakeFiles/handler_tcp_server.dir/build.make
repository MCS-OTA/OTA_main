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
CMAKE_SOURCE_DIR = /home/ota/Documents/handler_tcp_server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ota/Documents/handler_tcp_server/build

# Include any dependencies generated for this target.
include CMakeFiles/handler_tcp_server.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/handler_tcp_server.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/handler_tcp_server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/handler_tcp_server.dir/flags.make

CMakeFiles/handler_tcp_server.dir/main.cpp.o: CMakeFiles/handler_tcp_server.dir/flags.make
CMakeFiles/handler_tcp_server.dir/main.cpp.o: /home/ota/Documents/handler_tcp_server/main.cpp
CMakeFiles/handler_tcp_server.dir/main.cpp.o: CMakeFiles/handler_tcp_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ota/Documents/handler_tcp_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/handler_tcp_server.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/handler_tcp_server.dir/main.cpp.o -MF CMakeFiles/handler_tcp_server.dir/main.cpp.o.d -o CMakeFiles/handler_tcp_server.dir/main.cpp.o -c /home/ota/Documents/handler_tcp_server/main.cpp

CMakeFiles/handler_tcp_server.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/handler_tcp_server.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ota/Documents/handler_tcp_server/main.cpp > CMakeFiles/handler_tcp_server.dir/main.cpp.i

CMakeFiles/handler_tcp_server.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/handler_tcp_server.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ota/Documents/handler_tcp_server/main.cpp -o CMakeFiles/handler_tcp_server.dir/main.cpp.s

CMakeFiles/handler_tcp_server.dir/handlerStubImpl.cpp.o: CMakeFiles/handler_tcp_server.dir/flags.make
CMakeFiles/handler_tcp_server.dir/handlerStubImpl.cpp.o: /home/ota/Documents/handler_tcp_server/handlerStubImpl.cpp
CMakeFiles/handler_tcp_server.dir/handlerStubImpl.cpp.o: CMakeFiles/handler_tcp_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ota/Documents/handler_tcp_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/handler_tcp_server.dir/handlerStubImpl.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/handler_tcp_server.dir/handlerStubImpl.cpp.o -MF CMakeFiles/handler_tcp_server.dir/handlerStubImpl.cpp.o.d -o CMakeFiles/handler_tcp_server.dir/handlerStubImpl.cpp.o -c /home/ota/Documents/handler_tcp_server/handlerStubImpl.cpp

CMakeFiles/handler_tcp_server.dir/handlerStubImpl.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/handler_tcp_server.dir/handlerStubImpl.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ota/Documents/handler_tcp_server/handlerStubImpl.cpp > CMakeFiles/handler_tcp_server.dir/handlerStubImpl.cpp.i

CMakeFiles/handler_tcp_server.dir/handlerStubImpl.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/handler_tcp_server.dir/handlerStubImpl.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ota/Documents/handler_tcp_server/handlerStubImpl.cpp -o CMakeFiles/handler_tcp_server.dir/handlerStubImpl.cpp.s

# Object files for target handler_tcp_server
handler_tcp_server_OBJECTS = \
"CMakeFiles/handler_tcp_server.dir/main.cpp.o" \
"CMakeFiles/handler_tcp_server.dir/handlerStubImpl.cpp.o"

# External object files for target handler_tcp_server
handler_tcp_server_EXTERNAL_OBJECTS =

handler_tcp_server: CMakeFiles/handler_tcp_server.dir/main.cpp.o
handler_tcp_server: CMakeFiles/handler_tcp_server.dir/handlerStubImpl.cpp.o
handler_tcp_server: CMakeFiles/handler_tcp_server.dir/build.make
handler_tcp_server: /home/ota/Documents/install_folder/lib/libCommonAPI.so
handler_tcp_server: /home/ota/Documents/install_folder/lib/libCommonAPI-SomeIP.so
handler_tcp_server: /home/ota/Documents/install_folder/lib/libvsomeip3.so
handler_tcp_server: CMakeFiles/handler_tcp_server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ota/Documents/handler_tcp_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable handler_tcp_server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/handler_tcp_server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/handler_tcp_server.dir/build: handler_tcp_server
.PHONY : CMakeFiles/handler_tcp_server.dir/build

CMakeFiles/handler_tcp_server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/handler_tcp_server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/handler_tcp_server.dir/clean

CMakeFiles/handler_tcp_server.dir/depend:
	cd /home/ota/Documents/handler_tcp_server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ota/Documents/handler_tcp_server /home/ota/Documents/handler_tcp_server /home/ota/Documents/handler_tcp_server/build /home/ota/Documents/handler_tcp_server/build /home/ota/Documents/handler_tcp_server/build/CMakeFiles/handler_tcp_server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/handler_tcp_server.dir/depend

