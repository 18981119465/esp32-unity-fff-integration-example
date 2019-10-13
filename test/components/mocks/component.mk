# Make mocked symbols hidden.
CFLAGS += -fvisibility=hidden
CXXFLAGS += -fvisibility=hidden

# Do not link with mocks lib
COMPONENT_ADD_LDFLAGS := 
