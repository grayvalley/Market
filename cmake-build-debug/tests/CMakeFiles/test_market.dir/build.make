# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/clion-2020.2/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /opt/clion-2020.2/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/juha/repos/market

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/juha/repos/market/cmake-build-debug

# Include any dependencies generated for this target.
include tests/CMakeFiles/test_market.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/test_market.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/test_market.dir/flags.make

tests/CMakeFiles/test_market.dir/test_market.cc.o: tests/CMakeFiles/test_market.dir/flags.make
tests/CMakeFiles/test_market.dir/test_market.cc.o: ../tests/test_market.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/juha/repos/market/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/test_market.dir/test_market.cc.o"
	cd /home/juha/repos/market/cmake-build-debug/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_market.dir/test_market.cc.o -c /home/juha/repos/market/tests/test_market.cc

tests/CMakeFiles/test_market.dir/test_market.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_market.dir/test_market.cc.i"
	cd /home/juha/repos/market/cmake-build-debug/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/juha/repos/market/tests/test_market.cc > CMakeFiles/test_market.dir/test_market.cc.i

tests/CMakeFiles/test_market.dir/test_market.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_market.dir/test_market.cc.s"
	cd /home/juha/repos/market/cmake-build-debug/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/juha/repos/market/tests/test_market.cc -o CMakeFiles/test_market.dir/test_market.cc.s

# Object files for target test_market
test_market_OBJECTS = \
"CMakeFiles/test_market.dir/test_market.cc.o"

# External object files for target test_market
test_market_EXTERNAL_OBJECTS =

tests/test_market: tests/CMakeFiles/test_market.dir/test_market.cc.o
tests/test_market: tests/CMakeFiles/test_market.dir/build.make
tests/test_market: tests/CMakeFiles/test_market.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/juha/repos/market/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test_market"
	cd /home/juha/repos/market/cmake-build-debug/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_market.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/test_market.dir/build: tests/test_market

.PHONY : tests/CMakeFiles/test_market.dir/build

tests/CMakeFiles/test_market.dir/clean:
	cd /home/juha/repos/market/cmake-build-debug/tests && $(CMAKE_COMMAND) -P CMakeFiles/test_market.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/test_market.dir/clean

tests/CMakeFiles/test_market.dir/depend:
	cd /home/juha/repos/market/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/juha/repos/market /home/juha/repos/market/tests /home/juha/repos/market/cmake-build-debug /home/juha/repos/market/cmake-build-debug/tests /home/juha/repos/market/cmake-build-debug/tests/CMakeFiles/test_market.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/test_market.dir/depend
