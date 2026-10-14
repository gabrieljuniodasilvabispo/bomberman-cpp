# No PowerShell/Windows: mingw32-make test. No Linux: make test.
CMAKE ?= cmake
CTEST ?= ctest
BUILD_DIR ?= build
GENERATOR ?= Ninja

.PHONY: all configure test-build test list-tests
all: test-build

configure:
	$(CMAKE) -S . -B "$(BUILD_DIR)" -G "$(GENERATOR)" -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON -DBOMBERMAN_COVERAGE=OFF

test-build: configure
	$(CMAKE) --build "$(BUILD_DIR)" --target BombermanTests --parallel 4

test: test-build
	$(CTEST) --test-dir "$(BUILD_DIR)" --output-on-failure

list-tests: test-build
	"$(BUILD_DIR)/BombermanTests" --list-test-cases
