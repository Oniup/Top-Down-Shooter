# Top-Down Shooter Game

a small project for uni where I have to create a custom application. I have decided to create a game that is build with C and OpenGL as the renderer

## Install

### Requirements

* C/C++ compiler *(clang, gcc, MSVS'S clang-cl (windows))*
* cmake
* linux only:
  * make

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

## Dependencies

* [glfw](https://www.glfw.org/)
* [glad](https://glad.dav1d.de/)
* [stb](https://github.com/nothings/stb)
* [freetype](https://freetype.org/)
* [cglm](https://github.com/recp/cglm.git)
