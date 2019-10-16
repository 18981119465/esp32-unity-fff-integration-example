# ESP32 Unity FFF demo

Simple project that shows how to use [Unity](https://github.com/ThrowTheSwitch/Unity) 
and [FFF (Fake Function Framework)](https://github.com/meekrosoft/fff) for unit 
testing applications developed with [ESP-IDF](https://github.com/espressif/esp-idf).

This example makes use of the Unity component already present on ESP-IDF and 
illustrates a way to integrate test doubles into your application unit tests suite.

## Prerequisites
For running this example you will need the following:
- [ESP32 toolchain](https://docs.espressif.com/projects/esp-idf/en/v3.3/get-started/index.html#setup-toolchain)
- [ESP-IDF v3.3](https://docs.espressif.com/projects/esp-idf/en/v3.3/get-started/index.html#get-esp-idf)
- Target hardware with at least one GPIO that can be used as digital input. Have on hand this GPIO number, you will need it to build the project.

## Project layout
The example follows the common ESP-IDF based project layout, with custom components 
in the `components/` directory, the `main` component and the correspondig makefiles.
This is the **main** application.

In addition to this, there is another project, following a similar structure, in 
the `test/` folder, this is the **test** project and it will run the components tests.

```
esp32-unity-fff-demo
  - components
    + button
    + mockable
    + testable
  + main
  + test
  Makefile
  README.md
  sdkconfig.defaults
```
