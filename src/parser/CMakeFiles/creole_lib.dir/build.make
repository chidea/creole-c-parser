# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/odroid/creole/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/odroid/creole/src

# Include any dependencies generated for this target.
include parser/CMakeFiles/creole_lib.dir/depend.make

# Include the progress variables for this target.
include parser/CMakeFiles/creole_lib.dir/progress.make

# Include the compile flags for this target's objects.
include parser/CMakeFiles/creole_lib.dir/flags.make

parser/CMakeFiles/creole_lib.dir/cwrapper.o: parser/CMakeFiles/creole_lib.dir/flags.make
parser/CMakeFiles/creole_lib.dir/cwrapper.o: parser/cwrapper.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/odroid/creole/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object parser/CMakeFiles/creole_lib.dir/cwrapper.o"
	cd /home/odroid/creole/src/parser && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/creole_lib.dir/cwrapper.o -c /home/odroid/creole/src/parser/cwrapper.cpp

parser/CMakeFiles/creole_lib.dir/cwrapper.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/creole_lib.dir/cwrapper.i"
	cd /home/odroid/creole/src/parser && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/odroid/creole/src/parser/cwrapper.cpp > CMakeFiles/creole_lib.dir/cwrapper.i

parser/CMakeFiles/creole_lib.dir/cwrapper.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/creole_lib.dir/cwrapper.s"
	cd /home/odroid/creole/src/parser && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/odroid/creole/src/parser/cwrapper.cpp -o CMakeFiles/creole_lib.dir/cwrapper.s

parser/CMakeFiles/creole_lib.dir/cwrapper.o.requires:

.PHONY : parser/CMakeFiles/creole_lib.dir/cwrapper.o.requires

parser/CMakeFiles/creole_lib.dir/cwrapper.o.provides: parser/CMakeFiles/creole_lib.dir/cwrapper.o.requires
	$(MAKE) -f parser/CMakeFiles/creole_lib.dir/build.make parser/CMakeFiles/creole_lib.dir/cwrapper.o.provides.build
.PHONY : parser/CMakeFiles/creole_lib.dir/cwrapper.o.provides

parser/CMakeFiles/creole_lib.dir/cwrapper.o.provides.build: parser/CMakeFiles/creole_lib.dir/cwrapper.o


parser/CMakeFiles/creole_lib.dir/parser.o: parser/CMakeFiles/creole_lib.dir/flags.make
parser/CMakeFiles/creole_lib.dir/parser.o: parser/parser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/odroid/creole/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object parser/CMakeFiles/creole_lib.dir/parser.o"
	cd /home/odroid/creole/src/parser && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/creole_lib.dir/parser.o -c /home/odroid/creole/src/parser/parser.cpp

parser/CMakeFiles/creole_lib.dir/parser.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/creole_lib.dir/parser.i"
	cd /home/odroid/creole/src/parser && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/odroid/creole/src/parser/parser.cpp > CMakeFiles/creole_lib.dir/parser.i

parser/CMakeFiles/creole_lib.dir/parser.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/creole_lib.dir/parser.s"
	cd /home/odroid/creole/src/parser && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/odroid/creole/src/parser/parser.cpp -o CMakeFiles/creole_lib.dir/parser.s

parser/CMakeFiles/creole_lib.dir/parser.o.requires:

.PHONY : parser/CMakeFiles/creole_lib.dir/parser.o.requires

parser/CMakeFiles/creole_lib.dir/parser.o.provides: parser/CMakeFiles/creole_lib.dir/parser.o.requires
	$(MAKE) -f parser/CMakeFiles/creole_lib.dir/build.make parser/CMakeFiles/creole_lib.dir/parser.o.provides.build
.PHONY : parser/CMakeFiles/creole_lib.dir/parser.o.provides

parser/CMakeFiles/creole_lib.dir/parser.o.provides.build: parser/CMakeFiles/creole_lib.dir/parser.o


parser/CMakeFiles/creole_lib.dir/lex.yy.o: parser/CMakeFiles/creole_lib.dir/flags.make
parser/CMakeFiles/creole_lib.dir/lex.yy.o: parser/lex.yy.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/odroid/creole/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object parser/CMakeFiles/creole_lib.dir/lex.yy.o"
	cd /home/odroid/creole/src/parser && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/creole_lib.dir/lex.yy.o -c /home/odroid/creole/src/parser/lex.yy.c

parser/CMakeFiles/creole_lib.dir/lex.yy.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/creole_lib.dir/lex.yy.i"
	cd /home/odroid/creole/src/parser && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/odroid/creole/src/parser/lex.yy.c > CMakeFiles/creole_lib.dir/lex.yy.i

parser/CMakeFiles/creole_lib.dir/lex.yy.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/creole_lib.dir/lex.yy.s"
	cd /home/odroid/creole/src/parser && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/odroid/creole/src/parser/lex.yy.c -o CMakeFiles/creole_lib.dir/lex.yy.s

parser/CMakeFiles/creole_lib.dir/lex.yy.o.requires:

.PHONY : parser/CMakeFiles/creole_lib.dir/lex.yy.o.requires

