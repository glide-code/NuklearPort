# About
This repository contains SDL3 and Blend2D backends for software rendering in Nuklear.
Simply include the files in ```backends/``` directory in your own project.
An example application is provided under ```test/``` directory.

## Dependencies:
* [SDL3](https://github.com/libsdl-org/sdl)
* [Nuklear](https://github.com/Immediate-Mode-UI/Nuklear)
* [Blend2D](https://github.com/blend2d/blend2d)

# Building example
This project uses [CMake](https://cmake.org/) to build the example application.
```
git clone https://github.com/glide-code/NuklearPort.git
cd NuklearPort
mkdir build
cmake -S . -B build
cd build
cmake --build .
```

# Warning
The example application uses Segoe UI font by default. You may want to change it on non-Windows systems.
