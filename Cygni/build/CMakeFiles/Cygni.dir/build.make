# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/heyanjie/Documents/Projects/TypedCygni/Cygni

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/heyanjie/Documents/Projects/TypedCygni/Cygni/build

# Include any dependencies generated for this target.
include CMakeFiles/Cygni.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Cygni.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Cygni.dir/flags.make

CMakeFiles/Cygni.dir/main.cpp.o: CMakeFiles/Cygni.dir/flags.make
CMakeFiles/Cygni.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/heyanjie/Documents/Projects/TypedCygni/Cygni/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Cygni.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Cygni.dir/main.cpp.o -c /home/heyanjie/Documents/Projects/TypedCygni/Cygni/main.cpp

CMakeFiles/Cygni.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Cygni.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/heyanjie/Documents/Projects/TypedCygni/Cygni/main.cpp > CMakeFiles/Cygni.dir/main.cpp.i

CMakeFiles/Cygni.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Cygni.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/heyanjie/Documents/Projects/TypedCygni/Cygni/main.cpp -o CMakeFiles/Cygni.dir/main.cpp.s

CMakeFiles/Cygni.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/Cygni.dir/main.cpp.o.requires

CMakeFiles/Cygni.dir/main.cpp.o.provides: CMakeFiles/Cygni.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/Cygni.dir/build.make CMakeFiles/Cygni.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/Cygni.dir/main.cpp.o.provides

CMakeFiles/Cygni.dir/main.cpp.o.provides.build: CMakeFiles/Cygni.dir/main.cpp.o


CMakeFiles/Cygni.dir/src/sources/Lexer.cpp.o: CMakeFiles/Cygni.dir/flags.make
CMakeFiles/Cygni.dir/src/sources/Lexer.cpp.o: ../src/sources/Lexer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/heyanjie/Documents/Projects/TypedCygni/Cygni/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Cygni.dir/src/sources/Lexer.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Cygni.dir/src/sources/Lexer.cpp.o -c /home/heyanjie/Documents/Projects/TypedCygni/Cygni/src/sources/Lexer.cpp

CMakeFiles/Cygni.dir/src/sources/Lexer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Cygni.dir/src/sources/Lexer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/heyanjie/Documents/Projects/TypedCygni/Cygni/src/sources/Lexer.cpp > CMakeFiles/Cygni.dir/src/sources/Lexer.cpp.i

CMakeFiles/Cygni.dir/src/sources/Lexer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Cygni.dir/src/sources/Lexer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/heyanjie/Documents/Projects/TypedCygni/Cygni/src/sources/Lexer.cpp -o CMakeFiles/Cygni.dir/src/sources/Lexer.cpp.s

CMakeFiles/Cygni.dir/src/sources/Lexer.cpp.o.requires:

.PHONY : CMakeFiles/Cygni.dir/src/sources/Lexer.cpp.o.requires

CMakeFiles/Cygni.dir/src/sources/Lexer.cpp.o.provides: CMakeFiles/Cygni.dir/src/sources/Lexer.cpp.o.requires
	$(MAKE) -f CMakeFiles/Cygni.dir/build.make CMakeFiles/Cygni.dir/src/sources/Lexer.cpp.o.provides.build
.PHONY : CMakeFiles/Cygni.dir/src/sources/Lexer.cpp.o.provides

CMakeFiles/Cygni.dir/src/sources/Lexer.cpp.o.provides.build: CMakeFiles/Cygni.dir/src/sources/Lexer.cpp.o


CMakeFiles/Cygni.dir/src/sources/Token.cpp.o: CMakeFiles/Cygni.dir/flags.make
CMakeFiles/Cygni.dir/src/sources/Token.cpp.o: ../src/sources/Token.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/heyanjie/Documents/Projects/TypedCygni/Cygni/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Cygni.dir/src/sources/Token.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Cygni.dir/src/sources/Token.cpp.o -c /home/heyanjie/Documents/Projects/TypedCygni/Cygni/src/sources/Token.cpp

CMakeFiles/Cygni.dir/src/sources/Token.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Cygni.dir/src/sources/Token.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/heyanjie/Documents/Projects/TypedCygni/Cygni/src/sources/Token.cpp > CMakeFiles/Cygni.dir/src/sources/Token.cpp.i

CMakeFiles/Cygni.dir/src/sources/Token.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Cygni.dir/src/sources/Token.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/heyanjie/Documents/Projects/TypedCygni/Cygni/src/sources/Token.cpp -o CMakeFiles/Cygni.dir/src/sources/Token.cpp.s

CMakeFiles/Cygni.dir/src/sources/Token.cpp.o.requires:

.PHONY : CMakeFiles/Cygni.dir/src/sources/Token.cpp.o.requires

