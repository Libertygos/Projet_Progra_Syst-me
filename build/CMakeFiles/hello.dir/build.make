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
include CMakeFiles/hello.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/hello.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/hello.dir/flags.make

CMakeFiles/hello.dir/hello.c.o: CMakeFiles/hello.dir/flags.make
CMakeFiles/hello.dir/hello.c.o: ../hello.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/net/cremi/julloison/Bureau/testGit/Projet_Progra_Syst-me/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/hello.dir/hello.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/hello.dir/hello.c.o   -c /net/cremi/julloison/Bureau/testGit/Projet_Progra_Syst-me/hello.c

CMakeFiles/hello.dir/hello.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hello.dir/hello.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /net/cremi/julloison/Bureau/testGit/Projet_Progra_Syst-me/hello.c > CMakeFiles/hello.dir/hello.c.i

CMakeFiles/hello.dir/hello.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hello.dir/hello.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /net/cremi/julloison/Bureau/testGit/Projet_Progra_Syst-me/hello.c -o CMakeFiles/hello.dir/hello.c.s

CMakeFiles/hello.dir/hello.c.o.requires:

.PHONY : CMakeFiles/hello.dir/hello.c.o.requires

CMakeFiles/hello.dir/hello.c.o.provides: CMakeFiles/hello.dir/hello.c.o.requires
	$(MAKE) -f CMakeFiles/hello.dir/build.make CMakeFiles/hello.dir/hello.c.o.provides.build
.PHONY : CMakeFiles/hello.dir/hello.c.o.provides

CMakeFiles/hello.dir/hello.c.o.provides.build: CMakeFiles/hello.dir/hello.c.o


# Object files for target hello
hello_OBJECTS = \
"CMakeFiles/hello.dir/hello.c.o"

# External object files for target hello
hello_EXTERNAL_OBJECTS =

hello: CMakeFiles/hello.dir/hello.c.o
hello: CMakeFiles/hello.dir/build.make
hello: libtestfw_main.a
hello: libtestfw.a
hello: CMakeFiles/hello.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/net/cremi/julloison/Bureau/testGit/Projet_Progra_Syst-me/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable hello"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hello.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/hello.dir/build: hello

.PHONY : CMakeFiles/hello.dir/build

CMakeFiles/hello.dir/requires: CMakeFiles/hello.dir/hello.c.o.requires

.PHONY : CMakeFiles/hello.dir/requires

CMakeFiles/hello.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/hello.dir/cmake_clean.cmake
.PHONY : CMakeFiles/hello.dir/clean

CMakeFiles/hello.dir/depend:
	cd /net/cremi/julloison/Bureau/testGit/Projet_Progra_Syst-me/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /net/cremi/julloison/Bureau/testGit/Projet_Progra_Syst-me /net/cremi/julloison/Bureau/testGit/Projet_Progra_Syst-me /net/cremi/julloison/Bureau/testGit/Projet_Progra_Syst-me/build /net/cremi/julloison/Bureau/testGit/Projet_Progra_Syst-me/build /net/cremi/julloison/Bureau/testGit/Projet_Progra_Syst-me/build/CMakeFiles/hello.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/hello.dir/depend

