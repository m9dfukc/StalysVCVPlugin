# RACK_DIR: Rack source tree two directories above if it has been built
# (libRack present), otherwise the local prebuilt Rack SDK in ./Rack-SDK
# (download from https://vcvrack.com/downloads/, see README).
ifeq ($(wildcard ../../libRack.dylib ../../libRack.so ../../libRack.dll),)
ifneq ($(wildcard Rack-SDK/plugin.mk),)
RACK_DIR ?= Rack-SDK
endif
endif
RACK_DIR ?= ../..

# FLAGS will be passed to both the C and C++ compiler
FLAGS +=
CFLAGS +=
CXXFLAGS +=

# Careful about linking to shared libraries, since you can't assume much about the user's environment and library search path.
# Static libraries are fine, but they should be added to this plugin's build system.
LDFLAGS +=

# Add .cpp files to the build
SOURCES += $(wildcard src/*.cpp)

# Add files to the ZIP package when running `make dist`
# The compiled plugin and "plugin.json" are automatically added.
DISTRIBUTABLES += res
DISTRIBUTABLES += $(wildcard LICENSE*)

# Include the Rack plugin Makefile framework
include $(RACK_DIR)/plugin.mk
