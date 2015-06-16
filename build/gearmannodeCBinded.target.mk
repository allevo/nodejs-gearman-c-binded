# This file is generated by gyp; do not edit.

TOOLSET := target
TARGET := gearmannodeCBinded
DEFS_Debug := \
	'-DNODE_GYP_MODULE_NAME=gearmannodeCBinded' \
	'-D_LARGEFILE_SOURCE' \
	'-D_FILE_OFFSET_BITS=64' \
	'-DBUILDING_NODE_EXTENSION' \
	'-DDEBUG' \
	'-D_DEBUG'

# Flags passed to all source files.
CFLAGS_Debug := \
	-fPIC \
	-Wall \
	-Wextra \
	-Wno-unused-parameter \
	-pthread \
	-m64 \
	-g \
	-O0

# Flags passed to only C files.
CFLAGS_C_Debug :=

# Flags passed to only C++ files.
CFLAGS_CC_Debug := \
	-fno-rtti \
	-fno-exceptions

INCS_Debug := \
	-I/home/vagrant/.node-gyp/0.10.25/src \
	-I/home/vagrant/.node-gyp/0.10.25/deps/uv/include \
	-I/home/vagrant/.node-gyp/0.10.25/deps/v8/include \
	-I$(srcdir)/node_modules/nan

DEFS_Release := \
	'-DNODE_GYP_MODULE_NAME=gearmannodeCBinded' \
	'-D_LARGEFILE_SOURCE' \
	'-D_FILE_OFFSET_BITS=64' \
	'-DBUILDING_NODE_EXTENSION'

# Flags passed to all source files.
CFLAGS_Release := \
	-fPIC \
	-Wall \
	-Wextra \
	-Wno-unused-parameter \
	-pthread \
	-m64 \
	-O2 \
	-fno-strict-aliasing \
	-fno-tree-vrp \
	-fno-omit-frame-pointer

# Flags passed to only C files.
CFLAGS_C_Release :=

# Flags passed to only C++ files.
CFLAGS_CC_Release := \
	-fno-rtti \
	-fno-exceptions

INCS_Release := \
	-I/home/vagrant/.node-gyp/0.10.25/src \
	-I/home/vagrant/.node-gyp/0.10.25/deps/uv/include \
	-I/home/vagrant/.node-gyp/0.10.25/deps/v8/include \
	-I$(srcdir)/node_modules/nan

OBJS := \
	$(obj).target/$(TARGET)/src/init.o \
	$(obj).target/$(TARGET)/src/GearmanClient.o

# Add to the list of files we specially track dependencies for.
all_deps += $(OBJS)

# CFLAGS et al overrides must be target-local.
# See "Target-specific Variable Values" in the GNU Make manual.
$(OBJS): TOOLSET := $(TOOLSET)
$(OBJS): GYP_CFLAGS := $(DEFS_$(BUILDTYPE)) $(INCS_$(BUILDTYPE))  $(CFLAGS_$(BUILDTYPE)) $(CFLAGS_C_$(BUILDTYPE))
$(OBJS): GYP_CXXFLAGS := $(DEFS_$(BUILDTYPE)) $(INCS_$(BUILDTYPE))  $(CFLAGS_$(BUILDTYPE)) $(CFLAGS_CC_$(BUILDTYPE))

# Suffix rules, putting all outputs into $(obj).

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(srcdir)/%.cc FORCE_DO_CMD
	@$(call do_cmd,cxx,1)

# Try building from generated source, too.

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(obj).$(TOOLSET)/%.cc FORCE_DO_CMD
	@$(call do_cmd,cxx,1)

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(obj)/%.cc FORCE_DO_CMD
	@$(call do_cmd,cxx,1)

# End of this set of suffix rules
### Rules for final target.
LDFLAGS_Debug := \
	-pthread \
	-rdynamic \
	-m64

LDFLAGS_Release := \
	-pthread \
	-rdynamic \
	-m64

LIBS := \
	-lgearman

$(obj).target/gearmannodeCBinded.node: GYP_LDFLAGS := $(LDFLAGS_$(BUILDTYPE))
$(obj).target/gearmannodeCBinded.node: LIBS := $(LIBS)
$(obj).target/gearmannodeCBinded.node: TOOLSET := $(TOOLSET)
$(obj).target/gearmannodeCBinded.node: $(OBJS) FORCE_DO_CMD
	$(call do_cmd,solink_module)

all_deps += $(obj).target/gearmannodeCBinded.node
# Add target alias
.PHONY: gearmannodeCBinded
gearmannodeCBinded: $(builddir)/gearmannodeCBinded.node

# Copy this to the executable output path.
$(builddir)/gearmannodeCBinded.node: TOOLSET := $(TOOLSET)
$(builddir)/gearmannodeCBinded.node: $(obj).target/gearmannodeCBinded.node FORCE_DO_CMD
	$(call do_cmd,copy)

all_deps += $(builddir)/gearmannodeCBinded.node
# Short alias for building this executable.
.PHONY: gearmannodeCBinded.node
gearmannodeCBinded.node: $(obj).target/gearmannodeCBinded.node $(builddir)/gearmannodeCBinded.node

# Add executable to "all" target.
.PHONY: all
all: $(builddir)/gearmannodeCBinded.node

