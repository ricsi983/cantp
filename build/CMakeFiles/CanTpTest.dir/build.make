# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/hanaszrichard/projects/cantp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/hanaszrichard/projects/cantp/build

# Include any dependencies generated for this target.
include CMakeFiles/CanTpTest.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/CanTpTest.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/CanTpTest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CanTpTest.dir/flags.make

CMakeFiles/CanTpTest.dir/CanTp.cpp.o: CMakeFiles/CanTpTest.dir/flags.make
CMakeFiles/CanTpTest.dir/CanTp.cpp.o: /Users/hanaszrichard/projects/cantp/CanTp.cpp
CMakeFiles/CanTpTest.dir/CanTp.cpp.o: CMakeFiles/CanTpTest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/hanaszrichard/projects/cantp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/CanTpTest.dir/CanTp.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CanTpTest.dir/CanTp.cpp.o -MF CMakeFiles/CanTpTest.dir/CanTp.cpp.o.d -o CMakeFiles/CanTpTest.dir/CanTp.cpp.o -c /Users/hanaszrichard/projects/cantp/CanTp.cpp

CMakeFiles/CanTpTest.dir/CanTp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CanTpTest.dir/CanTp.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/hanaszrichard/projects/cantp/CanTp.cpp > CMakeFiles/CanTpTest.dir/CanTp.cpp.i

CMakeFiles/CanTpTest.dir/CanTp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CanTpTest.dir/CanTp.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/hanaszrichard/projects/cantp/CanTp.cpp -o CMakeFiles/CanTpTest.dir/CanTp.cpp.s

CMakeFiles/CanTpTest.dir/UtilTest.cpp.o: CMakeFiles/CanTpTest.dir/flags.make
CMakeFiles/CanTpTest.dir/UtilTest.cpp.o: /Users/hanaszrichard/projects/cantp/UtilTest.cpp
CMakeFiles/CanTpTest.dir/UtilTest.cpp.o: CMakeFiles/CanTpTest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/hanaszrichard/projects/cantp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/CanTpTest.dir/UtilTest.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CanTpTest.dir/UtilTest.cpp.o -MF CMakeFiles/CanTpTest.dir/UtilTest.cpp.o.d -o CMakeFiles/CanTpTest.dir/UtilTest.cpp.o -c /Users/hanaszrichard/projects/cantp/UtilTest.cpp

CMakeFiles/CanTpTest.dir/UtilTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CanTpTest.dir/UtilTest.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/hanaszrichard/projects/cantp/UtilTest.cpp > CMakeFiles/CanTpTest.dir/UtilTest.cpp.i

CMakeFiles/CanTpTest.dir/UtilTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CanTpTest.dir/UtilTest.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/hanaszrichard/projects/cantp/UtilTest.cpp -o CMakeFiles/CanTpTest.dir/UtilTest.cpp.s

CMakeFiles/CanTpTest.dir/CanInterface.cpp.o: CMakeFiles/CanTpTest.dir/flags.make
CMakeFiles/CanTpTest.dir/CanInterface.cpp.o: /Users/hanaszrichard/projects/cantp/CanInterface.cpp
CMakeFiles/CanTpTest.dir/CanInterface.cpp.o: CMakeFiles/CanTpTest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/hanaszrichard/projects/cantp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/CanTpTest.dir/CanInterface.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CanTpTest.dir/CanInterface.cpp.o -MF CMakeFiles/CanTpTest.dir/CanInterface.cpp.o.d -o CMakeFiles/CanTpTest.dir/CanInterface.cpp.o -c /Users/hanaszrichard/projects/cantp/CanInterface.cpp

CMakeFiles/CanTpTest.dir/CanInterface.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CanTpTest.dir/CanInterface.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/hanaszrichard/projects/cantp/CanInterface.cpp > CMakeFiles/CanTpTest.dir/CanInterface.cpp.i

CMakeFiles/CanTpTest.dir/CanInterface.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CanTpTest.dir/CanInterface.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/hanaszrichard/projects/cantp/CanInterface.cpp -o CMakeFiles/CanTpTest.dir/CanInterface.cpp.s