parser/CMakeFiles/creole_lib.dir/lex.yy.o.provides: parser/CMakeFiles/creole_lib.dir/lex.yy.o.requires
	$(MAKE) -f parser/CMakeFiles/creole_lib.dir/build.make parser/CMakeFiles/creole_lib.dir/lex.yy.o.provides.build
.PHONY : parser/CMakeFiles/creole_lib.dir/lex.yy.o.provides

parser/CMakeFiles/creole_lib.dir/lex.yy.o.provides.build: parser/CMakeFiles/creole_lib.dir/lex.yy.o


parser/CMakeFiles/creole_lib.dir/ConvertUTF.o: parser/CMakeFiles/creole_lib.dir/flags.make
parser/CMakeFiles/creole_lib.dir/ConvertUTF.o: parser/ConvertUTF.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/odroid/creole/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object parser/CMakeFiles/creole_lib.dir/ConvertUTF.o"
	cd /home/odroid/creole/src/parser && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/creole_lib.dir/ConvertUTF.o   -c /home/odroid/creole/src/parser/ConvertUTF.c

parser/CMakeFiles/creole_lib.dir/ConvertUTF.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/creole_lib.dir/ConvertUTF.i"
	cd /home/odroid/creole/src/parser && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/odroid/creole/src/parser/ConvertUTF.c > CMakeFiles/creole_lib.dir/ConvertUTF.i

parser/CMakeFiles/creole_lib.dir/ConvertUTF.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/creole_lib.dir/ConvertUTF.s"
	cd /home/odroid/creole/src/parser && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/odroid/creole/src/parser/ConvertUTF.c -o CMakeFiles/creole_lib.dir/ConvertUTF.s

parser/CMakeFiles/creole_lib.dir/ConvertUTF.o.requires:

.PHONY : parser/CMakeFiles/creole_lib.dir/ConvertUTF.o.requires

parser/CMakeFiles/creole_lib.dir/ConvertUTF.o.provides: parser/CMakeFiles/creole_lib.dir/ConvertUTF.o.requires
	$(MAKE) -f parser/CMakeFiles/creole_lib.dir/build.make parser/CMakeFiles/creole_lib.dir/ConvertUTF.o.provides.build
.PHONY : parser/CMakeFiles/creole_lib.dir/ConvertUTF.o.provides

parser/CMakeFiles/creole_lib.dir/ConvertUTF.o.provides.build: parser/CMakeFiles/creole_lib.dir/ConvertUTF.o


# Object files for target creole_lib
creole_lib_OBJECTS = \
"CMakeFiles/creole_lib.dir/cwrapper.o" \
"CMakeFiles/creole_lib.dir/parser.o" \
"CMakeFiles/creole_lib.dir/lex.yy.o" \
"CMakeFiles/creole_lib.dir/ConvertUTF.o"

# External object files for target creole_lib
creole_lib_EXTERNAL_OBJECTS =

parser/libcreolelib.a: parser/CMakeFiles/creole_lib.dir/cwrapper.o
parser/libcreolelib.a: parser/CMakeFiles/creole_lib.dir/parser.o
parser/libcreolelib.a: parser/CMakeFiles/creole_lib.dir/lex.yy.o
parser/libcreolelib.a: parser/CMakeFiles/creole_lib.dir/ConvertUTF.o
parser/libcreolelib.a: parser/CMakeFiles/creole_lib.dir/build.make
parser/libcreolelib.a: parser/CMakeFiles/creole_lib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/odroid/creole/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX static library libcreolelib.a"
	cd /home/odroid/creole/src/parser && $(CMAKE_COMMAND) -P CMakeFiles/creole_lib.dir/cmake_clean_target.cmake
	cd /home/odroid/creole/src/parser && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/creole_lib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
parser/CMakeFiles/creole_lib.dir/build: parser/libcreolelib.a

.PHONY : parser/CMakeFiles/creole_lib.dir/build

parser/CMakeFiles/creole_lib.dir/requires: parser/CMakeFiles/creole_lib.dir/cwrapper.o.requires
parser/CMakeFiles/creole_lib.dir/requires: parser/CMakeFiles/creole_lib.dir/parser.o.requires
parser/CMakeFiles/creole_lib.dir/requires: parser/CMakeFiles/creole_lib.dir/lex.yy.o.requires
parser/CMakeFiles/creole_lib.dir/requires: parser/CMakeFiles/creole_lib.dir/ConvertUTF.o.requires

.PHONY : parser/CMakeFiles/creole_lib.dir/requires

parser/CMakeFiles/creole_lib.dir/clean:
	cd /home/odroid/creole/src/parser && $(CMAKE_COMMAND) -P CMakeFiles/creole_lib.dir/cmake_clean.cmake
.PHONY : parser/CMakeFiles/creole_lib.dir/clean

parser/CMakeFiles/creole_lib.dir/depend:
	cd /home/odroid/creole/src && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/odroid/creole/src /home/odroid/creole/src/parser /home/odroid/creole/src /home/odroid/creole/src/parser /home/odroid/creole/src/parser/CMakeFiles/creole_lib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : parser/CMakeFiles/creole_lib.dir/depend

