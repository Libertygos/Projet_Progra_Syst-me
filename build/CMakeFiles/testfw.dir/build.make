# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_SOURCE_DIR = /net/cremi/julloison/Bureau/testGit/Projet_Progra_Syst-me

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /net/cremi/julloison/Bureau/testGit/Projet_Progra_Syst-me/build

# Include any dependencies generated for this target.
include CMakeFiles/testfw.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/testfw.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/testfw.dir/flags.make

CMakeFiles/testfw.dir/testfw.c.o: CMakeFiles/testfw.dir/flags.make
CMakeFiles/testfw.dir/testfw.c.o: ../testfw.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/net/cremi/julloison/Bureau/testGit/Projet_Progra_Syst-me/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/testfw.dir/testfw.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/testfw.dir/testfw.c.o   -c /net/cremi/julloison/Bureau/testGit/Projet_Progra_Syst-me/testfw.c

CMakeFiles/testfw.dir/testfw.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/testfw.dir/testfw.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /net/cremi/julloison/Bureau/testGit/Projet_Progra_Syst-me/testfw.c > CMakeFiles/testfw.dir/testfw.c.i

CMakeFiles/testfw.dir/testfw.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/testfw.dir/testfw.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /net/cremi/julloison/Bureau/testGit/Projet_Progra_Syst-me/testfw.c -o CMakeFiles/testfw.dir/testfw.c.s

CMakeFiles/testfw.dir/testfw.c.o.requires:

.PHONY : CMakeFiles/testfw.dir/testfw.c.o.requires

CMakeFiles/testfw.dir/testfw.c.o.provides: CMakeFiles/testfw.dir/testfw.c.o.requires
	$(MAKE) -f CMakeFiles/testfw.dir/build.make CMakeFiles/testfw.dir/testfw.c.o.provides.build
.PHONY : CMakeFiles/testfw.dir/testfw.c.o.provides

CMakeFiles/testfw.dir/testfw.c.o.provides.build: CMakeFiles/testfw.dir/testfw.c.o


# Object files for target testfw
testfw_OBJECTS = \
"CMakeFiles/testfw.dir/testfw.c.o"

# External object files for target testfw
testfw_EXTERNAL_OBJECTS =

libtestfw.a: CMakeFiles/testfw.dir/testfw.c.o
libtestfw.a: CMakeFiles/testfw.dir/build.make
libtestfw.a: CMakeFiles/testfw.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/net/cremi/julloison/Bureau/testGit/Projet_Progra_Syst-me/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libtestfw.a"
	$(CMAKE_COMMAND) -P CMakeFiles/testfw.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/testfw.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/testfw.dir/build: libtestfw.a

.PHONY : CMakeFiles/testfw.dir/build

CMakeFiles/testfw.dir/requires: CMakeFiles/testfw.dir/testfw.c.o.requires

.PHONY : CMakeFiles/testfw.dir/requires

CMakeFiles/testfw.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/testfw.dir/cmake_clean.cmake
.PHONY : CMakeFiles/testfw.dir/clean

CMakeFiles/testfw.dir/depend:
	cd /net/cremi/julloison/Bureau/testGit/Projet_Progra_Syst-me/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /net/cremi/julloison/Bureau/testGit/Projet_Progra_Syst-me /net/cremi/julloison/Bureau/testGit/Projet_Progra_Syst-me /net/cremi/julloison/Bureau/testGit/Projet_Progra_Syst-me/build /net/cremi/julloison/Bureau/testGit/Projet_Progra_Syst-me/build /net/cremi/julloison/Bureau/testGit/Projet_Progra_Syst-me/build/CMakeFiles/testfw.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/testfw.dir/depend