CMakeFiles/Cygni.dir/src/sources/Token.cpp.o.provides: CMakeFiles/Cygni.dir/src/sources/Token.cpp.o.requires
	$(MAKE) -f CMakeFiles/Cygni.dir/build.make CMakeFiles/Cygni.dir/src/sources/Token.cpp.o.provides.build
.PHONY : CMakeFiles/Cygni.dir/src/sources/Token.cpp.o.provides

CMakeFiles/Cygni.dir/src/sources/Token.cpp.o.provides.build: CMakeFiles/Cygni.dir/src/sources/Token.cpp.o


CMakeFiles/Cygni.dir/src/sources/Utility.cpp.o: CMakeFiles/Cygni.dir/flags.make
CMakeFiles/Cygni.dir/src/sources/Utility.cpp.o: ../src/sources/Utility.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/heyanjie/Documents/Projects/TypedCygni/Cygni/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Cygni.dir/src/sources/Utility.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Cygni.dir/src/sources/Utility.cpp.o -c /home/heyanjie/Documents/Projects/TypedCygni/Cygni/src/sources/Utility.cpp

CMakeFiles/Cygni.dir/src/sources/Utility.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Cygni.dir/src/sources/Utility.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/heyanjie/Documents/Projects/TypedCygni/Cygni/src/sources/Utility.cpp > CMakeFiles/Cygni.dir/src/sources/Utility.cpp.i

CMakeFiles/Cygni.dir/src/sources/Utility.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Cygni.dir/src/sources/Utility.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/heyanjie/Documents/Projects/TypedCygni/Cygni/src/sources/Utility.cpp -o CMakeFiles/Cygni.dir/src/sources/Utility.cpp.s

CMakeFiles/Cygni.dir/src/sources/Utility.cpp.o.requires:

.PHONY : CMakeFiles/Cygni.dir/src/sources/Utility.cpp.o.requires

CMakeFiles/Cygni.dir/src/sources/Utility.cpp.o.provides: CMakeFiles/Cygni.dir/src/sources/Utility.cpp.o.requires
	$(MAKE) -f CMakeFiles/Cygni.dir/build.make CMakeFiles/Cygni.dir/src/sources/Utility.cpp.o.provides.build
.PHONY : CMakeFiles/Cygni.dir/src/sources/Utility.cpp.o.provides

CMakeFiles/Cygni.dir/src/sources/Utility.cpp.o.provides.build: CMakeFiles/Cygni.dir/src/sources/Utility.cpp.o


# Object files for target Cygni
Cygni_OBJECTS = \
"CMakeFiles/Cygni.dir/main.cpp.o" \
"CMakeFiles/Cygni.dir/src/sources/Lexer.cpp.o" \
"CMakeFiles/Cygni.dir/src/sources/Token.cpp.o" \
"CMakeFiles/Cygni.dir/src/sources/Utility.cpp.o"

# External object files for target Cygni
Cygni_EXTERNAL_OBJECTS =

Cygni: CMakeFiles/Cygni.dir/main.cpp.o
Cygni: CMakeFiles/Cygni.dir/src/sources/Lexer.cpp.o
Cygni: CMakeFiles/Cygni.dir/src/sources/Token.cpp.o
Cygni: CMakeFiles/Cygni.dir/src/sources/Utility.cpp.o
Cygni: CMakeFiles/Cygni.dir/build.make
Cygni: CMakeFiles/Cygni.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/heyanjie/Documents/Projects/TypedCygni/Cygni/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable Cygni"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Cygni.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Cygni.dir/build: Cygni

.PHONY : CMakeFiles/Cygni.dir/build

CMakeFiles/Cygni.dir/requires: CMakeFiles/Cygni.dir/main.cpp.o.requires
CMakeFiles/Cygni.dir/requires: CMakeFiles/Cygni.dir/src/sources/Lexer.cpp.o.requires
CMakeFiles/Cygni.dir/requires: CMakeFiles/Cygni.dir/src/sources/Token.cpp.o.requires
CMakeFiles/Cygni.dir/requires: CMakeFiles/Cygni.dir/src/sources/Utility.cpp.o.requires

.PHONY : CMakeFiles/Cygni.dir/requires

CMakeFiles/Cygni.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Cygni.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Cygni.dir/clean

CMakeFiles/Cygni.dir/depend:
	cd /home/heyanjie/Documents/Projects/TypedCygni/Cygni/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/heyanjie/Documents/Projects/TypedCygni/Cygni /home/heyanjie/Documents/Projects/TypedCygni/Cygni /home/heyanjie/Documents/Projects/TypedCygni/Cygni/build /home/heyanjie/Documents/Projects/TypedCygni/Cygni/build /home/heyanjie/Documents/Projects/TypedCygni/Cygni/build/CMakeFiles/Cygni.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Cygni.dir/depend

