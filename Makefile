# make: compila o jogo. make run: compila e executa.
# No PowerShell/Windows, use mingw32-make no lugar de make.
.DEFAULT_GOAL := all

CMAKE ?= cmake
CTEST ?= ctest
BUILD_DIR ?= build
GENERATOR ?= Ninja
JOBS ?= 4
COVERAGE ?= OFF
CMAKE_ARGS ?=
ARGS ?=

ifeq ($(OS),Windows_NT)
EXE_EXT := .exe
else
EXE_EXT :=
endif

GAME_EXE = $(BUILD_DIR)/Bomberman$(EXE_EXT)
TEST_EXE = $(BUILD_DIR)/BombermanTests$(EXE_EXT)

.PHONY: all configure game run test-build test list-tests
all: game

configure:
	$(CMAKE) -S . -B "$(BUILD_DIR)" -G "$(GENERATOR)" -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON -DBOMBERMAN_COVERAGE=$(COVERAGE) $(CMAKE_ARGS)

game: configure
	$(CMAKE) --build "$(BUILD_DIR)" --target Bomberman --parallel $(JOBS)

run: game
	"$(GAME_EXE)" $(ARGS)

test-build: configure
	$(CMAKE) --build "$(BUILD_DIR)" --target BombermanTests --parallel $(JOBS)

test: test-build
	$(CTEST) --test-dir "$(BUILD_DIR)" --output-on-failure

list-tests: test-build
	"$(TEST_EXE)" --list-test-cases
