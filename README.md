# ESP32 Unity FFF demo

Simple project that shows how to use [Unity](https://github.com/ThrowTheSwitch/Unity) 
and [FFF (Fake Function Framework)](https://github.com/meekrosoft/fff) for unit 
testing applications developed with [ESP-IDF](https://github.com/espressif/esp-idf).

This example makes use of the Unity component already present on ESP-IDF and 
illustrates a way to integrate test doubles into your application's unit tests.

# Contents
- [Requirements](#requirements)
- [Example layout](#example-layout)
- [Unit tests for components](#unit-tests-for-components)
  - [Tests without test doubles](#tests-without-test-doubles)
  - [Tests with own test doubles](#tests-with-own-test-doubles)
  - [Tests with shared test doubles](#tests-with-shared-test-doubles)
- [Shared test doubles](#shared-test-doubles)
- [How to add this to your project](#how-to-add-this-to-your-project)
- [How to run the example](#how-to-run-the-example)
  - [Run **application** project](#run-application-project)
  - [Run **test** project](#run-test-project)

## Requirements
For running this example you will need the following:
- [ESP32 toolchain](https://docs.espressif.com/projects/esp-idf/en/v3.3/get-started/index.html#setup-toolchain)
- [ESP-IDF v3.3](https://docs.espressif.com/projects/esp-idf/en/v3.3/get-started/index.html#get-esp-idf)
- Target hardware with at least one GPIO that can be used as digital input. 
  Have on hand this GPIO number, you will need it to build the application project.

## Example layout
The example follows the common ESP-IDF based project layout, with custom components 
in the `components/` directory, the `main` component and the correspondig makefiles.
This is the **application** project.

```
esp32-unity-fff-demo        - Application project directory
  - components              - Application project components
    + button
    + mockable
    + testable
  + main                    - Application project main component
  + test                    - Test project directory
  Makefile                  - Application project makefile
  README.md                 - This README
  sdkconfig.defaults        - Application project default settings
  UNLICENSE
```

In addition to this, there is a similar structure inside the `test/` folder, 
this is the **test** project and will run the application components tests.

```
esp32-unity-fff-demo        - Application project directory
  + components              - Application project components
  + main                    - Application project main component
  - test                    - Test project directory
    - components            - Test project components
      + fff                 - Fake Function Framework component directory
      + mocks               - Shared test doubles component
    + main                  - Test project main component
    Makefile                - Test project makefile
    sdkconfig.defaults      - Test project default settings
    test_component.mk       - Custom makefile for tests components using test doubles.
  Makefile                  - Application project makefile
  README.md                 - This README
  sdkconfig.defaults        - Application project default settings
  UNLICENSE
```

## Unit tests for components

Each component to be tested has a `test/` folder inside containing tests source files 
and component makefile.

### Tests without test doubles

Components that do not require test doubles are the simplest ones and follow this structure:

```
esp32-unity-fff-demo        - Application project directory
  - components              - Application project components
    + button
    - mockable
      + include             - Component header files directory
      - test                - Component tests directory
        component.mk        - Component tests makefile
        mockable_test.c     - Component tests source file
      component.mk          - Component makefile
      mockable.c            - Component source file
    + testable
  + main                    - Application project main component
  + test                    - Test project directory
  Makefile                  - Application project makefile
  README.md                 - This README
  sdkconfig.defaults        - Application project default settings
  UNLICENSE
```

Its `component.mk` file (the one inside `test/` folder) just contains:

```
COMPONENT_ADD_LDFLAGS = -Wl,--whole-archive -l$(COMPONENT_NAME) -Wl,--no-whole-archive
```

### Tests with own test doubles

Components that require test doubles, but those doubles are not require for other 
components tests, follow this structure:

```
esp32-unity-fff-demo        - Application project directory
  - components              - Application project components
    + button
    + mockable
    + testable
      + include             - Component header files directory
      - test                - Component tests directory
        - mocks             - Require test doubles directory
          + include         - Test doubles header files directory
          mockable_mocks.c  - Test doubles source file
        component.mk        - Component tests makefile
        testable_test.c     - Component tests source file
      component.mk          - Component makefile
      testable.c            - Component source file
  + main                    - Application project main component
  + test                    - Test project directory
  Makefile                  - Application project makefile
  README.md                 - This README
  sdkconfig.defaults        - Application project default settings
  UNLICENSE
```

The `component.mk` file inside `test/` folder should contain:

```
COMPONENT_OWNBUILDTARGET := 1
COMPONENT_OWNCLEANTARGET := 1
COMPONENT_ADD_LDFLAGS = -Wl,--whole-archive -l$(COMPONENT_NAME) -Wl,--no-whole-archive

# Source directories with mocks for this test component. 
export MOCK_SRCDIRS := mocks

# Pass this component source dirs to sub-make
export COMPONENT_SRCDIRS

.PHONY: build
build:
	$(MAKE) -C $(COMPONENT_BUILD_DIR) -f $(PROJECT_PATH)/test_component.mk build

.PHONY: clean
clean:
	$(MAKE) -C $(COMPONENT_BUILD_DIR) -f $(PROJECT_PATH)/test_component.mk clean
```

The above tells the build system to use custom build and clean targets for 
component tests to allow linking to test doubles instead of real dependencies.

### Tests with shared test doubles

Components that require test doubles that are common to other components tests, 
follow the same structure as those that do not require test doubles at all:

```
esp32-unity-fff-demo        - Application project directory
  - components              - Application project components
    + button
      + include             - Component header files directory
      - test                - Component tests directory
        component.mk        - Component tests makefile
        button_test.c       - Component tests source file
      component.mk          - Component makefile
      button.c              - Component source file
    + mockable
    + testable
  + main                    - Application project main component
  + test                    - Test project directory
  Makefile                  - Application project makefile
  README.md                 - This README
  sdkconfig.defaults        - Application project default settings
  UNLICENSE
```

The `component.mk` file inside `test/` folder is very similar to that of tests 
that has own test doubles, but the variable `MOCK_SRCDIRS` should be left empty 
or can be omitted.

```
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
```

## Shared test doubles

These are part of a component named `mocks` of the **test** project:

```
esp32-unity-fff-demo        - Application project directory
  + components              - Application project components
  + main                    - Application project main component
  - test                    - Test project directory
    - components            - Test project components
      + fff                 - Fake Function Framework component directory
      - mocks               - Shared test doubles component
        + include           - Test doubles header files directory
        component.mk        - Test doubles component makefile
        freertos_mocks.c    - Test doubles source file (FreeRTOS doubles)
        gpio_mocks.c        - Test doubles source file (GPIO doubles)
    + main                  - Test project main component
    Makefile                - Test project makefile
    sdkconfig.defaults      - Test project default settings
    test_component.mk       - Custom makefile for tests components using test doubles.
  Makefile                  - Application project makefile
  README.md                 - This README
  sdkconfig.defaults        - Application project default settings
  UNLICENSE
```

The `component.mk` file of the shared test doubles component should contain:

```
# Make mocked symbols hidden.
CFLAGS += -fvisibility=hidden
CXXFLAGS += -fvisibility=hidden

# Do not link with mocks lib
COMPONENT_ADD_LDFLAGS := 
```

## How to add this to your project

You will need to copy the `test_component.mk` file inside this **test** project 
folder to your **test** project folder and add test doubles source files for 
your components tests according to the cases described above. To write your 
test doubles and unit tests, you may want to take a look into the source files 
of this example.

## How to run the example

Open a terminal, go to the directory where you want to put this example and run:
```
git clone --recursive https://gitlab.com/deltalejo/esp32-unity-fff-demo.git
```
This will download the project into folder `esp32-unity-fff-demo`.

### Run **application** project

Go to `esp32-unity-fff-demo` folder, e.g.: `cd esp32-unity-fff-demo`

#### Configure

Run `make menuconfig` and set the serial port under **Serial Flasher Options**.
Under **Example Configuration**, enter the GPIO number that will be used as input.
Make sure logging output level is at least *info*, this is the default.

#### Build and flash

Connect target hardware and run:
```
make -j(number of CPUs + 1) flash monitor
```

#### Sample output

Ignoring boot logs, main application will show:

```
I (47) main: Hello from app_main()!
I (47) gpio: GPIO[34]| InputEn: 1| OutputEn: 0| OpenDrain: 0| Pullup: 0| Pulldown: 0| Intr:2 
```

After pressing a button attached to the configured GPIO, in this case GPIO 34,
will log:

```
I (47) main: Hello from app_main()!
I (47) gpio: GPIO[34]| InputEn: 1| OutputEn: 0| OpenDrain: 0| Pullup: 0| Pulldown: 0| Intr:2 

I (1597) testable: testable_calls_mockable() - Lets see if I call the real function...
I (1597) mockable: I'm the real mockable_function()!
I (1597) testable: testable_calls_mockable() - It seems real.

```

Subsequent presses will append the last three logs:

```
I (47) main: Hello from app_main()!
I (47) gpio: GPIO[34]| InputEn: 1| OutputEn: 0| OpenDrain: 0| Pullup: 0| Pulldown: 0| Intr:2 

I (1597) testable: testable_calls_mockable() - Lets see if I call the real function...
I (1597) mockable: I'm the real mockable_function()!
I (1597) testable: testable_calls_mockable() - It seems real.


I (4667) testable: testable_calls_mockable() - Lets see if I call the real function...
I (4667) mockable: I'm the real mockable_function()!
I (4667) testable: testable_calls_mockable() - It seems real.

```

### Run **test** project

Go to `esp32-unity-fff-demo/test` folder, e.g.: `cd test`

#### Configure

Run `make menuconfig` and set the serial port under **Serial Flasher Options**.
Make sure logging output level is at least *info*, this is the default.

#### Build and flash

Connect target hardware and run:
```
make -j(number of CPUs + 1) flash monitor
```

#### Sample output

Ignoring boot logs, test application will show:

```
I (60) main: app_main() - First, call some real functions...
I (66) mockable: I'm the real mockable_function()!
I (72) testable: testable_calls_mockable() - Lets see if I call the real function...
I (80) mockable: I'm the real mockable_function()!
I (86) testable: testable_calls_mockable() - It seems real.
I (92) main: app_main() - Then, go to test menu...


Press ENTER to see the list of tests.

```

And the tests list:

```
Here's the test menu, pick your combo:
(1)     "mockable_function(): Should return 0" [mockable]
(2)     "testable_calls_mockable(): Should return true if mockable_function() returns zero" [testable]
(3)     "testable_calls_mockable(): Should return false if mockable_function() returns non-zero" [testable]
(4)     "button_init(): Should config given GPIO as input" [button]
(5)     "button_init(): Should enable falling edge interrupts on given GPIO" [button]
(6)     "button_init(): Should install GPIO ISR service" [button]
(7)     "button_init(): Should add ISR handler with given GPIO as argument" [button]
(8)     "button_init(): Should create queue for at least one gpio_num_t elements" [button]
(9)     "button_init(): Should create queue before adding ISR handler" [button]
(10)    "button_init(): Should create queue before creating task" [button]
(11)    "button_init(): Should create task with name 'button_task'" [button]
(12)    "button_isr(): Should send given argument to queue" [button]
(13)    "button_task(): Should should wait on queue for button event" [button]
(14)    "button_task(): Should call registered callback when receive a button event" [button]
(15)    "button_task(): Should not call registered callback when there is any button event" [button]

Enter test for running.

```

- Running tests with tag [mockable]:

```
Running tests matching '[mockable]'...
Running mockable_function(): Should return 0...
I (5039) mockable: I'm the real mockable_function()!
/mnt/Alejo/Workspaces/esp/projects/esp32-unity-fff-demo/components/mockable/test/mockable_test.c:16:mockable_function(): Should return 0:PASS

-----------------------
1 Tests 0 Failures 0 Ignored 
OK
```

- Running tests with tag [testable]:

```
Running tests matching '[testable]'...
Running testable_calls_mockable(): Should return true if mockable_function() returns zero...
I (10004) testable: testable_calls_mockable() - Lets see if I call the real function...
I (10004) testable: testable_calls_mockable() - It seems real.
/mnt/Alejo/Workspaces/esp/projects/esp32-unity-fff-demo/components/testable/test/testable_test.c:17:testable_calls_mockable(): Should return true if mockable_function() returns zero:PASS
Running testable_calls_mockable(): Should return false if mockable_function() returns non-zero...
I (10049) testable: testable_calls_mockable() - Lets see if I call the real function...
W (10049) testable: testable_calls_mockable() - It seems it has been mocked!
/mnt/Alejo/Workspaces/esp/projects/esp32-unity-fff-demo/components/testable/test/testable_test.c:27:testable_calls_mockable(): Should return false if mockable_function() returns non-zero:PASS

-----------------------
2 Tests 0 Failures 0 Ignored 
OK
```

- Running tests with tag [button]:

```
Running tests matching '[button]'...
Running button_init(): Should config given GPIO as input...
/mnt/Alejo/Workspaces/esp/projects/esp32-unity-fff-demo/components/button/test/button_test.c:66:button_init(): Should config given GPIO as input:PASS
Running button_init(): Should enable falling edge interrupts on given GPIO...
/mnt/Alejo/Workspaces/esp/projects/esp32-unity-fff-demo/components/button/test/button_test.c:87:button_init(): Should enable falling edge interrupts on given GPIO:PASS
Running button_init(): Should install GPIO ISR service...
/mnt/Alejo/Workspaces/esp/projects/esp32-unity-fff-demo/components/button/test/button_test.c:108:button_init(): Should install GPIO ISR service:PASS
Running button_init(): Should add ISR handler with given GPIO as argument...
/mnt/Alejo/Workspaces/esp/projects/esp32-unity-fff-demo/components/button/test/button_test.c:125:button_init(): Should add ISR handler with given GPIO as argument:PASS
Running button_init(): Should create queue for at least one gpio_num_t elements...
/mnt/Alejo/Workspaces/esp/projects/esp32-unity-fff-demo/components/button/test/button_test.c:145:button_init(): Should create queue for at least one gpio_num_t elements:PASS
Running button_init(): Should create queue before adding ISR handler...
/mnt/Alejo/Workspaces/esp/projects/esp32-unity-fff-demo/components/button/test/button_test.c:164:button_init(): Should create queue before adding ISR handler:PASS
Running button_init(): Should create queue before creating task...
/mnt/Alejo/Workspaces/esp/projects/esp32-unity-fff-demo/components/button/test/button_test.c:186:button_init(): Should create queue before creating task:PASS
Running button_init(): Should create task with name 'button_task'...
/mnt/Alejo/Workspaces/esp/projects/esp32-unity-fff-demo/components/button/test/button_test.c:207:button_init(): Should create task with name 'button_task':PASS
Running button_isr(): Should send given argument to queue...
/mnt/Alejo/Workspaces/esp/projects/esp32-unity-fff-demo/components/button/test/button_test.c:228:button_isr(): Should send given argument to queue:PASS
Running button_task(): Should should wait on queue for button event...
/mnt/Alejo/Workspaces/esp/projects/esp32-unity-fff-demo/components/button/test/button_test.c:270:button_task(): Should should wait on queue for button event:PASS
Running button_task(): Should call registered callback when receive a button event...
/mnt/Alejo/Workspaces/esp/projects/esp32-unity-fff-demo/components/button/test/button_test.c:312:button_task(): Should call registered callback when receive a button event:PASS
Running button_task(): Should not call registered callback when there is any button event...
/mnt/Alejo/Workspaces/esp/projects/esp32-unity-fff-demo/components/button/test/button_test.c:376:button_task(): Should not call registered callback when there is any button event:PASS

-----------------------
12 Tests 0 Failures 0 Ignored 
OK
```
