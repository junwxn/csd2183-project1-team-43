# CSD2183 Project 1: Comparing Red-Black and AVL Trees
#
# DO NOT MODIFY THIS FILE. Gradescope replaces it with the original before
# building your submission.
#
#   make           build ./tree (release build, used for the one-minute timing
#                  check)
#   make debug     build ./tree_debug (sanitizers, used for correctness tests)
#   make bench     build one program in build/ for each .cpp file in bench/
#   make clean     delete everything built by this file
#
# Every .cpp file in src/ is compiled, so you never need to edit this file
# when you add source files. Benchmark programs use every .cpp file in src/
# except main.cpp.
#
# The same commands work in a Unix terminal (Linux, macOS, Git Bash, MSYS2) and
# in PowerShell or cmd.exe with MinGW-w64. MinGW-w64 does not support the
# sanitizers, so make debug fails on native Windows; Gradescope still builds
# and tests the debug build.

CXX = g++

RELEASE_FLAGS = -std=c++20 -O2 -DNDEBUG
DEBUG_FLAGS = -std=c++20 -O0 -g -fno-omit-frame-pointer -fsanitize=address,undefined
WARNINGS = -Wall -Wextra
INCLUDES = -Iinclude -Ibench/nanobench

# GNU make sets OS to Windows_NT on Windows, whatever shell invoked it, and the
# Windows compiler always adds .exe to the programs it builds.
ifeq ($(OS),Windows_NT)
  EXEEXT = .exe
else
  EXEEXT =
endif

# Git Bash and MSYS2 set MSYSTEM and provide Unix commands, so the cmd.exe
# commands are used only on Windows without such a shell.
ifeq ($(OS)$(MSYSTEM),Windows_NT)
  DEL = del /Q
  RMDIR = rmdir /S /Q
  MKDIR = if not exist build mkdir build
  NULLDEV = NUL
else
  DEL = rm -f
  RMDIR = rm -rf
  MKDIR = mkdir -p build
  NULLDEV = /dev/null
endif

TREE = tree$(EXEEXT)
TREE_DEBUG = tree_debug$(EXEEXT)

SRCS = $(wildcard src/*.cpp)
LIB_SRCS = $(filter-out src/main.cpp,$(SRCS))
HDRS = $(wildcard include/*.h) $(wildcard src/*.h) $(wildcard src/*.hpp)

BENCH_SRCS = $(wildcard bench/*.cpp)
BENCHES = $(patsubst bench/%.cpp,build/%$(EXEEXT),$(BENCH_SRCS))
NANOBENCH_OBJ = build/nanobench.o

.PHONY: all debug bench clean

all: $(TREE)

$(TREE): $(SRCS) $(HDRS)
	$(CXX) $(RELEASE_FLAGS) $(WARNINGS) $(INCLUDES) $(SRCS) -o $@

debug: $(TREE_DEBUG)

$(TREE_DEBUG): $(SRCS) $(HDRS)
	$(CXX) $(DEBUG_FLAGS) $(WARNINGS) $(INCLUDES) $(SRCS) -o $@

bench: $(BENCHES)

# The nanobench implementation is compiled once and linked into every
# benchmark program.
$(NANOBENCH_OBJ): bench/nanobench/nanobench.cpp bench/nanobench/nanobench.h
	$(MKDIR)
	$(CXX) $(RELEASE_FLAGS) $(INCLUDES) -c $< -o $@

build/%$(EXEEXT): bench/%.cpp $(LIB_SRCS) $(HDRS) $(NANOBENCH_OBJ)
	$(CXX) $(RELEASE_FLAGS) $(WARNINGS) $(INCLUDES) $< $(LIB_SRCS) $(NANOBENCH_OBJ) -o $@

# The leading - keeps make quiet when something was never built.
clean:
	-$(DEL) $(TREE) $(TREE_DEBUG) 2>$(NULLDEV)
	-$(RMDIR) build 2>$(NULLDEV)
	-$(RMDIR) $(TREE_DEBUG).dSYM 2>$(NULLDEV)
