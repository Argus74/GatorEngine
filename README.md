<p align="center">
  <img src="https://github.com/Argus74/GatorEngine/blob/main/GatorEngineLogo.png?raw=true" alt="Gator Engine Logo"/>
</p>

# Overview
Gator Engine is a 2D game engine that offers a simple, but flexible, environment for students to learn game development principles. The engine attempts to bridge the gap between overly simplistic and complex game development tools, offering instead a graduated learning experience that is both engaging and educational.

# Building

## Requirements
- CMake
- Visual Studio / MSVC  or Clang

## Cloning
First, clone the GitHub repository by running the following command in Command Prompt:

```
git clone https://github.com/Argus74/GatorEngine.git
```

Then, move into the newly-created project directory

```
cd GatorEngine
```

From here, you must build the project using CMake. How this looks will depend on the compiler you're using

## Building with Visual Studio / MSVC (Windows)
Visual Studio conveniently has CMake tools available that can automate the build process for you.
To use them, open the project with Visual Studio either manually or with the following command:

```
start devenv . 
```

Visual Studio will then begin to build the project. See the 'Output' window of Visual Studio for build details.
Once built, you can run the GatorEngine executable from Visual Studio by 
1. Press 'Select Startup Item'
2. Find and select GatorEngine.exe from the list of items
3. Run!

*NOTE: While we have not officially tested for Visual Studio 2019 or below, the same build steps should work regardless of VS release.*

## Building with Clang (Mac)
If Visual Studio is not available on your machine, you may still build the project from a command line interface.

First, generate the CMake for SFML by running the following commands:
```
cd extern/sfml
cmake .
cd ../..
```

Next, back in the project directory, create and move into a new folder to store the build output with:

```
mkdir out
cd out
```

then, generate the project's CMake:

```
cmake ..
```

and, finally, build the project in the out/ folder:

```
cmake --build .
```

*NOTE: Verify that the first line of output following this command reads something like "-- The C compiler identification is AppleClang X.X..."*