CMakeFiles/CanTpTest.dir/CanTpTest.cpp.o: CMakeFiles/CanTpTest.dir/flags.make
CMakeFiles/CanTpTest.dir/CanTpTest.cpp.o: /Users/hanaszrichard/projects/cantp/CanTpTest.cpp
CMakeFiles/CanTpTest.dir/CanTpTest.cpp.o: CMakeFiles/CanTpTest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/hanaszrichard/projects/cantp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/CanTpTest.dir/CanTpTest.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CanTpTest.dir/CanTpTest.cpp.o -MF CMakeFiles/CanTpTest.dir/CanTpTest.cpp.o.d -o CMakeFiles/CanTpTest.dir/CanTpTest.cpp.o -c /Users/hanaszrichard/projects/cantp/CanTpTest.cpp

CMakeFiles/CanTpTest.dir/CanTpTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CanTpTest.dir/CanTpTest.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/hanaszrichard/projects/cantp/CanTpTest.cpp > CMakeFiles/CanTpTest.dir/CanTpTest.cpp.i

CMakeFiles/CanTpTest.dir/CanTpTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CanTpTest.dir/CanTpTest.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/hanaszrichard/projects/cantp/CanTpTest.cpp -o CMakeFiles/CanTpTest.dir/CanTpTest.cpp.s

# Object files for target CanTpTest
CanTpTest_OBJECTS = \
"CMakeFiles/CanTpTest.dir/CanTp.cpp.o" \
"CMakeFiles/CanTpTest.dir/UtilTest.cpp.o" \
"CMakeFiles/CanTpTest.dir/CanInterface.cpp.o" \
"CMakeFiles/CanTpTest.dir/CanTpTest.cpp.o"

# External object files for target CanTpTest
CanTpTest_EXTERNAL_OBJECTS =

CanTpTest: CMakeFiles/CanTpTest.dir/CanTp.cpp.o
CanTpTest: CMakeFiles/CanTpTest.dir/UtilTest.cpp.o
CanTpTest: CMakeFiles/CanTpTest.dir/CanInterface.cpp.o
CanTpTest: CMakeFiles/CanTpTest.dir/CanTpTest.cpp.o
CanTpTest: CMakeFiles/CanTpTest.dir/build.make
CanTpTest: lib/libgtest_main.a
CanTpTest: lib/libgtest.a
CanTpTest: CMakeFiles/CanTpTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/hanaszrichard/projects/cantp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable CanTpTest"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CanTpTest.dir/link.txt --verbose=$(VERBOSE)
	/Applications/CMake.app/Contents/bin/cmake -D TEST_TARGET=CanTpTest -D TEST_EXECUTABLE=/Users/hanaszrichard/projects/cantp/build/CanTpTest -D TEST_EXECUTOR= -D TEST_WORKING_DIR=/Users/hanaszrichard/projects/cantp/build -D TEST_EXTRA_ARGS= -D TEST_PROPERTIES= -D TEST_PREFIX= -D TEST_SUFFIX= -D TEST_FILTER= -D NO_PRETTY_TYPES=FALSE -D NO_PRETTY_VALUES=FALSE -D TEST_LIST=CanTpTest_TESTS -D CTEST_FILE=/Users/hanaszrichard/projects/cantp/build/CanTpTest[1]_tests.cmake -D TEST_DISCOVERY_TIMEOUT=5 -D TEST_XML_OUTPUT_DIR= -P /Applications/CMake.app/Contents/share/cmake-3.26/Modules/GoogleTestAddTests.cmake

# Rule to build all files generated by this target.
CMakeFiles/CanTpTest.dir/build: CanTpTest
.PHONY : CMakeFiles/CanTpTest.dir/build

CMakeFiles/CanTpTest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/CanTpTest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/CanTpTest.dir/clean

CMakeFiles/CanTpTest.dir/depend:
	cd /Users/hanaszrichard/projects/cantp/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/hanaszrichard/projects/cantp /Users/hanaszrichard/projects/cantp /Users/hanaszrichard/projects/cantp/build /Users/hanaszrichard/projects/cantp/build /Users/hanaszrichard/projects/cantp/build/CMakeFiles/CanTpTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/CanTpTest.dir/depend

