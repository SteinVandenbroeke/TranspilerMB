# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = "/mnt/d/onedrive - Uantwerpen/OneDrive - Universiteit Antwerpen/Documenten/Jaar 3 UA/Semester 1/Machines en berekenbaarheid/transpiler"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/mnt/d/onedrive - Uantwerpen/OneDrive - Universiteit Antwerpen/Documenten/Jaar 3 UA/Semester 1/Machines en berekenbaarheid/transpiler/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/transpiler.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/transpiler.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/transpiler.dir/flags.make

CMakeFiles/transpiler.dir/main.cpp.o: CMakeFiles/transpiler.dir/flags.make
CMakeFiles/transpiler.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/d/onedrive - Uantwerpen/OneDrive - Universiteit Antwerpen/Documenten/Jaar 3 UA/Semester 1/Machines en berekenbaarheid/transpiler/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/transpiler.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/transpiler.dir/main.cpp.o -c "/mnt/d/onedrive - Uantwerpen/OneDrive - Universiteit Antwerpen/Documenten/Jaar 3 UA/Semester 1/Machines en berekenbaarheid/transpiler/main.cpp"

CMakeFiles/transpiler.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/transpiler.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/mnt/d/onedrive - Uantwerpen/OneDrive - Universiteit Antwerpen/Documenten/Jaar 3 UA/Semester 1/Machines en berekenbaarheid/transpiler/main.cpp" > CMakeFiles/transpiler.dir/main.cpp.i

CMakeFiles/transpiler.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/transpiler.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/mnt/d/onedrive - Uantwerpen/OneDrive - Universiteit Antwerpen/Documenten/Jaar 3 UA/Semester 1/Machines en berekenbaarheid/transpiler/main.cpp" -o CMakeFiles/transpiler.dir/main.cpp.s

CMakeFiles/transpiler.dir/AstNode.cpp.o: CMakeFiles/transpiler.dir/flags.make
CMakeFiles/transpiler.dir/AstNode.cpp.o: ../AstNode.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/d/onedrive - Uantwerpen/OneDrive - Universiteit Antwerpen/Documenten/Jaar 3 UA/Semester 1/Machines en berekenbaarheid/transpiler/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/transpiler.dir/AstNode.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/transpiler.dir/AstNode.cpp.o -c "/mnt/d/onedrive - Uantwerpen/OneDrive - Universiteit Antwerpen/Documenten/Jaar 3 UA/Semester 1/Machines en berekenbaarheid/transpiler/AstNode.cpp"

CMakeFiles/transpiler.dir/AstNode.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/transpiler.dir/AstNode.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/mnt/d/onedrive - Uantwerpen/OneDrive - Universiteit Antwerpen/Documenten/Jaar 3 UA/Semester 1/Machines en berekenbaarheid/transpiler/AstNode.cpp" > CMakeFiles/transpiler.dir/AstNode.cpp.i

CMakeFiles/transpiler.dir/AstNode.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/transpiler.dir/AstNode.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/mnt/d/onedrive - Uantwerpen/OneDrive - Universiteit Antwerpen/Documenten/Jaar 3 UA/Semester 1/Machines en berekenbaarheid/transpiler/AstNode.cpp" -o CMakeFiles/transpiler.dir/AstNode.cpp.s

CMakeFiles/transpiler.dir/Token.cpp.o: CMakeFiles/transpiler.dir/flags.make
CMakeFiles/transpiler.dir/Token.cpp.o: ../Token.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/d/onedrive - Uantwerpen/OneDrive - Universiteit Antwerpen/Documenten/Jaar 3 UA/Semester 1/Machines en berekenbaarheid/transpiler/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/transpiler.dir/Token.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/transpiler.dir/Token.cpp.o -c "/mnt/d/onedrive - Uantwerpen/OneDrive - Universiteit Antwerpen/Documenten/Jaar 3 UA/Semester 1/Machines en berekenbaarheid/transpiler/Token.cpp"

CMakeFiles/transpiler.dir/Token.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/transpiler.dir/Token.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/mnt/d/onedrive - Uantwerpen/OneDrive - Universiteit Antwerpen/Documenten/Jaar 3 UA/Semester 1/Machines en berekenbaarheid/transpiler/Token.cpp" > CMakeFiles/transpiler.dir/Token.cpp.i

CMakeFiles/transpiler.dir/Token.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/transpiler.dir/Token.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/mnt/d/onedrive - Uantwerpen/OneDrive - Universiteit Antwerpen/Documenten/Jaar 3 UA/Semester 1/Machines en berekenbaarheid/transpiler/Token.cpp" -o CMakeFiles/transpiler.dir/Token.cpp.s

# Object files for target transpiler
transpiler_OBJECTS = \
"CMakeFiles/transpiler.dir/main.cpp.o" \
"CMakeFiles/transpiler.dir/AstNode.cpp.o" \
"CMakeFiles/transpiler.dir/Token.cpp.o"

# External object files for target transpiler
transpiler_EXTERNAL_OBJECTS =

transpiler: CMakeFiles/transpiler.dir/main.cpp.o
transpiler: CMakeFiles/transpiler.dir/AstNode.cpp.o
transpiler: CMakeFiles/transpiler.dir/Token.cpp.o
transpiler: CMakeFiles/transpiler.dir/build.make
transpiler: CMakeFiles/transpiler.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/mnt/d/onedrive - Uantwerpen/OneDrive - Universiteit Antwerpen/Documenten/Jaar 3 UA/Semester 1/Machines en berekenbaarheid/transpiler/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable transpiler"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/transpiler.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/transpiler.dir/build: transpiler

.PHONY : CMakeFiles/transpiler.dir/build

CMakeFiles/transpiler.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/transpiler.dir/cmake_clean.cmake
.PHONY : CMakeFiles/transpiler.dir/clean

CMakeFiles/transpiler.dir/depend:
	cd "/mnt/d/onedrive - Uantwerpen/OneDrive - Universiteit Antwerpen/Documenten/Jaar 3 UA/Semester 1/Machines en berekenbaarheid/transpiler/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/mnt/d/onedrive - Uantwerpen/OneDrive - Universiteit Antwerpen/Documenten/Jaar 3 UA/Semester 1/Machines en berekenbaarheid/transpiler" "/mnt/d/onedrive - Uantwerpen/OneDrive - Universiteit Antwerpen/Documenten/Jaar 3 UA/Semester 1/Machines en berekenbaarheid/transpiler" "/mnt/d/onedrive - Uantwerpen/OneDrive - Universiteit Antwerpen/Documenten/Jaar 3 UA/Semester 1/Machines en berekenbaarheid/transpiler/cmake-build-debug" "/mnt/d/onedrive - Uantwerpen/OneDrive - Universiteit Antwerpen/Documenten/Jaar 3 UA/Semester 1/Machines en berekenbaarheid/transpiler/cmake-build-debug" "/mnt/d/onedrive - Uantwerpen/OneDrive - Universiteit Antwerpen/Documenten/Jaar 3 UA/Semester 1/Machines en berekenbaarheid/transpiler/cmake-build-debug/CMakeFiles/transpiler.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/transpiler.dir/depend

