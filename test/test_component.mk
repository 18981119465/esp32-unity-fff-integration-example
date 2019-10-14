COMPONENT_LIBRARY := lib$(COMPONENT_NAME).a
COMPONENT_SRCDIRS ?= .

PARENT_COMPONENT_NAME := $(subst _test,,$(COMPONENT_NAME))
PARENT_COMPONENT_LIBRARY := lib$(PARENT_COMPONENT_NAME).a
PARENT_COMPONENT_BUILD_DIR := $(BUILD_DIR_BASE)/$(PARENT_COMPONENT_NAME)
PARENT_COMPONENT_OBJS != $(AR) t $(PARENT_COMPONENT_BUILD_DIR)/$(PARENT_COMPONENT_LIBRARY)
PARENT_COMPONENT_MOCK_OBJS := $(addprefix mocked/,$(PARENT_COMPONENT_OBJS))

# Project mocks
ifeq (mocks,$(findstring mocks,$(COMPONENTS)))
	MOCK_LIBRARY := libmocks.a
else
	MOCK_LIBRARY := 
endif

# Component mocks.
MOCK_SRCDIRS ?= 
MOCK_INCLUDEDIRS ?= $(MOCK_SRCDIRS) $(MOCK_SRCDIRS)/include
MOCK_INCLUDEDIRS := $(addprefix $(COMPONENT_PATH)/,$(MOCK_INCLUDEDIRS))


