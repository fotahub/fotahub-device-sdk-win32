KERNEL = $(shell uname -s)
MACHINE = $(shell uname -m)

ifndef HOST
ifeq ($(OS),Windows_NT)
HOST = Win32
else
ifeq ($(KERNEL),Linux)
HOST = Linux
endif
ifeq ($(KERNEL),Darwin)
HOST = MacOSX
endif
endif
endif

TARGET ?= $(HOST)
FLAVOR ?= debug

TARGET_INFIX = $(call toLowerCase,$(TARGET))
FLAVOR_INFIX = $(call toLowerCase,$(FLAVOR))

VERBOSE ?= 
ifdef VERBOSE
Q = 
else
Q = @
endif

vecho = @echo
replaceDir = $(addprefix $1/,$(notdir $2))
parentDirs = $(sort $(patsubst %/,%,$(dir $1)))
toUpperCase = $(shell echo $1 | tr a-z A-Z)
toLowerCase = $(shell echo $1 | tr A-Z a-z)

ifeq ($(HOST),$(TARGET))
export CC = gcc
export LD = $(CC)
TARGET_TOOLCHAIN_AVAILABLE = true
endif

BUILDROOT ?= 
OROOT ?= $(if $(BUILDROOT),$(BUILDROOT)/$(TARGET_INFIX)/$(FLAVOR_INFIX),build/$(TARGET_INFIX)/$(FLAVOR_INFIX))
OBJODIR ?= $(OROOT)/obj
BINODIR ?= $(OROOT)/bin

CFLAGS = -std=c99
INCDIRS = 
LDFLAGS = 
LIBDIRS = 

ifeq ($(FLAVOR),debug)
CFLAGS += -g
endif

export FOTAHUB_INCDIR ?= $(PREFIX)/include
export PREFIX ?= $(abspath $(dir $(firstword $(MAKEFILE_LIST))))/../..
export FOTAHUB_SRCDIR ?=
export FOTAHUB_LIBDIR ?= $(PREFIX)/lib

#  
# Begin DemoProduct Executable
#  

SUPPORTED_TARGETS_DemoProduct = Win32

# => 'true' if SUPPORTED_TARGETS contains 'any', undefined otherwise
FULLY_PORTABLE_DemoProduct = $(if $(filter any,$(SUPPORTED_TARGETS_DemoProduct)),true,)
# => 'true' if FULLY_PORTABLE is 'true' or SUPPORTED_TARGETS contains more than one value including 'Win32', undefined otherwise
PORTABLE_DemoProduct = $(if $(or $(FULLY_PORTABLE_DemoProduct),$(and $(filter-out 0 1,$(words $(SUPPORTED_TARGETS_DemoProduct))),$(filter Win32,$(SUPPORTED_TARGETS_DemoProduct)))),true,)
# => 'true' if SUPPORTED_TARGETS contains a single value different from 'any', undefined otherwise
TARGET_SPECIFIC_DemoProduct = $(if $(and $(filter-out any,$(SUPPORTED_TARGETS_DemoProduct)),$(filter 1,$(words $(SUPPORTED_TARGETS_DemoProduct)))),true,)
# => 'true' if SUPPORTED_TARGETS contains 'any' or TARGET, undefined otherwise
MATCHES_TARGET_DemoProduct = $(if $(or $(FULLY_PORTABLE_DemoProduct),$(filter $(TARGET),$(SUPPORTED_TARGETS_DemoProduct))),true,)
# => 'true' if MATCHES_TARGET and TARGET_TOOLCHAIN_AVAILABLE are 'true', undefined otherwise
BUILDABLE_DemoProduct = $(if $(and $(MATCHES_TARGET_DemoProduct),$(TARGET_TOOLCHAIN_AVAILABLE)),true,)

EXPFX_DemoProduct = ./
EXAFKT_DemoProduct = $(shell sed -n -e 's@^\#define DEMO_PRODUCT_NAME "\(.\+\)"@\1@p' DemoProductInfo.h | sed -e 's@\s@@g')$(if $(UPDATE),Update,)
EXEXT_DemoProduct = .exe

ALL_CFLAGS_DemoProduct = $(CFLAGS) -DWINVER=0x0601 -D_WIN32_WINNT=0x0601

LIBS_DemoProduct = fotahub
LIBDIRS_DemoProduct = $(FOTAHUB_LIBDIR)
EXT_LIBS_DemoProduct = mbedtls mbedx509 mbedcrypto ws2_32 winmm
EXT_LIBDIRS_DemoProduct = $(PREFIX)/third-party/mbedtls/lib $(PREFIX)/lib/$(shell echo $(TARGET) | tr A-Z a-z)
ALL_LIBS_DemoProduct = $(wildcard $(FOTAHUB_LIBDIR)/libfotahub.a) $(foreach ldir,$(LIBDIRS),$(wildcard $(ldir)/libfotahub.a))
ALL_LIB_FLAGS_DemoProduct = $(addprefix -l,$(LIBS_DemoProduct)) $(addprefix -l,$(EXT_LIBS_DemoProduct))
ALL_LIBDIR_FLAGS_DemoProduct = $(addprefix -L,$(LIBDIRS_DemoProduct)) $(addprefix -L,$(LIBDIRS)) $(addprefix -L,$(EXT_LIBDIRS_DemoProduct))

