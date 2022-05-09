# Top-Down Shooter Game

a small project for uni where I decided to create a small topdown shooter game with custom game engine. The program only use functional/procedural programming, thus the game/engine is programmed using purely C and OpenGL

***Some of the Goals:***

* To make the application cross platform and to make the entire build process easier by using cmake rather than what I did before (just giving the clang/gcc compile command which usually ended up failing due to linker errors)
* make a 2D batch renderer using OpenGL for the graphics API
* make an efficient ECS (entity component system)
  * order the components in the ideal position in the stack (calling the update loops (physics related last), then render everything to the screen)

## Install

### Requirements

* C/C++ compiler *(the ones that I have tested it with: (linux: clang, gcc), (windows: MSVS'S clang-cl))*
* cmake
* ***linux***:
  * make
* ***windows***:
  * Microsoft visual studio (easiest way. Can use MinGW's clang/gcc but its not as easy)

### Steps

once you have cloned the repo, create a directory where you are going to build the program

```bash
~/..$ mkdir build
~/..$ cd build
~/../build$
```

cmake using default compiler

```bash
~/../build$ cmake ../
```

cmake using different compiler (replace clang with whatever)

```bash
~/../build$ cmake -D CMAKE_C_COMPILER=clang ../
```

#### Windows

cmake should create the files that are needed to build the program using MS visual studio

open the .sln file and build (ctr+shift+b)

the executable will be located in the Debug/Release folder

#### Linux

cmake should create a make file that we will use to build the program

after that, can run the executable by using ```./```

```bash
~/../build$ make
~/../build$ ./topdown_shooter
```

### The Build folder

once you have compiled and built the project using cmake, you have to move the art and shader files
to that build folder.

***Make sure to move***

* safire's shaders and textures folders into the safire's folder in the build
* top-down shooter game's art and shader folder (this just goes in the root)

*file structure of the final build folder should look like the following:*

* .cmake/
* art/
* CMakeFiles/
* mock
* safire/
* safire/shaders
* safire/textures
* shooter_game (exec)

## Dependencies

* [glfw](https://www.glfw.org/)
* [glad](https://glad.dav1d.de/)
* [stb](https://github.com/nothings/stb)
<!-- * [freetype](https://freetype.org/) -->
* [cglm](https://github.com/recp/cglm.git)