COMPONENT_SRCS := $(foreach srcdir,$(COMPONENT_SRCDIRS), $(wildcard $(COMPONENT_PATH)/$(srcdir)/*.c))
COMPONENT_SRCS += $(foreach srcdir,$(COMPONENT_SRCDIRS), $(wildcard $(COMPONENT_PATH)/$(srcdir)/*.cpp))
COMPONENT_SRCS += $(foreach srcdir,$(COMPONENT_SRCDIRS), $(wildcard $(COMPONENT_PATH)/$(srcdir)/*.cc))
COMPONENT_SRCS := $(patsubst $(COMPONENT_PATH)/%,%,$(COMPONENT_SRCS))

COMPONENT_OBJS := $(patsubst %.c,%.o,$(filter %.c,$(COMPONENT_SRCS)))
COMPONENT_OBJS += $(patsubst %.cpp,%.o,$(filter %.cpp,$(COMPONENT_SRCS)))
COMPONENT_OBJS += $(patsubst %.cc,%.o,$(filter %.cc,$(COMPONENT_SRCS)))

COMPONENT_DEPS := $(COMPONENT_OBJS:.o=.d)


MOCK_SRCS := $(foreach srcdir,$(MOCK_SRCDIRS), $(wildcard $(COMPONENT_PATH)/$(srcdir)/*.c))
MOCK_SRCS += $(foreach srcdir,$(MOCK_SRCDIRS), $(wildcard $(COMPONENT_PATH)/$(srcdir)/*.cpp))
MOCK_SRCS += $(foreach srcdir,$(MOCK_SRCDIRS), $(wildcard $(COMPONENT_PATH)/$(srcdir)/*.cc))
MOCK_SRCS := $(patsubst $(COMPONENT_PATH)/%,%,$(MOCK_SRCS))

MOCK_OBJS := $(patsubst %.c,%.o,$(filter %.c,$(MOCK_SRCS)))
MOCK_OBJS += $(patsubst %.cpp,%.o,$(filter %.cpp,$(MOCK_SRCS)))
MOCK_OBJS += $(patsubst %.cc,%.o,$(filter %.cc,$(MOCK_SRCS)))

MOCK_DEPS := $(MOCK_OBJS:.o=.d)

TEST_COMPONENT_MAKEFILE_DEPS := $(COMMON_MAKEFILES) $(COMPONENT_MAKEFILE) $(PROJECT_PATH)/test_component.mk


.PHONY: build
build: $(COMPONENT_LIBRARY)

.PHONY: clean
clean: CLEAN_FILES := $(COMPONENT_LIBRARY) $(COMPONENT_OBJS) $(COMPONENT_DEPS) $(MOCK_OBJS) $(MOCK_DEPS) component_project_vars.mk
clean:
	@echo RM $(CLEAN_FILES)
	rm -f $(CLEAN_FILES)
	rm -rf real mocked

$(COMPONENT_LIBRARY): $(COMPONENT_OBJS)
	@echo AR $(patsubst $(PWD)/%,%,$(CURDIR))/$@
	rm -f $@
	$(AR) $(ARFLAGS) $@ $(COMPONENT_OBJS)

-include $(COMPONENT_DEPS) $(MOCK_DEPS)

# Component objects targets.
$(patsubst %.c,%.o,$(filter %.c,$(COMPONENT_SRCS))): %.o: $(COMPONENT_PATH)/%.c $(PARENT_COMPONENT_LIBRARY) $(if $(MOCK_LIBRARY),$(BUILD_DIR_BASE)/mocks/$(MOCK_LIBRARY)) $(TEST_COMPONENT_MAKEFILE_DEPS)
	@echo CC $(patsubst $(PWD)/%,%,$(CURDIR))/$@
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(addprefix -I ,$(COMPONENT_INCLUDES)) $(addprefix -I ,$(MOCK_INCLUDEDIRS)) -I $(abspath $(dir $<)) -L. $(if $(MOCK_LIBRARY),-L$(BUILD_DIR_BASE)/mocks) -Wl,-r,--exclude-libs,ALL $(abspath $<) -l$(PARENT_COMPONENT_NAME) $(if $(MOCK_LIBRARY),-lmocks) -o $@
	$(OBJCOPY) --localize-hidden $@

$(patsubst %.cpp,%.o,$(filter %.cpp,$(COMPONENT_SRCS))): %.o: $(COMPONENT_PATH)/%.cpp $(PARENT_COMPONENT_LIBRARY) $(TEST_COMPONENT_MAKEFILE_DEPS)
	@echo CXX $(patsubst $(PWD)/%,%,$(CURDIR))/$@
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(addprefix -I ,$(COMPONENT_INCLUDES)) $(addprefix -I ,$(MOCK_INCLUDEDIRS)) -I $(abspath $(dir $<)) -L. $(if $(MOCK_LIBRARY),-L$(BUILD_DIR_BASE)/mocks) -Wl,-r,--exclude-libs,ALL $(abspath $<) -l$(PARENT_COMPONENT_NAME) $(if $(MOCK_LIBRARY),-lmocks) -o $@
	$(OBJCOPY) --localize-hidden $@

$(patsubst %.cc,%.o,$(filter %.cc,$(COMPONENT_SRCS))): %.o: $(COMPONENT_PATH)/%.cc $(PARENT_COMPONENT_LIBRARY) $(TEST_COMPONENT_MAKEFILE_DEPS)
	@echo CXX $(patsubst $(PWD)/%,%,$(CURDIR))/$@
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(addprefix -I ,$(COMPONENT_INCLUDES)) $(addprefix -I ,$(MOCK_INCLUDEDIRS)) -I $(abspath $(dir $<)) -L. $(if $(MOCK_LIBRARY),-L$(BUILD_DIR_BASE)/mocks) -Wl,-r,--exclude-libs,ALL $(abspath $<) -l$(PARENT_COMPONENT_NAME) $(if $(MOCK_LIBRARY),-lmocks) -o $@
	$(OBJCOPY) --localize-hidden $@

# Mocks objects targets.
$(patsubst %.c,%.o,$(filter %.c,$(MOCK_SRCS))): %.o: $(COMPONENT_PATH)/%.c $(TEST_COMPONENT_MAKEFILE_DEPS)
	@echo CC $(patsubst $(PWD)/%,%,$(CURDIR))/$@
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(addprefix -I ,$(COMPONENT_INCLUDES)) $(addprefix -I ,$(MOCK_INCLUDEDIRS)) -I $(abspath $(dir $<)) -fvisibility=hidden -c $(abspath $<) -o $@

$(patsubst %.cpp,%.o,$(filter %.cpp,$(MOCK_SRCS))): %.o: $(COMPONENT_PATH)/%.cpp $(TEST_COMPONENT_MAKEFILE_DEPS)
	@echo CXX $(patsubst $(PWD)/%,%,$(CURDIR))/$@
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(addprefix -I ,$(COMPONENT_INCLUDES)) $(addprefix -I ,$(MOCK_INCLUDEDIRS)) -I $(abspath $(dir $<)) -fvisibility=hidden -c $(abspath $<) -o $@

$(patsubst %.cc,%.o,$(filter %.cc,$(MOCK_SRCS))): %.o: $(COMPONENT_PATH)/%.cc $(TEST_COMPONENT_MAKEFILE_DEPS)
	@echo CXX $(patsubst $(PWD)/%,%,$(CURDIR))/$@
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(addprefix -I ,$(COMPONENT_INCLUDES)) $(addprefix -I ,$(MOCK_INCLUDEDIRS)) -I $(abspath $(dir $<)) -fvisibility=hidden -c $(abspath $<) -o $@	

$(PARENT_COMPONENT_LIBRARY): $(PARENT_COMPONENT_MOCK_OBJS)
	@echo AR $(patsubst $(PWD)/%,%,$(CURDIR))/$@
	rm -f $@
	$(AR) $(ARFLAGS) $@ $(PARENT_COMPONENT_MOCK_OBJS)

$(PARENT_COMPONENT_MOCK_OBJS): mocked/%.o: real/%.o $(MOCK_OBJS)
	@echo LD $(patsubst $(PWD)/%,%,$(CURDIR))/$@
	@mkdir -p $(@D)
	$(LD) -r $(MOCK_OBJS) $< -o $@

real/%.o: $(PARENT_COMPONENT_BUILD_DIR)/$(PARENT_COMPONENT_LIBRARY)
	@echo AR x $(patsubst $(PWD)/%,%,$(CURDIR))/$@
	@mkdir -p $(@D)
	cd $(@D) && $(AR) x $< $(@F)