LIB_INCDIRS_DemoProduct = $(FOTAHUB_INCDIR)
EXT_INCDIRS_DemoProduct = 
ALL_INCDIR_FLAGS_DemoProduct = $(addprefix -I,$(LIB_INCDIRS_DemoProduct)) $(addprefix -I,$(INCDIRS)) $(addprefix -I,$(EXT_INCDIRS_DemoProduct))
ALL_INCS_DemoProduct = BinHexConverter.h Configuration.h DemoFOTAUpdateSelfTest.h DemoFOTAUpdateWorkflow.h DemoFOTAUpdateWorkflowFilePersistence.h DemoFOTAUpdateWorkflowPersistence.h DemoProductInfo.h Main.h SinglePartitionDemoFirmwareUpdateInfoFileReader.h $(foreach lidir,$(LIB_INCDIRS_DemoProduct),$(wildcard $(lidir)/*.h)) $(foreach idir,$(INCDIRS),$(wildcard $(idir)/*.h)) $(foreach eidir,$(EXT_INCDIRS_DemoProduct),$(wildcard $(eidir)/*.h))

ALL_SRCS_DemoProduct = BinHexConverter.c Configuration.c DemoFOTAUpdateSelfTest.c DemoFOTAUpdateWorkflow.c DemoFOTAUpdateWorkflowFilePersistence.c Main.c SinglePartitionDemoFirmwareUpdateInfoFileReader.c

ALL_OBJS_DemoProduct = $(addsuffix .o,$(call replaceDir,$(OBJODIR),$(ALL_SRCS_DemoProduct)))

#  
# End DemoProduct Executable
#  

EXTRA_CLEAN_AFKTS ?= 

.PHONY: all allDependencies clean cleanDependencies cleanAll targets

all: allDependencies $(BINODIR)/$(EXAFKT_DemoProduct)$(EXEXT_DemoProduct)

allDependencies: 
ifeq ($(BUILDABLE_DemoProduct),true)
	$(if $(wildcard $(FOTAHUB_SRCDIR)/Makefile),$(Q) $(MAKE) -C $(FOTAHUB_SRCDIR) BUILDROOT=$(BUILDROOT) TARGET=$(TARGET) FLAVOR=$(FLAVOR) PREFIX=$(PREFIX),)
endif

clean: 
	$(if $(wildcard $(filter-out . ..,$(OROOT))),$(vecho) "RM $(notdir $(OROOT))",)
	$(Q) $(RM) -r $(filter-out . ..,$(OROOT))
	$(if $(wildcard DemoProduct$(EXEXT_DemoProduct)),$(vecho) "RM DemoProduct$(EXEXT_DemoProduct)",)
	$(Q) $(RM) DemoProduct$(EXEXT_DemoProduct)
	$(if $(wildcard $(filter-out . ..,$(EXTRA_CLEAN_AFKTS))),$(vecho) "RM $(filter-out . ..,$(EXTRA_CLEAN_AFKTS))",)
	$(Q) $(RM) -r $(filter-out . ..,$(EXTRA_CLEAN_AFKTS))

cleanDependencies: 
	$(if $(wildcard $(FOTAHUB_SRCDIR)/Makefile),$(Q) $(MAKE) -C $(FOTAHUB_SRCDIR) clean BUILDROOT=$(BUILDROOT) TARGET=$(TARGET) FLAVOR=$(FLAVOR) PREFIX=$(PREFIX),)

cleanAll: clean cleanDependencies

targets: 
	$(vecho) $(sort $(SUPPORTED_TARGETS_DemoProduct))

$(OBJODIR): 
	$(Q) mkdir -p $@

define CompileSources
$$(OBJODIR)/%.c.o: $(1)/%.c $$(ALL_INCS_$(2)) Makefile | $$(OBJODIR)
	$$(vecho) "CC $$<"
	$$(Q) $$(CC) $$(ALL_CFLAGS_$(2)) $$(ALL_INCDIR_FLAGS_$(2)) -c -o $$@ $$<
endef
$(foreach sdir, $(call parentDirs,$(ALL_SRCS_DemoProduct)), $(eval $(call CompileSources,$(sdir),DemoProduct)))

$(BINODIR): 
	$(Q) mkdir -p $@

define LinkObjectsToExecutable
ifeq ($$(BUILDABLE_$(1)),true)
$$(BINODIR)/$$(EXAFKT_$(1))$$(EXEXT_$(1)): $$(ALL_OBJS_$(1)) $$(ALL_LIBS_$(1)) | $$(BINODIR)
	$$(vecho) "LD $$(notdir $$@)"
	$$(Q) $$(LD) $$(LDFLAGS) -o $$@ $$(filter %.o,$$^) $$(ALL_LIBDIR_FLAGS_$(1)) $$(ALL_LIB_FLAGS_$(1))
else
$$(BINODIR)/$$(EXAFKT_$(1))$$(EXEXT_$(1)): 
	$$(vecho) "Cannot build $$(if $$(TARGET_SPECIFIC_$(1)),$$(SUPPORTED_TARGETS_$(1))-specific ,)'$$(EXAFKT_$(1))' application$$(if $$(and $$(TARGET_SPECIFIC_$(1)),$$(MATCHES_TARGET_$(1))),, for $$(TARGET))$$(if $$(and $$(filter-out $$(TARGET),$$(HOST)),$$(MATCHES_TARGET_$(1))), under $$(HOST),)."
endif
endef
$(eval $(call LinkObjectsToExecutable,DemoProduct))
