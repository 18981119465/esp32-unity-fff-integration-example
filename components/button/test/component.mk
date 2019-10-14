COMPONENT_OWNBUILDTARGET := 1
COMPONENT_OWNCLEANTARGET := 1
COMPONENT_ADD_LDFLAGS = -Wl,--whole-archive -l$(COMPONENT_NAME) -Wl,--no-whole-archive

# Source directories with mocks for this test component. 
export MOCK_SRCDIRS := 

# Pass this component source dirs to sub-make
export COMPONENT_SRCDIRS

.PHONY: build
build:
	$(MAKE) -C $(COMPONENT_BUILD_DIR) -f $(PROJECT_PATH)/test_component.mk build

.PHONY: clean
clean:
	$(MAKE) -C $(COMPONENT_BUILD_DIR) -f $(PROJECT_PATH)/test_component.mk clean
