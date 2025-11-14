# SH4ZAM!
#
# KallistiOS-Compatible Makefile
# Copyright (C) 2025 Falco Girgis
#
# Top-level Makefile for use within the KallistiOS
# build environment. You should have already sourced
# your 'environ.sh' file, initializing KOS paths and vars.
#

# Ensure we're working within the KOS environment.
ifeq ($(origin KOS_BASE),undefined)
    $(error KOS_BASE path not set. Source your environ.sh script first!)
endif

# CMake artifact directory
CMAKE_BUILD_DIR := build

################# PUBLIC RULES ##################

# Builds static library only.
lib: $(CMAKE_BUILD_DIR)/libsh4zam.a

# Builds unit test binary.
tests: $(CMAKE_BUILD_DIR)/test/Sh4zamTests.elf

# Builds library + unit test binary.
all: lib tests

# Runs unit tests using KOS's default loader.
run: tests
	${KOS_LOADER} $(CMAKE_BUILD_DIR)/test/Sh4zamTests.elf

# Rebuilds and reruns unit tests with KOS's default loader.
rerun: clean run

# Runs unit tests with flycast.
flycast: tests
	flycast $(CMAKE_BUILD_DIR)/test/Sh4zamTests.elf

# Installs the static library as a KOS addon.
install: lib
	$(MAKE) -C $(CMAKE_BUILD_DIR) install

# Uninstalls the static library from KOS addons.
uninstall:
	rm -rf ${KOS_BASE}/addons/include/dreamcast/sh4zam
	rm -f ${KOS_BASE}/addons/lib/dreamcast/libsh4zam.a

# Reinstalls the static library as a KOS addon.
reinstall: uninstall install

# Deletes all build artifacts.
clean:
	-rm -rf $(CMAKE_BUILD_DIR)

# Cleans artifacts then rebuilds static library.
rebuild: clean lib

# Regenerates Doxygen documentation and opens in the browser.
docs:
	-rm -rf doc/html
	-rm -rf doc/latex
	cd doc && doxygen
	open doc/html/index.html

# Updates the git repository to the latest.
update:
	git checkout master
	git pull origin master

################# PRIVATE RULES #################

# CMake-generated Makefile.
$(CMAKE_BUILD_DIR)/Makefile: CMakeLists.txt
	kos-cmake -S $(<D) -DSHZ_ENABLE_TESTS=on -B $(@D)

# Statically linked library artifact.
.PHONY: $(CMAKE_BUILD_DIR)/libsh4zam.a
$(CMAKE_BUILD_DIR)/libsh4zam.a: $(CMAKE_BUILD_DIR)/Makefile
	$(MAKE) -C $(@D) sh4zam

# Unit test binary artifact.
.PHONY: $(CMAKE_BUILD_DIR)/test/Sh4zamTests.elf
$(CMAKE_BUILD_DIR)/test/Sh4zamTests.elf: $(CMAKE_BUILD_DIR)/Makefile
	$(MAKE) -C $(@D) Sh4zamTests